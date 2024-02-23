/*----------------------------------------------------------------------------
	Program : dbyConnect.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : 2019 - 2024
	Synopsis: Connect to mysql database. Uses config file located in 
				secure (none source code) location to lookup user name
				and database; and retrieve password and license.
	Return  : DBY_CONN_SUCCESS or error code on failure
	Note    : Create license key with program dbylicense

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
    Library for working with mysql/mariadb

    Copyright (C)  2019 - 2024 Tom Stevelt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
---------------------------------------------------------------------------*/

#include	<string.h>
#include	"shslib.h"
#include	"dbylib.h"

#define		LOGFILENAME		"/var/local/dbylib.log"
LICENSE_RECORD	LicenseInfo;
static	int		DebugConnect = 0;

char *dbyConnectErrorStr ( int ErrorCode )
{
	switch ( ErrorCode )
	{
		case DBY_CONN_SUCCESS:		return ( "dbyConnect: Success" );
		case DBY_CONN_ERR_CFG_FILE:	return ( "dbyConnect: Missing .cfg file" );
		case DBY_CONN_ERR_CFG_DB:	return ( "dbyConnect: Missing database in .cfg file" );
		case DBY_CONN_ERR_CFG_PARM:	return ( "dbyConnect: Missing parameters in .cfg file" );
		case DBY_CONN_ERR_STATUS:	return ( "dbyConnect: No status information for db" );
		case DBY_CONN_ERR_OFFLINE:	return ( "dbyConnect: Database offline" );
		case DBY_CONN_ERR_DATE:		return ( "dbyConnect: Bad status date" );
		case DBY_CONN_ERR_EXPIRED:	return ( "dbyConnect: Database expired" );
		case DBY_CONN_ERR_MYSQL:	return ( "dbyConnect: mysql error" );
		case DBY_CONN_ERR_LICENSE:	return ( "dbyConnect: No license information for db" );
		case DBY_CONN_ERR_HACKED:	return ( "dbyConnect: License key error" );
		case DBY_CONN_ERR_SYNTAX:	return ( "dbyConnect: License syntax" );
		case DBY_CONN_ERR_WRONG_DB:	return ( "dbyConnect: Wrong database" );
	}
	return ( "dbyConnect: Unknown" );
}

