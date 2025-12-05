#include "F:\Projects\Lab4OS-Sync\manager\manager.h"
#include "Sender.h"
#include <string>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Error: filename required.";
	}

	std::string filename = argv[1];
	
	openSyncObj();
	std::cout << "Sender ios active, list of commands: send(1) | finish(2)\n";

	while (true) {
		std::string cmd;
		std::cout << "-> ";
		std::cin >> cmd;
		if (cmd == "2" || cmd == "finish") {
			break;
		}
		if (cmd == "1" || cmd == "send") {
			std::cin.ignore();
			std::string msg;
			std::cout << "Enter ur message: ";
			std::getline(std::cin, msg);

			if (msg.empty()) {
				std::cout << "Message is empty!";
				continue;
			}
			if (!writeMsg(filename, msg)) {
				std::cerr << "Failed write ur message!";
				continue;
			}
			std::cout << "Message is successfully written!\n";
		}
	}
	std::cout << "\nSender successfully finished its work!";
	return 0;
}
