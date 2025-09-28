#ifndef _UTILTOOLS_H_
#define _UTILTOOLS_H_

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif



namespace util{

inline void split(const std::string& s, char delim, 
                    std::vector<std::string>& v) {
    size_t i = 0;
    auto pos = s.find(delim);
    if(pos == std::string::npos){
        v.push_back(s);
        return;
    }

    while(pos != std::string::npos){
        v.push_back(s.substr(i, pos - i));
        i = ++pos;
        pos = s.find(delim, pos);

        if(pos == std::string::npos){
            v.push_back(s.substr(i, s.length()));
        }
    }
}

// YYYYMMDD
inline int getDateNow() {
    struct tm tmval;
    time_t time_now = time(NULL);

#ifdef _WIN32
    errno_t err = localtime_s(&tmval, &time_now);
    if (err != 0) {
        return  err > 0 ? -err : err;
    }
#else
    errno = 0;
    struct tm* ret = localtime_r(&time_now, &tmval);
    if (ret == NULL) {
        return errno > 0 ? -errno : errno;
    }
#endif
    return (tmval.tm_year + 1900) * 10000 + (tmval.tm_mon + 1) * 100 + tmval.tm_wday;
}

// 获取当前格式化时间
inline std::string GetCurrentFormattedTime(){
    std::stringstream ss;
    auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto now_ms = now.time_since_epoch().count();
    int timems_part = now_ms % 1000;
    std::time_t t = now_ms / 1000;
    std::tm tm_result;

#ifdef _WIN32
    errno_t err = localtime_s(&tm_result, &t);
    if(err != 0){
        return "";
    }
#else
    errno = 0;
    struct tm* ret = localtime_r(&t, &tm_result);
    if(ret == NULL){
        return "";
    }
#endif
    char szTemp[64];
    snprintf(szTemp, sizeof(szTemp), "%02d:%02d:%02d.%03d", tm_result.tm_hour, tm_result.tm_min,
            tm_result.tm_sec, timems_part);
    return szTemp;   
}


// 创建指定路径的文件夹
inline int CreateDirectory(const char* path) {
    std::string path_str(path);
    size_t offset = 0;
    while(true){
        std::string::size_type pos = path_str.find_first_of("/\\", offset);
        if(pos == std::string::npos) {
            break;
        }
        std::string cur_path = path_str.substr(0, pos + 1);
#ifdef _WIN32
        if(-1 == _access(cur_path.c_str(), 0)) {
            if(!CreateDirectoryA(cur_path.c_str(), NULL))   return GetLastError();
        }
#else
        if(access(cur_path.c_str(), F_OK) < 0) {
            if(mkdir(cur_path.c_str(), S_IROTH | S_IXOTH | S_IRWXU | S_IRWXG) < 0){
                return errno;
            }
        }
#endif
        offset = pos + 1;
    }

    // last subdirectory
    std::string sub_dir_name = path_str.substr(offset);
    if(!sub_dir_name.empty()){
#ifdef _WIN32
        if(-1 == _access(path_str.c_str(), 0)){
            if(!CreateDirectoryA(path_str.c_str(), NULL)) return GetLastError();
        }
#else
        if(access(path_str.c_str(), F_OK) < 0){
            if(mkdir(path_str.c_str(), S_IROTH | S_IXOTH | S_IRWXU | S_IRWXG) < 0) {
                return errno;
            }
        }
#endif
    }
    return 0;
}


}   // namespace util end!
#endif
