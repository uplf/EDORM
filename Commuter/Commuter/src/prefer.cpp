#include "prefer.h"

Preferences prefs;


void prefInit(){
    prefs.begin("userNamespace");

}

/*
template <typename T>
T readOrCreate(const char* name, T defaultVal) {
    if constexpr (std::is_same<T, int>::value) {
        if (prefs.isKey(name)) return prefs.getInt(name);
        prefs.putInt(name, defaultVal);
    } else if constexpr (std::is_same<T, float>::value) {
        if (prefs.isKey(name)) return prefs.getFloat(name);
        prefs.putFloat(name, defaultVal);
    }else if constexpr()
    return defaultVal;
}
*/

// int 版本
template <>
int readOrCreate<int>(const char* name, int defaultVal) {
    if (prefs.isKey(name)) return prefs.getInt(name);
    prefs.putInt(name, defaultVal);
    return defaultVal;
}
// float 版本
template <>
float readOrCreate<float>(const char* name, float defaultVal) {
    if (prefs.isKey(name)) return prefs.getFloat(name);
    prefs.putFloat(name, defaultVal);
    return defaultVal;
}
// String 版本
template <>
String readOrCreate<String>(const char* name, String defaultVal) {
    if (prefs.isKey(name)) return prefs.getString(name);
    prefs.putString(name, defaultVal);
    return defaultVal;
}