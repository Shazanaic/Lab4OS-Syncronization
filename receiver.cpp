#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include "manager.h"

using namespace std;

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
}