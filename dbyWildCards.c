/*----------------------------------------------------------------------------
	Program : dbyWildCards.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : 2019
	Synopsis: Given a string to search for, changed '*' to '%'
	Return  : Number of substitutions.

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

#include	"shslib.h"
#include	"dbylib.h"

int dbyWildCards ( char Buffer[] )
{
	int		WildCardCount = 0;
	int		ndx;

	for ( ndx = 0; ndx < nsStrlen(Buffer); ndx++ )
	{
		if ( Buffer[ndx] == '%' )
		{
			WildCardCount++;
		}
		else if ( Buffer[ndx] == '*' )
		{
			Buffer[ndx] = '%';
			WildCardCount++;
		}
	}

	return ( WildCardCount );
}
