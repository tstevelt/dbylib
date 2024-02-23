/*----------------------------------------------------------------------------
	Program : dbyFuncs.c
	Author  : Tom Stevelt
	Date    : 12/12/2018
	Synopsis: Functions to help with mysql's way of doing things.  These do
			  not necessarily interact with the database, and could 
			  possibly located in shslib instead of dbylib.

	Return  : 

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

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"shslib.h"
#include	"dbylib.h"

/*----------------------------------------------------------
	convert 18:00:00 to 6:00 PM
----------------------------------------------------------*/
char *hms24tostr ( char *hms24 )
{
	int 	Hour, Minute;
	char	*cp;
	static	char	String[10];

	Hour = nsAtoi ( hms24 );

	if (( cp = nsStrchr ( hms24, ':' )) != (char *)0 )
	{
		cp++;
		Minute = nsAtoi ( cp );
	}
	else
	{
		Minute = 0;
	}

// printf ( "hms24tostr: Hour %d Minute %d<br>\n", Hour, Minute );

	if ( Hour < 12 )
	{
		sprintf ( String, "%d:%02d AM", Hour, Minute );
	}
	else
	{
		sprintf ( String, "%d:%02d PM", (Hour - 12), Minute );
	}

	return ( String );
}

/*----------------------------------------------------------
	convert 2018-12-25 to Dec 25, 2018
----------------------------------------------------------*/
char *ymdtostr ( char *ymd, int FormatCode )
{
	int		Year, Month, Day;
	char	*cp;
	static	char	String[14];

	sprintf ( String, "%s", ymd );

	Year = nsAtoi ( ymd );

	cp = nsStrchr ( ymd, '-' );
	if ( cp == NULL )
	{
		sprintf ( String, "format" );
		return ( String );
	}
	cp++;
	Month = nsAtoi ( cp );

	cp = nsStrchr ( cp, '-' );
	if ( cp == NULL )
	{
		sprintf ( String, "format" );
		return ( String );
	}
	cp++;
	Day = nsAtoi ( cp );

	switch ( Month )
	{
		case 1: cp = "Jan"; break;
		case 2: cp = "Feb"; break;
		case 3: cp = "Mar"; break;
		case 4: cp = "Apr"; break;
		case 5: cp = "May"; break;
		case 6: cp = "Jun"; break;
		case 7: cp = "Jul"; break;
		case 8: cp = "Aug"; break;
		case 9: cp = "Sep"; break;
		case 10: cp = "Oct"; break;
		case 11: cp = "Nov"; break;
		case 12: cp = "Dec"; break;
	}

	switch ( FormatCode )
	{
		case DATE_FORMAT_AAA_DD_YYYY:
			sprintf ( String, "%s %d, %d", cp, Day, Year );
			break;
		case DATE_FORMAT_MM_DD_YY:
			sprintf ( String, "%d/%d/%d", Month, Day, Year % 100 );
			break;
	}

	return ( String );
}
