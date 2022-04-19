//
// Created by LittleSheep on 18/04/2022.
//

#ifndef MYSQL_CONNECTION_H_
#define MYSQL_CONNECTION_H_

#include <mysql.h>
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

class MysqlConn {
public:
    const char * Msg;
    MYSQL mysql;
    /**
     * a query result set that returns rows
     */
    MYSQL_RES *res;

    /**
     * A type-safe representation of row data
     */
    MYSQL_ROW column;

    MysqlConn(){}
    /**
     * connect database
     * @return
     */
    bool ConnectMysql();

    /**
     * Release Resources
     */
    void FreeConnect();

    /**
     * select data
     * @param SQL SQL statement for query
     * @return Returned data
     */
    string SelectData(const char * SQL);

    /**
     * insert data
     * @param SQL  SQL statement for insert
     * @return int : 0 means success; 1 means failure
     */
    int InsertData(const char * SQL);

    /**
     * update data
     * @param SQL SQL statement for update
     * @return int : 0 means success; 1 means failure
     */
    int UpdateData(const char * SQL);

    /**
     * delete data
     * @param SQL SQL statement for delete
     * @return int : 0 means success; 1 means failure
     */
    int DeleteData(const char * SQL);


};


#endif //MYSQL_CONNECTION_H_
