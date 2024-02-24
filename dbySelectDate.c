/*----------------------------------------------------------------------------
	Program : dbySelectDate.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : 2019 - 2024
	Synopsis: Given select statement which selects one date field, return
				the date.
	Return  : char *Date or zero length string.

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

char *dbySelectDate ( MYSQL *MySql, char *Statement, char *LogFileName )
{
static	char		strDate[12];
		DBY_QUERY	*Query;

	strDate[0] = '\0';

	Query = dbySelect ( "dbySelectDate", MySql, Statement, LogFileName );

	if ( Query != NULL && Query->NumRows == 1 )
	{
		if (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
		{
			if (  Query->EachRow[0] != NULL )
			{
				sprintf ( strDate, "%s", Query->EachRow[0] );
			}
		}
		dbyFreeQuery ( Query );
		Query = (DBY_QUERY *) 0;
	}
	
	return ( strDate );
}
