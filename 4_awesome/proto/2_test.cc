#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>

using namespace rapidjson;

// string 转 JsonValue方法
inline rapidjson::Value& String2JsonValue(const std::string& cache, rapidjson::Value& jsonValue, 
        rapidjson::Document::AllocatorType& jsonAlloc){
    jsonValue.SetString(cache.c_str(), cache.length(), jsonAlloc);
    return jsonValue;
}

void TestJson(){
    const char* branchid = "2024";
    Document jsonDoc;
    jsonDoc.SetObject();
    Document::AllocatorType& jsonAlloc = jsonDoc.GetAllocator();
    Value jsonValue(rapidjson::kStringType);
    jsonValue = StringRef(branchid, strlen(branchid));
    jsonDoc.AddMember("branch_no", jsonValue, jsonAlloc);

    std::string Fund = "112233445566";
    jsonDoc.AddMember("fundid", String2JsonValue(Fund, jsonValue, jsonAlloc), jsonAlloc);

    StringBuffer buffer;
    Writer<StringBuffer> write(buffer);
    jsonDoc.Accept(write);

    std::cout << "request: " << buffer.GetString() << std::endl;
}

void test1(){
    Document doc;
    Document::AllocatorType& allocContent = doc.GetAllocator();
    // 添加键值对
    Value Content(rapidjson::kObjectType);
    Content.AddMember("branch_no", "3344", allocContent);
    Content.AddMember("funid", "22222", allocContent);

    // 序列化为字符串
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* sz = buffer.GetString();

    // 输出 JSON 字符串
    std::cout << "json: " << sz <<std::endl;
}


void test(){
    // 1. 把 JSON 解析至 DOM。
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    //std::string json = "{"project":"rapidjson","stars":11}";
    Document d;
    d.Parse(json);
 
    // 2. 利用 DOM 作出修改。
    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);
 
    // 3. 把 DOM 转换（stringify）成 JSON。
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
 
    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
}

int main(){

    TestJson();
    test1();

    return 0;
}

