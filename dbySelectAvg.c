/*----------------------------------------------------------------------------
	Program : dbySelectAvg.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Aug 2022
	Synopsis: Given table, field and  where clause, return average of field
	Return  : 
	Notes	: pass SHS_DATA_UNION and datatype, instead of double ptrs.

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

int dbySelectAvg ( MYSQL *MySql, char *Table, char *Field, char *WhereClause, char *LogFileName, double *Average )
{
	int			ReturnValue = 0L;
	DBY_QUERY	*Query;

	*Average = 0.0;

	if ( WhereClause == NULL || nsStrlen ( WhereClause ) == 0 )
	{
		sprintf ( Statement, "select avg(%s) from %s", Field, Table );
	}
	else
	{
		sprintf ( Statement, "select avg(%s) from %s where %s", Field, Table, WhereClause );
	}

	Query = dbySelect ( "dbySelectAvg", MySql, Statement, LogFileName );

	if ( Query == (DBY_QUERY *) 0 )
	{
		ReturnValue = 0;
	}
	else if ( Query->NumRows == 0 )
	{
		dbyFreeQuery ( Query );
		Query = (DBY_QUERY *) 0;
		ReturnValue = 0;
	}
	else 
	{
		if (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
		{
			*Average = nsAtof( Query->EachRow[0] );
		}
		else
		{
			ReturnValue = 0;
		}
		dbyFreeQuery ( Query );
		Query = (DBY_QUERY *) 0;
	}
	
	return ( ReturnValue );
}
