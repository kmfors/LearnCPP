#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

inline std::string pubsub_key_prefix(const std::string &key = "") {
    static std::string KEY_PREFIX = "pss";
    if (!key.empty()) {
        KEY_PREFIX = key;
    }

    return KEY_PREFIX;
}

inline std::string pubsub_key(const std::string &k) {
    // Key prefix with hash tag,
    // so that we can call multiple-key commands on RedisCluster.
    return "{" + pubsub_key_prefix() + "}::" + k;
}

#endif