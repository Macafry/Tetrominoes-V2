#pragma once
#include <fstream>
#include <string>
using std::string;

namespace DOT {
	void fromString(string content, string name) {
		string path = "C:\\Users\\ian20\\Desktop\\DOT\\input\\" + name + ".dot";
		std::ofstream file;
		file.open(path);
		file << content;
		file.close();
	}
}