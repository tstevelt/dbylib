/*----------------------------------------------------------
	dbylib.h
----------------------------------------------------------*/
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
#include	<mysql/mysql.h>
#include	<mysql/errmsg.h>

#ifndef DBYLIB_H
#define	DBYLIB_H

#define		DBNAME_LENGTH		20
#define		APPSTR_LENGTH		48

typedef struct
{
	char	*Host;
	char	*User;
	char	*Passwd;
	char	*Db;
	int		Port;
} DBY_OPTIONS;

typedef struct
{
    MYSQL_RES		*Result;  
    my_ulonglong	NumRows; 
    MYSQL_ROW		EachRow; 
// not used:  unsigned long *Lengths; 
    unsigned int	NumFields;
	MYSQL_FIELD		*FieldsArray;
} DBY_QUERY;

#define		MAXSTATEMENT		4096

#define DATE_FORMAT_AAA_DD_YYYY	101
#define DATE_FORMAT_MM_DD_YY	102

#define	DBY_CONN_SUCCESS		0
#define	DBY_CONN_ERR_CFG_FILE	1
#define	DBY_CONN_ERR_CFG_DB		2
#define	DBY_CONN_ERR_CFG_PARM	3
#define	DBY_CONN_ERR_STATUS		4
#define	DBY_CONN_ERR_OFFLINE	5
#define	DBY_CONN_ERR_DATE		6
#define	DBY_CONN_ERR_EXPIRED	7
#define	DBY_CONN_ERR_MYSQL		8
#define	DBY_CONN_ERR_LICENSE	9
#define	DBY_CONN_ERR_HACKED		10
#define	DBY_CONN_ERR_SYNTAX		11
#define	DBY_CONN_ERR_WRONG_DB	12

/*----------------------------------------------------------
	FIELD_RECORD used to custom maint screens.
----------------------------------------------------------*/
typedef struct
{
	int		required;
	int		keyfield;
	int		datatype;
	char	*buffer;
	int		length;
	char	*name;
	char	*label;
} FIELD_RECORD;

typedef struct
{
	char	*child_table;
	char	*child_field;
	char	*english;
} FK_RECORD;

typedef struct
{
	char	Database[DBNAME_LENGTH+1];
	DATEVAL	ExpirationDate;
	int		DaysToExpiration;
	char	AppString[APPSTR_LENGTH+1];
} LICENSE_RECORD;

/*----------------------------
:.,$-1d
:$-1
:r ! mkproto -p *.c
----------------------------*/

/* dbyCommitFuncs.c */
void dbyCommitInit ( MYSQL *MySql );
void dbyCommitCheck ( MYSQL *MySql );
void dbyCommitClose ( MYSQL *MySql );

/* dbyClose.c */
void dbyClose ( MYSQL *MySql );

/* dbyConnect.c */
char *dbyConnectErrorStr ( int ErrorCode );
int dbyConnect ( MYSQL *MySql , char *DbName , char *UserName , int CheckStatus , FILE *fpError );

/* dbyDelete.c */
long dbyDelete ( char *Module , MYSQL *MySql , char *Statement , int ErrorHandling , char *LogFileName );

/* dbyFreeQuery.c */
void dbyFreeQuery ( DBY_QUERY *Query );

/* dbyFuncs.c */
char *hms24tostr ( char *hms24 );
char *ymdtostr ( char *ymd , int FormatCode );

/* dbyInit.c */
void dbyInit ( MYSQL *MySql , char *Host , char *User , char *Passwd , char *Db , int Port , char *LogFileName );
int dbyInitNoExit ( MYSQL *MySql , char *Host , char *User , char *Passwd , char *Db , int Port , char *LogFileName );

/* dbyInsert.c */
long dbyInsert ( char *Module , MYSQL *MySql , char *Statement , int ErrorHandling , char *LogFileName );

/* dbySelectAvg.c */
int dbySelectAvg ( MYSQL *MySql , char *Table , char *Field , char *WhereClause , char *LogFileName , double *Average );

/* dbySelectDate.c */
char *dbySelectDate ( MYSQL *MySql , char *Statement , char *LogFileName );

/* dbySelect.c */
DBY_QUERY *dbySelect ( char *Module , MYSQL *MySql , char *SelectStatement , char *LogFileName );

/* dbySelectCB.c */
long dbySelectCB ( char *Module , MYSQL *MySql , char *SelectStatement , int (*CallBack )(), char *LogFileName );

/* dbySelectCount.c */
long dbySelectCount ( MYSQL *MySql , char *Table , char *WhereClause , char *LogFileName );

/* dbySelectMinMaxAvg.c */
int dbySelectMinMaxAvg ( MYSQL *MySql , char *Table , char *Field , char *WhereClause , char *LogFileName , double *Minimum , double *Maximum , double *Average );

/* dbySelectSumDouble.c */
double dbySelectSumDouble ( MYSQL *MySql , char *Table , char *Field , char *WhereClause , char *LogFileName );

/* dbySelectSumLong.c */
long dbySelectSumLong ( MYSQL *MySql , char *Table , char *Field , char *WhereClause , char *LogFileName );

/* dbyUpdate.c */
long dbyUpdate ( char *Module , MYSQL *MySql , char *Statement , int ErrorHandling , char *LogFileName );

/* dbyWildCards.c */
int dbyWildCards ( char Buffer []);
#endif
