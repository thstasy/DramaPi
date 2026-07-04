#include "CommandRunner.h"
#include <array>
#include <cstdio>
#include <memory>
#include <string>

std::string runCommand(const char* command){
	std::array<char,128> buffer{};
	std::string result;

	std::unique_ptr<FILE, decltype(&pclose)>pipe(popen(command, "r"),pclose);

	if (!pipe){
		return "";
	}

	while(fgets(buffer.data(), buffer.size(),pipe.get())!= nullptr){
		result += buffer.data();
	}	

	return result;
}