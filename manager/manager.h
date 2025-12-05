#pragma once
#include <string>

bool writeMsg(const std::string& filename, const std::string& msg);
bool readMsg(const std::string& filename, const std::string& outmsg);
void initSyncObj(int fifoSize);
void initSyncObj();
void openSyncObj();