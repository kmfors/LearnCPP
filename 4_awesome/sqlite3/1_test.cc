#include "sqlite3++.h"
#include <thread>

using namespace std;

void test(){
    sqlite3 *db;
    int ret = sqlite3_open("CustInfo.db", &db);
    if( ret ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    sqlite3_db_config()
    sql3::createTable(db);
    sql3::createIndex(db);
    sql3::createIndex(db);
    // sqlite3_stmt* pStmt = sql3::preIns(db);
    // sql3::CustMsg msg;
    // msg.custid = 20001;
    // msg.name = "pingan";
    // msg.sex = 1;
    // msg.text = "hello";
    // msg.flag = 0;
    // sql3::insert(db, pStmt, msg);

}


int main(){
    sqlite3 *db;
    int ret = sqlite3_open("CustInfo.db", &db);
    if( ret ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    sql3::createTable(db);
    std::thread th1(sql3::createIndex, db);
    std::thread th2(sql3::createIndex, db);

    th1.join();
    th2.join();


    //test();
    return 0;
}
