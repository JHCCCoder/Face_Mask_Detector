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
     * @brief a query result set that returns rows
     */
    MYSQL_RES *res;

    /**
     * @brief A type-safe representation of row data
     */
    MYSQL_ROW column;

    MysqlConn(){}
    /**
     * @brief Database connection
     * @return Return execution results. If successful, return ture, if not, return false.
     */
    bool ConnectMysql();

    /**
     * @brief Release database connection
     */
    void FreeConnect();

    /**
     * @brief Select data from database
     * @param SQL: SQL statement for query
     * @return Returned data from database.
     */
    string SelectData(const char * SQL);

    /**
     * @brief Insert data to database
     * @param SQL: SQL statement for insert
     * @return int : 0 means success; 1 means failure
     */
    int InsertData(const char * SQL);

    /**
     * @brief Update data
     * @param SQL: SQL statement for update
     * @return int : 0 means success; 1 means failure
     */
    int UpdateData(const char * SQL);

    /**
     * @brief Delete data
     * @param SQL: SQL statement for delete
     * @return int : 0 means success; 1 means failure
     */
    int DeleteData(const char * SQL);


};


#endif //MYSQL_CONNECTION_H_
