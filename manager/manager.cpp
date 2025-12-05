#include "manager.h"
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include <iostream>

HANDLE hMutex = nullptr;
HANDLE hFull = nullptr;
HANDLE hEmpty = nullptr;
const int MAXLEN = 20;

struct FIFOHeader {
	int head = 0;
	int tail = 0;
	int capacity = 0;
};

void initSyncObj(int fifoSize) {
	hMutex = CreateMutex(NULL, FALSE, "Mutex");
	hFull = CreateSemaphore(NULL, 0, fifoSize, "MsgCount");
	hEmpty = CreateSemaphore(NULL, fifoSize, fifoSize, "EmptySize");
}

void openSyncObj() {
	hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "Mutex");
	hFull = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "MsgCount");
	hEmpty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "EmptySize");
}

bool writeMsg(const std::string& filename, const std::string& msg) {
	if (!hMutex || !hFull || !hEmpty) return false;

	uint8_t length = static_cast<uint8_t>(std::min<size_t>(msg.size(), MAXLEN));

	WaitForSingleObject(hEmpty, INFINITE);
	WaitForSingleObject(hMutex, INFINITE);

	std::fstream file(filename, std::ios::binary | std::ios::trunc | std::ios::out);
	if (!file.is_open()) {
		ReleaseMutex(hMutex);
		ReleaseSemaphore(hEmpty, 1, nullptr);
		return false;
	}

	FIFOHeader header;
	file.read(reinterpret_cast<char*>(&header), sizeof(header));
	file.seekp(sizeof(header) + header.tail * (1 + MAXLEN));
	file.write(reinterpret_cast<char*>(&length), sizeof(length));

	char buf[MAXLEN] = { 0 };
	std::copy_n(msg.c_str(), length, buf);
	file.write(buf, MAXLEN);

	header.tail = (header.tail + 1) % header.capacity;
	file.seekp(0);
	file.write(reinterpret_cast<char*>(&header), sizeof(header));

	file.close();
	ReleaseMutex(hMutex);
	ReleaseSemaphore(hFull, 1, nullptr);

	return true;
}

bool readMsg(const std::string& fileName, std::string& outMessage) {
	if (!hMutex || !hFull || !hEmpty) return false;

	WaitForSingleObject(hFull, INFINITE);
	WaitForSingleObject(hMutex, INFINITE);

	std::fstream file(fileName, std::ios::binary | std::ios::in | std::ios::out);
	if (!file.is_open()) {
		ReleaseMutex(hMutex);
		ReleaseSemaphore(hFull, 1, nullptr);
		return false;
	}

	FIFOHeader header;
	file.read(reinterpret_cast<char*>(&header), sizeof(header));

	file.seekg(sizeof(header) + header.head * (1 + MAXLEN));
	uint8_t len;
	file.read(reinterpret_cast<char*>(&len), sizeof(len));

	char buf[MAXLEN];
	file.read(buf, MAXLEN);

	outMessage.assign(buf, len);

	header.head = (header.head + 1) % header.capacity;
	file.seekp(0);
	file.write(reinterpret_cast<char*>(&header), sizeof(header));

	file.close();
	ReleaseMutex(hMutex);
	ReleaseSemaphore(hEmpty, 1, nullptr);

	return true;
}