int dbyConnect (  MYSQL *MySql, char *DbName, char *UserName, int CheckStatus, FILE *fpError )
{
	char	xbuffer[256];
	char	*tokens[4];
	int		tokcnt;
	FILE	*fp;
	char	*fn = "/var/local/mysql.cfg";
	char	DbHost[256];
	char	DbUser[256];
	char	DbPassword[256];
	int		ParmCount = 0;
	int		InSection = 0;
	int		rv, xi, xl, xc;
	int		lineno;
	char	StatusCode[20];
	int		FoundStatus;
	DATEVAL	dvToday;
	char	LicenseKey[100];
	char	PlainText[100];
	int		FoundLicense;
	unsigned char   CheckSum;
	unsigned char	KeySum;
	int		CharCount;
	char	*cp;

	if ( DebugConnect )
	{
		fprintf ( stderr, "dbyConnect: DB [%s] User [%s] CheckStatus [%d]<br>\n", DbName, UserName, CheckStatus );
	}

	if (( fp = fopen ( fn, "r" )) == (FILE *)0 )
	{
		if ( fpError )
		{
			fprintf ( fpError, "Can not open %s for reading.", fn );
		}
		return ( DBY_CONN_ERR_CFG_FILE );
	}

	ParmCount = 0;
	lineno = 0;
	memset ( StatusCode, '\0', sizeof(StatusCode) );
	FoundStatus = 0;
	LicenseInfo.DaysToExpiration = 9999;
	InSection = 0;

	while ( InSection < 3 && fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		lineno++;

		switch ( xbuffer[0] )
		{
			case '#':
				continue;

			case '{':
				if ( InSection == 1 )
				{
					ParmCount = 0;
					InSection = 2;
				}
				continue;

			case '}':
				if ( InSection == 1 )
				{
					InSection = 0;
				}
				else if ( InSection == 2 )
				{
					InSection = 3;
				}
				continue;
		}

		if (( tokcnt = GetTokensW ( xbuffer, tokens, 3 )) < 2 )
		{
			continue;
		}

		if ( nsStrcmp ( tokens[0], "dbname" ) == 0 )
		{
			if ( nsStrcmp ( tokens[1], DbName ) == 0 )
			{
				InSection = 1;
			}
		}

		if ( InSection == 2 )
		{
			if ( DebugConnect )
			{
				fprintf ( stderr, "%d %s %s\n", tokcnt, tokens[0], tokens[1] );
			}

			if ( nsStrcmp ( tokens[0], "dbhost" ) == 0 )
			{
				snprintf ( DbHost, sizeof(DbHost), "%s", tokens[1] );
				ParmCount++;
			}
			else if ( nsStrcmp ( tokens[0], "dbuser" ) == 0 )
			{
				snprintf ( DbUser, sizeof(DbUser), "%s", tokens[1] );
				if ( nsStrcmp ( DbUser, UserName ) != 0 )
				{
					InSection = 0;
					ParmCount = 0;
					continue;
				}
				ParmCount++;
			}
			else if ( nsStrcmp ( tokens[0], "dbpassword" ) == 0 )
			{
				snprintf ( DbPassword, sizeof(DbPassword), "%s", tokens[1] );
				ParmCount++;
			}
			else if ( nsStrcmp ( tokens[0], "status" ) == 0 )
			{
				snprintf ( StatusCode, sizeof ( StatusCode ), "%-s", tokens[1] );
				FoundStatus = 1;
			}
			else if ( nsStrcmp ( tokens[0], "license" ) == 0 )
			{
				snprintf ( LicenseKey, sizeof ( LicenseKey ), "%-s", tokens[1] );
				FoundLicense = 1;
			}
			else if ( fpError )
			{
				fprintf ( fpError, "Unknown token %s in line %d.\n", tokens[0], lineno );
			}
		}
	}

	nsFclose ( fp );

	if ( ParmCount < 3 )
	{
		if ( fpError )
		{
			fprintf ( fpError, "%s does not contain requested database login information\n", fn );
		}
		return ( DBY_CONN_ERR_CFG_PARM );
	}

	if ( CheckStatus )
	{
		if ( FoundStatus == 0 )
		{
			return ( DBY_CONN_ERR_STATUS );
		}

		if ( FoundLicense == 0 )
		{
			return ( DBY_CONN_ERR_LICENSE );
		}

		CurrentDateval ( &dvToday );	

		/*----------------------------------------------------------
			check plain english status
		----------------------------------------------------------*/
		if ( nsStrcmp ( StatusCode, "lifetime" ) == 0 )
		{
			/* nothing to do, just keep going */
		}
		else if ( nsStrcmp ( StatusCode, "offline" ) == 0 )
		{
			return ( DBY_CONN_ERR_OFFLINE );
		}
		else
		{
			if ( StrToDateval ( StatusCode, &LicenseInfo.ExpirationDate ) != 0 )
			{
				return ( DBY_CONN_ERR_DATE );
			}

			if ( CompareDateval ( &LicenseInfo.ExpirationDate, &dvToday ) < 0 )
			{
				return ( DBY_CONN_ERR_EXPIRED );
			}
		}
		
		/*----------------------------------------------------------
			check encrypted license
		----------------------------------------------------------*/
		xl = sprintf ( PlainText, "%s", tmsDecode(LicenseKey) );

		if ( DebugConnect )
		{
			fprintf ( stderr, "%s\n", LicenseKey );
			fprintf ( stderr, "%s\n", PlainText );
		}

#ifdef WEAK_CHECKSUM_AND_BEFORE_APPSTR
		for ( xi = 0, xc = 0, CheckSum = 0; xi < xl; xi++ )
		{
			if ( PlainText[xi] == ':' )
			{
				xc++;
				if ( xc == 2 )
				{
					xi++;
					KeySum = nsAtoi ( &PlainText[xi] );
					break;
				}
			}

			CheckSum += PlainText[xi];
		}
#endif

#define NEW_CHECKSUM_AND_APPSTR
#ifdef NEW_CHECKSUM_AND_APPSTR
        cp = nsStrrchr ( PlainText, ':' );
        cp++;
        KeySum = nsAtoi ( cp );

        CharCount = nsCharCount ( PlainText, ':' );

        for ( xi = 0, xc = 0, CheckSum = 0; xi < xl; xi++ )
        {
            if ( PlainText[xi] == ':' )
            {
                xc++;
                if ( xc == CharCount )
                {
                    break;
                }
            }

            CheckSum += ((xi%2) ? 3 : 2) * PlainText[xi];
        }
#endif

		if ( DebugConnect )
		{
			fprintf ( stderr, "KeySum %d CheckSum %d\n", (int)KeySum,  (int)CheckSum );
		}

		if ( KeySum != (short) CheckSum )
		{
			return ( DBY_CONN_ERR_HACKED );
		}

		if (( tokcnt = GetTokensStd ( PlainText, ":", tokens, 5, 0 )) < 3 )
		{
			return ( DBY_CONN_ERR_SYNTAX );
		}

		if ( nsStrcmp ( DbName, tokens[0] ) != 0 )
		{
			return ( DBY_CONN_ERR_WRONG_DB );
		}

		StrToDatevalFmt ( tokens[1], DATEFMT_YYYYMMDD, &LicenseInfo.ExpirationDate );

		if ( CompareDateval ( &LicenseInfo.ExpirationDate, &dvToday ) < 0 )
		{
			return ( DBY_CONN_ERR_EXPIRED );
		}

		LicenseInfo.DaysToExpiration = DateDiff ( &dvToday, &LicenseInfo.ExpirationDate );

		if ( DebugConnect )
		{
			fprintf ( stderr, "tokens[1] %s,  ExpirationDate.year4 %d, Days %d\n", 
				tokens[1], LicenseInfo.ExpirationDate.year4, LicenseInfo.DaysToExpiration );
		}

		snprintf ( LicenseInfo.Database, sizeof(LicenseInfo.Database), "%s", tokens[0] );

		if ( tokcnt > 3 )
		{
			snprintf ( LicenseInfo.AppString, sizeof(LicenseInfo.AppString), "%s", tokens[2] );
		}
		else
		{
			memset ( LicenseInfo.AppString, '\0', sizeof(LicenseInfo.AppString) );
		}

		if ( DebugConnect )
		{
			fprintf ( stderr, "Database %s\n", tokens[0] );
			fprintf ( stderr, "Expires  %d/%d/%d\n", LicenseInfo.ExpirationDate.month, LicenseInfo.ExpirationDate.day, LicenseInfo.ExpirationDate.year4 );
		}
	}

	if ( DebugConnect )
	{
		fprintf ( stderr, "dbyConnect: before call dbyInitNoExit\n" );
	}

	rv = dbyInitNoExit ( MySql, DbHost, DbUser, DbPassword, DbName, 3306, LOGFILENAME );

	if ( DebugConnect )
	{
		fprintf ( stderr, "dbyConnect: dbyInitNoExit returned %d\n", rv );
	}

	if ( rv < 0 )
	{
		return ( DBY_CONN_ERR_MYSQL );
	}

	return ( DBY_CONN_SUCCESS );
}
