/*----------------------------------------------------------------------------
	Program : dbySelectCount.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : 2019 - 2024
	Synopsis: Given table and where clause, return count of matching records.
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		02/24/2024	Return ZERO instead of -1 on errors.

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

static	char	Statement[1024];

long dbySelectCount ( MYSQL *MySql, char *Table, char *WhereClause, char *LogFileName )
{
	long	RecordCount = 0L;
	DBY_QUERY		*Query;

	if ( WhereClause == NULL || nsStrlen ( WhereClause ) == 0 )
	{
		sprintf ( Statement, "select count(*) from %s", Table );
	}
	else
	{
		sprintf ( Statement, "select count(*) from %s where %s", Table, WhereClause );
	}

	Query = dbySelect ( "dbySelectCount", MySql, Statement, LogFileName );

	RecordCount = 0;
	if ( Query == (DBY_QUERY *) 0 )
	{
		RecordCount = 0;
	}
	else if ( Query->NumRows == 0 )
	{
		dbyFreeQuery ( Query );
		Query = (DBY_QUERY *) 0;
		RecordCount = 0;
	}
	else 
	{
		if (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
		{
			RecordCount = nsAtol( Query->EachRow[0] );
		}
		else
		{
			RecordCount = 0;
		}
		dbyFreeQuery ( Query );
		Query = (DBY_QUERY *) 0;
	}
	
	return ( RecordCount );
}
