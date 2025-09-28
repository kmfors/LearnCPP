#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include <iostream>
 
using namespace rapidjson;
using namespace std;

// const string json = R"(
// {
//     "data":[
//         {"FundID":"331040000007111","PermissionID":"16","secuid_SH":null,"secuid_SZ":null,"enable":"1","note":null},
//         {"FundID":"331040000007112","PermissionID":"17","secuid_SH":null,"secuid_SZ":null,"enable":"1","note":null},
//         {"FundID":"331040000007113","PermissionID":"18","secuid_SH":null,"secuid_SZ":null,"enable":"1","note":null}
//     ],
//     "database":"pss",
//     "es":1724842883000,
//     "gtid":"",
//     "id":16,
//     "isDdl":false,
//     "mysqlType":{
//         "FundID":"BIGINT",
//         "PermissionID":"INT",
//         "secuid_SH":"CHAR(12)",
//         "secuid_SZ":"CHAR(12)",
//         "enable":"TINYINT",
//         "note":"TINYTEXT"},
//     "old":null,
//     "pkNames":["FundID"],
//     "sql":"",
//     "sqlType":{"FundID":-5,"PermissionID":4,"secuid_SH":1,"secuid_SZ":1,"enable":-6,"note":2005},
//     "table":"user_info",
//     "ts":1724842883316,
//     "type":"INSERT"
// })";



string str = R"(
{
        "data":[{"permissionID":"3","binary_form":"302","msg_type":"风控指标"}],
    "database":"pss","es":1723790430000,"gtid":"","id":12,"isDdl":false,
    "mysqlType":{"permissionID":"INT","binary_form":"INT","msg_type":"TINYTEXT"},
    "old":null,"pkNames":null,"sql":"",
    "sqlType":{"permissionID":4,"binary_form":4,"msg_type":2005},
    "table":"permission_info","ts":1723790431023,"type":"DELETE"
})";
string json1 = R"({
    "data":[
        {"FundID":"331040000007111","PermissionID":"16","secuid_SH":null,"secuid_SZ":null,"enable":"1","note":null},
        {"FundID":"331040000007112","PermissionID":"17","secuid_SH":null,"secuid_SZ":null,"enable":"1","note":null},
        {"FundID":"331040000007113","PermissionID":"18","secuid_SH":null,"secuid_SZ":null,"enable":"1","note":null}
    ],
    "mysqlType":{
        "FundID":"BIGINT",
        "PermissionID":"INT",
        "enable":"TINYINT",
        "note":"TINYTEXT"
        
    },
    "database":"pss",
    "es":1724842883000,
    "gtid":"",
    "id":16,
    "isDdl":false,
    "old":null,
    "pkNames":["FundID"],
    "sql":"",
    "sqlType":{"FundID":-5,"PermissionID":4,"secuid_SH":1,"secuid_SZ":1,"enable":-6,"note":2005},
    "table":"user_info",
    "ts":1724842883316,
    "type":"INSERT"

})";

string json = "{ \"type\": \"0\", \"resultId\": \"0000001\" \"}";

int jsonParse(const string& str) {
    
    rapidjson::Document document;
    document.Parse(str.c_str());

    if (document.HasParseError()) {
        std::cout << "Error parsing JSON: " << document.GetParseError() << ", "
        << GetParseError_En(document.GetParseError())  << std::endl;

        return 1;
    }
    
    std::cout << "Database: " << document["resultId"].GetString() << std::endl;
    // if(document["isDdl"].GetBool())     return 0;

    // // 访问JSON对象的属性
    // std::cout << "Database: " << document["database"].GetString() << std::endl;
    // std::cout << "Table: " << document["table"].GetString() << std::endl;
    // std::cout << "Type: " << document["type"].GetString() << std::endl;

    // string key;
    // string value;
    // if(document.HasMember("Database") && document["Database"].IsString())

    // if (document.HasMember("data") && document["data"].IsArray()) {
    //     for(const auto& item : document["data"].GetArray()){
    //         cout << "FundID: " << item["FundID"].GetString() << endl;
    //         cout << "PermissionID: " << item["PermissionID"].GetString() << endl;
    //     }
    // }
    return 0;
}

int main(){
    jsonParse(json);
    return 0;
}

 
// int main() {
//     // 1. 把 JSON 解析至 DOM。
//     const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
//     Document d;
//     d.Parse(json);
 
//     // 2. 利用 DOM 作出修改。
//     Value& s = d["stars"];
//     s.SetInt(s.GetInt() + 1);
 
//     // 3. 把 DOM 转换（stringify）成 JSON。
//     StringBuffer buffer;
//     Writer<StringBuffer> writer(buffer);
//     d.Accept(writer);
 
//     // Output {"project":"rapidjson","stars":11}
//     std::cout << buffer.GetString() << std::endl;
//     return 0;
// }
