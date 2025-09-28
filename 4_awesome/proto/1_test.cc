#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <iostream>

int main() {
    // 给定的 JSON 字符串
    const char* jsonStr = "{\"branch_no\":\"2024\",\"fundid\":\"112233445566\"}";

    // 解析 JSON 字符串
    rapidjson::Document document;
    document.Parse(jsonStr);

    // 创建 StringBuffer 用于构建带斜杠的 JSON 字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    // 手动添加转义字符
    writer.StartObject();
    for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        writer.Key(itr->name.GetString());
        writer.String(itr->value.GetString());
    }
    writer.EndObject();

    // 打印带斜杠的 JSON 字符串
    std::cout << buffer.GetString() << std::endl;

    return 0;
}

