#ifndef __PREFER_H
#define __PREFER_H


#include <Arduino.h>

#include <Preferences.h>

extern Preferences prefs;
extern Preferences prefs_user;

void prefInit();

template <typename T>
T readOrCreate(const char* name, T defaultVal);


#endif
