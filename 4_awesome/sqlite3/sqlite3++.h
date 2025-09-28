#include <iostream>
#include <string>
#include "sqlite3.h"

namespace sql3{

class CustMsg {
public:
    int custid;
    int sex;
    int flag;
    std::string name;
    std::string text;

};

 
inline void createTable(sqlite3* db) {
    char* zErrMsg = nullptr;
    std::string sql = 
        "create table if not exists CustInfo ("
        "Custid     INT             NOT NULL,"
        "Name       varchar(10)     NOT NULL,"
        "Sex        INT             NOT NULL,"
        "Text       varchar(30)     ,"
        "Flag       INT             ,"
        "PRIMARY KEY (custid));";
    int ret = sqlite3_exec(db, sql.c_str(), NULL, NULL, &zErrMsg);
    std::cout << "ret = " << ret << ":" << __FUNCTION__ << std::endl;

    if (ret != SQLITE_OK) {
        sqlite3_free(zErrMsg);
    }
}

//---------------------------------------------- 语句执行预处理 --------------------------------------------

inline sqlite3_stmt* preIns(sqlite3* db) {
    std::string sql("insert into CustInfo values (?, ?, ?, ?, ?);");
    sqlite3_stmt* pStmt;
    int ret = sqlite3_prepare_v2(db, sql.c_str(), -1, &pStmt, NULL);
    std::cout << "ret = " << ret << ":" << __FUNCTION__ << std::endl;
    if (ret != SQLITE_OK) {
        std::cout << "ret = " << ret << ":" << std::string(sqlite3_errmsg(db)) << std::endl;
        return nullptr;
    }
    return pStmt;
}

//---------------------------------------------- 数据占位处理 --------------------------------------------

inline void insert(sqlite3* db, sqlite3_stmt* pStmt, CustMsg& msg) {
    sqlite3_bind_int(pStmt, 1, msg.custid);
    sqlite3_bind_text(pStmt, 2, msg.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(pStmt, 3, msg.sex);
    sqlite3_bind_text(pStmt, 4, msg.text.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(pStmt, 5, msg.flag);

    int ret = sqlite3_step(pStmt);
    std::cout << "ret = " << ret << ":" << __FUNCTION__ << std::endl;

    ret = sqlite3_reset(pStmt);
    std::cout << "ret = " << ret << ":" << __FUNCTION__ << std::endl;
}

inline void createIndex(sqlite3* db){
    char* zErrMsg;
    sqlite3_stmt* pStmt;
    std::string sql(
        "select name from sqlite_master where "
        "type = 'index' and name = 'custIndex';"
    );
    int ret = sqlite3_prepare_v2(db, sql.c_str(), -1, &pStmt, NULL);
    if (ret != SQLITE_OK){
        std::cerr << "create index prepare failed: " 
        << std::string(sqlite3_errmsg(db)) << std::endl;
        return;
    }
    int step_ret = sqlite3_step(pStmt);
    ret = sqlite3_finalize(pStmt);
    if(step_ret != SQLITE_DONE) {
        std::cerr << "step failed: ret:" << step_ret << std::endl;
    } else {
        std::cout << "step success, ret:" << step_ret << std::endl;
    }
}


} // namespace end!
