/*----------------------------------------------------------------------------
	Program : dbyInit.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Oct 2004
	Synopsis: Initialize mysql connection and connect to database
	Return  : dbyInit() calls exit() on failure, 
			  dbyInitNoExit() returns error codes on failure.

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

#include		<stdio.h>
#include		<stdlib.h>
#include        "shslib.h"
#include        "dbylib.h"

static	char	xbuf[512];
extern	int		include_pid;

void dbyInit ( MYSQL *MySql,
				char *Host, 
				char *User, 
				char *Passwd, 
				char *Db, 
				int Port, 
				char *LogFileName )
{
	include_pid = 0;

	if ( mysql_init ( MySql ) == (MYSQL *)0 )
	{
		snprintf ( xbuf, sizeof(xbuf), "Error: mysql_init() failed" );
		logmsg ( LogFileName, xbuf );
		snprintf ( xbuf, sizeof(xbuf), "dbyInit: %s", mysql_error(MySql) );
		logmsg ( LogFileName, xbuf );
		snprintf ( xbuf, sizeof(xbuf), "Host %s User %s Passwd %s Db %s", Host, User, Passwd, Db );
		logmsg ( LogFileName, xbuf );
		exit ( 1 );
	}

	if ( mysql_real_connect ( MySql, Host, User, Passwd, Db, Port, NULL, 0 ) == NULL )
	{
		snprintf ( xbuf, sizeof(xbuf), "Error: mysql_real_connect() failed" );
		logmsg ( LogFileName, xbuf );
		snprintf ( xbuf, sizeof(xbuf), "dbyInit: %s", mysql_error(MySql) );
		logmsg ( LogFileName, xbuf );
		snprintf ( xbuf, sizeof(xbuf), "Host %s User %s Passwd %s Db %s", Host, User, Passwd, Db );
		logmsg ( LogFileName, xbuf );
		exit ( 1 );
	}
}

int dbyInitNoExit ( MYSQL *MySql,
				char *Host, 
				char *User, 
				char *Passwd, 
				char *Db, 
				int Port, 
				char *LogFileName )
{
	include_pid = 0;

	if ( mysql_init ( MySql ) == (MYSQL *)0 )
	{
		snprintf ( xbuf, sizeof(xbuf), "Error: mysql_init() failed" );
		logmsg ( LogFileName, xbuf );
		snprintf ( xbuf, sizeof(xbuf), "dbyInitNoExit: %s", mysql_error(MySql) );
		logmsg ( LogFileName, xbuf );
		snprintf ( xbuf, sizeof(xbuf), "Host %s User %s Passwd %s Db %s", Host, User, Passwd, Db );
		logmsg ( LogFileName, xbuf );
		return ( -1 );
	}

	if ( mysql_real_connect ( MySql, Host, User, Passwd, Db, Port, NULL, 0 ) == NULL )
	{
		snprintf ( xbuf, sizeof(xbuf), "Error: mysql_real_connect() failed" );
		logmsg ( LogFileName, xbuf );
		snprintf ( xbuf, sizeof(xbuf), "dbyInitNoExit: %s", mysql_error(MySql) );
		logmsg ( LogFileName, xbuf );
		snprintf ( xbuf, sizeof(xbuf), "Host %s User %s Passwd %s Db %s", Host, User, Passwd, Db );
		logmsg ( LogFileName, xbuf );
		return ( -2 );
	}

	return ( 0 );
}
