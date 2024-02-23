/*----------------------------------------------------------------------------
	Program : dbySelect.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Oct 2004
	Synopsis: Given Statement, select record[s] from database
	Return  : Pointer to DBY_QUERY.

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
#include	"shslib.h"
#include	"dbylib.h"

static	char		xbuf[512];
extern	int			include_pid;
static	DBY_QUERY	*Query = NULL;

DBY_QUERY *dbySelect ( char *Module,
						MYSQL *MySql, 
						char *SelectStatement, 
						char *LogFileName )
{
	int		MyRv;

	include_pid = 0;

	if (( MyRv = mysql_query ( MySql, SelectStatement )) != 0 )
	{
		snprintf ( xbuf, sizeof(xbuf), 
			"%s: mysql_query: rv %d\n%s", Module, MyRv, mysql_error(MySql) );
		logmsg ( LogFileName, xbuf );
		return ( NULL );
	}

	if (( Query = (DBY_QUERY *) calloc ( 1, sizeof(DBY_QUERY) )) == NULL )
	{
		snprintf ( xbuf, sizeof(xbuf), "dbySelect:%s malloc failed", Module );
		logmsg ( LogFileName, xbuf );
		return ( NULL );
	}

	Query->Result = mysql_store_result ( MySql );
				
	if ( Query->Result == NULL )
	{               
		snprintf ( xbuf, sizeof(xbuf), 
			"%s: mysql_store_result\n%s", Module,  mysql_error(MySql) );
		logmsg ( LogFileName, xbuf );
		dbyFreeQuery ( Query );
		return ( NULL );
	}

	Query->NumRows = mysql_num_rows ( Query->Result );
	Query->NumFields = mysql_num_fields ( Query->Result );
	Query->FieldsArray = mysql_fetch_fields ( Query->Result );

	return ( Query );
}
