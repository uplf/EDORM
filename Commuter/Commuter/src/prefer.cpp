#include "prefer.h"

Preferences prefs;
Preferences prefs_user;


void prefInit(){
    prefs.begin("setting");
    prefs_user.begin("user");
    
    if(!prefs_user.isKey("ADMIN")){
        prefs_user.clear();
        prefs_user.putString("ADMIN","admin51wiuplf");
        prefs_user.putInt("admin51wiuplf",2);
        prefs_user.putInt("LOVEWYX",1);
        prefs_user.putInt("esp32wroomkituser",3);

        
    }
    if(!prefs.isKey("FLAG")){
        prefs.clear();
        prefs.putInt("FLAG",1);
        prefs.putInt("FBI",1);
        prefs.putInt("light",1);
        prefs.putInt("AlertOff",1);
        prefs.putInt("debug",1);
        prefs.putInt("request",1);
        prefs.putInt("EtherOn",1);
        prefs.putInt("DeviceRelink",1);
        prefs.putInt("FORCEStop",1);
        prefs.putInt("permission",2);
        prefs.putInt("MusicPlay",1);
        prefs.putString("theme","to be tested");
        prefs.putString("musicUrl","local");
    }
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
