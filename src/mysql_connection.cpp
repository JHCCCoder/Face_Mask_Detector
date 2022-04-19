//
// Created by LittleSheep on 18/04/2022.
//

#include "mysql_connection.h"

bool MysqlConn::ConnectMysql() {
    mysql_init(&mysql);//init mysql
    if (!(mysql_real_connect(&mysql,
                             "localhost",//hostname
                             "root",//username
                             "123456",//password
                             "mask_detection",//dabasename
                             3306,//port number default 0 or 3306
                             NULL, 0
    ))) {
        cout << "Error connecting to database:" + (string) mysql_error(&mysql) << endl;
        return false;
    } else {
        cout << ">>  Connected Successful..." << endl;
        return true;
    }
}

void MysqlConn::FreeConnect() {
    mysql_free_result(res);
    mysql_close(&mysql);
    cout << ">>  Free Successful" << endl;
}

string MysqlConn::SelectData( const char * SQL)
{
    MYSQL_ROW m_row;
    MYSQL_RES *m_res;
    char sql[2048];
    sprintf(sql,"%s",SQL);
    int rnum = 0;
    char rg = 0x06;//行隔开
    char cg = {0x05};//字段隔开

    if(mysql_query(&mysql,sql) != 0){
        Msg = "select ps_info Error";
        return "";
    }
    m_res = mysql_store_result(&mysql);

    if(m_res==NULL){
        Msg = "select result Error";
        return "";
    }
    string str("");
    int Cnum = mysql_num_fields(m_res);

    if(m_res){
        while((m_row = mysql_fetch_row(m_res))){
            for(int i = 0;i < Cnum;i++){
                str += m_row[i];
                str += rg;
            }
            str += rg;
            rnum++;
        }
    }

    mysql_free_result(m_res);
    return str;
}

int MysqlConn::InsertData(const char *SQL){
    char sql[2048];
    sprintf(sql,"%s",SQL);
    if(mysql_query(&mysql,sql) != 0){
        Msg = "Insert Data Error";
        return 1;
    }
    return 0;
}

int MysqlConn::UpdateData(const char * SQL){
    char sql[2048];
    sprintf(sql,"%s",SQL);
    if(mysql_query(&mysql,sql) != 0){
        Msg = "Update Data Error";
        return 1;
    }
    return 0;
}

int MysqlConn::DeleteData(const char * SQL)
{
    char sql[2048];
    sprintf(sql,"%s",SQL);
    if(mysql_query(&mysql,sql) != 0)
    {
        Msg = "Delete Data error";
        return 1;
    }
    return 0;
}

// test code
int main() {
    MysqlConn * conn = new MysqlConn();
    // connect Mysql
    conn->ConnectMysql();

    /*
     * select
     */
    const char *SQL ="";
//    SQL = "SELECT * FROM user";
//    string str = conn->SelectData(SQL);
//
//    if( str.length() > 0 ){
//        printf("--------select successful----------- \n");
//        printf("%s",str.data());
//        printf("\n");
//    }else{
//        printf("%s",Msg);
//    }

    /*
     * insert
     */
//    SQL = "insert into user(username,age) values('aa','30')";
//    if(conn->InsertData(SQL) == 0)
//        printf("insert successful \n");


    /*
     * update
     */
//    SQL = "update user set username = 'bbb',age='23' where uid = 3 ";
//    if(conn->UpdateData(SQL) == 0)
//        printf("update successful \n");

//      SQL = "delete from user where uid = 3 ";
//      if(conn->DeleteData(SQL) == 0)
//          printf("delete successful \n");

    conn->FreeConnect();
}
