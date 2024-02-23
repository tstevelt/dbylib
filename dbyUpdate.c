/*----------------------------------------------------------------------------
	Program : dbyUpdate.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Oct 2004
	Synopsis: Given Statement, update record[s] in database
	Return  : Number of records updated
	Note    : Can not be used for binary data.

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

static	int		DebugdbyUpdate = 0;
static	char	xbuf[512];
extern	int		include_pid;

long dbyUpdate ( char *Module, MYSQL *MySql, char *Statement, int ErrorHandling, char *LogFileName )
{
	int		MyRv;
	long	Affected = 0L;

	include_pid = 0;

	/*----------------------------------------------------------
		mysql docs: can not be used for binary data -- use
		mysql_real_query instead.
		http://www.mysql.com/doc/en/mysql_query.html
	----------------------------------------------------------*/
	if ( DebugdbyUpdate )
	{
		printf ( "%s\n", Statement );
	}
	if (( MyRv = mysql_query ( MySql, Statement )) != 0 )
	{
		snprintf ( xbuf, sizeof(xbuf), "%s, rv %d", Module, MyRv  );
		logmsg ( LogFileName, xbuf );

		snprintf ( xbuf, sizeof(xbuf), "%s", mysql_error ( MySql ) );
		logmsg ( LogFileName, xbuf );
		
		if ( ErrorHandling == 0 )
		{
			return ( Affected );
		}

		exit ( 1 );
	}

	Affected = (long) mysql_affected_rows ( MySql );

	dbyCommitCheck ( MySql );

	return  ( Affected );

}
