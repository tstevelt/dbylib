
This is a library of functions to simplify mysql/mariadb programming (in C language).

dbyInit.c:	Initialize mysql connection and connect to database
```
void dbyInit ( MYSQL *MySql , char *Host , char *User , char *Passwd , char *Db , int Port , char *LogFileName );
int dbyInitNoExit ( MYSQL *MySql , char *Host , char *User , char *Passwd , char *Db , int Port , char *LogFileName );
```

dbyClose.c:	Close connection to mysql
```
void dbyClose ( MYSQL *MySql );
```

dbyConnect.c:	Connect to mysql database. Uses config file located in 
```
char *dbyConnectErrorStr ( int ErrorCode );
int dbyConnect ( MYSQL *MySql , char *DbName , char *UserName , int CheckStatus , FILE *fpError );
```

### Selection Functions
dbySelect.c:	Given Statement, select record[s] from database
```
DBY_QUERY *dbySelect ( char *Module , MYSQL *MySql , char *SelectStatement , char *LogFileName );
```

dbySelectCB.c:	Select records with a callback function for each record found.
```
long dbySelectCB ( char *Module , MYSQL *MySql , char *SelectStatement , int (*CallBack )(), char *LogFileName );
```

dbySelect.c:	Given Statement, select record[s] from database
```
DBY_QUERY *dbySelect ( char *Module , MYSQL *MySql , char *SelectStatement , char *LogFileName );
```

dbySelectCB.c:	Select records with a callback function for each record found.
```
long dbySelectCB ( char *Module , MYSQL *MySql , char *SelectStatement , int (*CallBack )(), char *LogFileName );
```

dbySelectAvg.c:	Given table, field and  where clause, return average of field
```
int dbySelectAvg ( MYSQL *MySql , char *Table , char *Field , char *WhereClause , char *LogFileName , double *Average );
```

dbySelectDate.c:	Given select statement which selects one date field, return
```
char *dbySelectDate ( MYSQL *MySql , char *Statement , char *LogFileName );
```

dbySelectCount.c:	Given table and where clause, return count of matching records.
```
long dbySelectCount ( MYSQL *MySql , char *Table , char *WhereClause , char *LogFileName );
```

dbySelectMinMaxAvg.c:	Given table, field and  where clause, return mininum, maximum
```
int dbySelectMinMaxAvg ( MYSQL *MySql , char *Table , char *Field , char *WhereClause , char *LogFileName , double *Minimum , double *Maximum , double *Average );
```

dbySelectSumDouble.c:	Given table and where clause which selects one double field,
```
double dbySelectSumDouble ( MYSQL *MySql , char *Table , char *Field , char *WhereClause , char *LogFileName );
```

dbySelectSumLong.c:	Given table and where clause which selects one long field,
```
long dbySelectSumLong ( MYSQL *MySql , char *Table , char *Field , char *WhereClause , char *LogFileName );
```

## Insert, Update, Delete Functions
dbyInsert.c:	Given Statement, insert record[s] into database.
```
long dbyInsert ( char *Module , MYSQL *MySql , char *Statement , int ErrorHandling , char *LogFileName );
```

dbyUpdate.c:	Given Statement, update record[s] in database
```
long dbyUpdate ( char *Module , MYSQL *MySql , char *Statement , int ErrorHandling , char *LogFileName );
```

dbyDelete.c:	Given Statement, delete record[s] from mysql database
```
long dbyDelete ( char *Module , MYSQL *MySql , char *Statement , int ErrorHandling , char *LogFileName );
```

dbyCommitFuncs.c:	mysql commit functions, call periodically to prevent bogging.
```
void dbyCommitInit ( MYSQL *MySql );
void dbyCommitCheck ( MYSQL *MySql );
void dbyCommitClose ( MYSQL *MySql );
```

## Miscellaneous Functions
dbyFreeQuery.c:	mysql memory management
```
void dbyFreeQuery ( DBY_QUERY *Query );
```

dbyFuncs.c:	Functions to help with mysql's way of doing things.  These do
```
char *hms24tostr ( char *hms24 );
char *ymdtostr ( char *ymd , int FormatCode );
```

dbyWildCards.c:	Given a string to search for, changed '*' to '%'
```
int dbyWildCards ( char Buffer []);
```
