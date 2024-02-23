/*----------------------------------------------------------------------------
	Program : dbySelectMinMaxAvg.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Aug 2022
	Synopsis: Given table, field and  where clause, return mininum, maximum
				and average of field
	Return  : 
	Notes	: pass SHS_DATA_UNION and datatype, instead of double ptrs.

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

int dbySelectMinMaxAvg ( MYSQL *MySql, char *Table, char *Field, char *WhereClause, char *LogFileName, double *Minimum, double *Maximum, double *Average )
{
	int			ReturnValue = 0;
	DBY_QUERY	*Query;

	*Minimum = 0.0;
	*Maximum = 0.0;
	*Average = 0.0;

	if ( WhereClause == NULL || nsStrlen ( WhereClause ) == 0 )
	{
		sprintf ( Statement, "select min(%s), max(%s), avg(%s) from %s", Field, Field, Field, Table );
	}
	else
	{
		sprintf ( Statement, "select min(%s), max(%s), avg(%s) from %s where %s", Field, Field, Field, Table, WhereClause );
	}

	Query = dbySelect ( "dbySelectMinMaxAvg", MySql, Statement, LogFileName );

	if ( Query == (DBY_QUERY *) 0 )
	{
		ReturnValue = -1L;
	}
	else if ( Query->NumRows == 0 )
	{
		dbyFreeQuery ( Query );
		Query = (DBY_QUERY *) 0;
		ReturnValue = -1L;
	}
	else 
	{
		if (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
		{
			*Minimum = nsAtof( Query->EachRow[0] );
			*Maximum = nsAtof( Query->EachRow[1] );
			*Average = nsAtof( Query->EachRow[2] );
		}
		else
		{
			ReturnValue = -1L;
		}
		dbyFreeQuery ( Query );
		Query = (DBY_QUERY *) 0;
	}
	
	return ( ReturnValue );
}
