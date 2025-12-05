#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include "Receiver.h"
#include "F:\Projects\Lab4OS-Sync\manager\manager.h"


int main() {
	std::string filename;
	int senders, messages;

	std::cout << "Enter the name of binfile: "; std::cin >> filename;
	std::cout << "Enter the amount of sender processes: "; std::cin >> senders;
	std::cout << "Enter the amount of messages: "; std::cin >> messages;

	std::fstream file(filename, std::ios::binary | std::ios::trunc || std::ios::out);

	struct FIFO {
		int head = 0;
		int tail = 0;
		int capacity = 0;
	};
	FIFO header;
	header.capacity = messages;
	file.write(reinterpret_cast<char*>(&header), sizeof(header));
	file.close();

	initSyncObj(messages - 1);

	for (int i = 0; i < senders; i++) {
		STARTUPINFOA si{};
		PROCESS_INFORMATION pi{};
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(si);

		std::string cmd = "Sender.exe " + filename;
		if (CreateProcessA(NULL, cmd.data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
			std::cout << "Sender(" << i + 1 << ") active.\n";
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else {
			std::cerr << "Error occured while launching Sender(" << i + 1 << ").\n";
		}
	}

	std::cout << "\nCommand list: read(1) | finish(2)\n";
	while (true) {
		std::string cmd;
		std::cout << "-> ";
		std::cin >> cmd;

		if (cmd == "finish" || cmd == "2") break;
		if (cmd == "1" || cmd == "read") {
			std::string msg;
			if (readMsg(filename, msg)) {
				std::cout << "Message: " << msg << "\n";
			}
			else {
				std::cout << "0 new messages.\n";
			}
		}
	}
	std::cout << "Receiver finishes its work.\n";
	return 0;
}