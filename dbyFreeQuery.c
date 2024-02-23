/*----------------------------------------------------------------------------
	Program : dbyFreeQuery.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Oct 2004
	Synopsis: mysql memory management
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


#include	<stdlib.h>

#include	"shslib.h"
#include	"dbylib.h"

void dbyFreeQuery ( DBY_QUERY *Query )
{
	if ( Query == NULL )
	{
		return;
	}

	if ( Query->Result != NULL )
	{
		mysql_free_result ( Query->Result );
	}

	free ( Query );

}
