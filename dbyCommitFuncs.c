/*----------------------------------------------------------------------------
	Program : dbyCommitFuncs.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : 2023
	Synopsis: mysql commit functions, call periodically to prevent bogging.
				void dbyCommitInit ( MYSQL *MySql )
				void dbyCommitCheck ( MYSQL *MySql )
				void dbyCommitClose ( MYSQL *MySql )
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

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"shslib.h"
#include	"dbylib.h"

#define	APP_HANDLES_COMMIT	-1
#define	COMMIT_BATCH_SIZE	100
static	int		BatchCount = APP_HANDLES_COMMIT;

void dbyCommitInit ( MYSQL *MySql )
{
	BatchCount = 0;

	if ( mysql_autocommit ( MySql, 0 ) != 0 )
	{
		fprintf ( stderr, "dbyCommitInit: Cannot turn autocommit OFF\n" );
		exit ( 1  );
	}
}

void dbyCommitCheck ( MYSQL *MySql )
{
	int		rv;

	if ( BatchCount == APP_HANDLES_COMMIT )
	{
		return;
	}

	BatchCount++;

	if ( BatchCount == COMMIT_BATCH_SIZE )
	{
		BatchCount = 0;

		if (( rv = mysql_commit ( MySql )) != 0 )
		{
			fprintf ( stderr, "dbyCommitCheck: mysql_commit failed, rv %d\n", rv );
			exit ( 1 );
		}
	}
}

void dbyCommitClose ( MYSQL *MySql )
{
	int		rv;

	if ( BatchCount == APP_HANDLES_COMMIT )
	{
		return;
	}

	if (( rv = mysql_commit ( MySql )) != 0 )
	{
		fprintf ( stderr, "dbyCommitCheck: mysql_commit failed, rv %d\n", rv );
		exit ( 1 );
	}
	if ( mysql_autocommit ( MySql, 1 ) != 0 )
	{
		fprintf ( stderr, "dbyCommitClose: Cannot turn autocommit ON\n" );
		exit ( 1  );
	}

	BatchCount = APP_HANDLES_COMMIT;

}
