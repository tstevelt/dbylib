/*----------------------------------------------------------------------------
	Program : dbySelectCB.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : 2019 - 2024
	Synopsis: Select records with a callback function for each record found.
				CallBack() returns 0 to continue or non-zero to break.
	Return  : Number of rows selected, regardless of early CallBack break.

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

static char xbuf[512];

extern	int		include_pid;

long dbySelectCB ( char *Module,
						MYSQL *MySql, 
						char *SelectStatement, 
						int (*CallBack)(),
						char *LogFileName )
{
	int		MyRv;
	DBY_QUERY		*Query;

	include_pid = 0;

	if (( MyRv = mysql_query ( MySql, SelectStatement )) != 0 )
	{
		snprintf ( xbuf, sizeof(xbuf), 
			"%s: mysql_query: rv %d\n%s", Module, MyRv, mysql_error(MySql) );
		logmsg ( LogFileName, xbuf );
		return ( 0 );
	}

	/*----------------------------------------------------------
		calloc()  allocates  memory  for  an array of nmemb 
		elements of size bytes each and returns a pointer to the 
		allocated memory.  The memory is set to zero.

		malloc() allocates size bytes and returns a pointer to
		the allocated  memory.   The memory is not cleared.
	----------------------------------------------------------*/
	if (( Query = (DBY_QUERY *) calloc ( 1, sizeof(DBY_QUERY) )) == NULL )
	{
		snprintf ( xbuf, sizeof(xbuf), "dbySelect:%s malloc failed", Module );
		logmsg ( LogFileName, xbuf );
		return ( 0 );
	}

	Query->Result = mysql_store_result ( MySql );
				
	if ( Query->Result == NULL )
	{               
		snprintf ( xbuf, sizeof(xbuf), 
			"%s: mysql_store_result\n%s", Module,  mysql_error(MySql) );
		logmsg ( LogFileName, xbuf );
		dbyFreeQuery ( Query );
		return ( 0 );
	}

	Query->NumRows = mysql_num_rows ( Query->Result );
	Query->NumFields = mysql_num_fields ( Query->Result );
	Query->FieldsArray = mysql_fetch_fields ( Query->Result );

	if ( Query->NumRows > 0 )
	{
		while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
		{
			if ( CallBack )
			{
				if ( CallBack ( Query ) != 0 )
				{
					break;
				}
			}
		}
	}

	return ( Query->NumRows );
}
