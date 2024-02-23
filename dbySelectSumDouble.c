/*----------------------------------------------------------------------------
	Program : dbySelectSumDouble.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : 2019 - 2024
	Synopsis: Given table and where clause which selects one double field,
				return found value.
	Return  : double

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

static	char	Statement[1024];

double dbySelectSumDouble ( MYSQL *MySql, char *Table, char *Field, char *WhereClause, char *LogFileName )
{
	double		dblValue = 0.0;
	DBY_QUERY	*Query;

	if ( WhereClause == NULL || nsStrlen ( WhereClause ) == 0 )
	{
		sprintf ( Statement, "select sum(%s) from %s", Field, Table );
	}
	else
	{
		sprintf ( Statement, "select sum(%s) from %s where %s", Field, Table, WhereClause );
	}

	Query = dbySelect ( "dbySelectCount", MySql, Statement, LogFileName );

	if ( Query != NULL && Query->NumRows == 1 )
	{
		if (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
		{
			dblValue = nsAtof( Query->EachRow[0] );
		}
		dbyFreeQuery ( Query );
		Query = (DBY_QUERY *) 0;
	}
	
	return ( dblValue );
}
