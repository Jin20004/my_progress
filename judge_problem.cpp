#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>

namespace fs = std::filesystem;
namespace ch = std::chrono;

int main(int argc, char* argv[]) {//1 -> is it python? 2->codename (ex:template-contest_A) 3-> username (ex:admin) 4->limited time

	fs::current_path("C:/apache/httpd-2.4.39-win64-VS16/Apache24/htdocs/common");
	//This is for the debug(please erase when releasing).

	{
		std::string tmp = "../users/";
		tmp += argv[3];
		fs::current_path(tmp);
	}
	//To make execute file in each user directory.

	std::string path = std::string(argv[2]);
	std::string conName, probName;
	for (int i = 0; i < path.size(); i++) {
		if (path[i] == '_') {
			conName = path.substr(0, i);
			probName = path.substr(i + 1, 1);
			break;
		}
	}
	//get contest name and problem name.


	if (std::stoi(argv[1]) == 0) {//If submitted code is written in C++,
		std::string compCmd = "g++ -std=c++17 -o " + std::string(argv[2]) + " " + std::string(argv[2]) + ".cpp > nul 2>&1";
		//make compile command.
		int endCode = system(compCmd.c_str());//compile submitted code.
		if (endCode != 0) {//If compile process doesn't go well,
			std::cout << -2 << std::endl;
			return 0;
		}
	}

	ch::high_resolution_clock clock; //set a clock for judging TLE.


	std::string testcasePath = "../../contests/";
	testcasePath += conName;
	testcasePath += "/";
	testcasePath += probName;
	//make a path of testcase texts.


	int cnt = 0;//the number of the testcases.
	for (const fs::directory_entry& x : fs::directory_iterator(testcasePath + "/in")) {//search all files in input and output directory.

		std::string runCmd = "";
		if (std::stoi(argv[1]) == 0) {
			runCmd += argv[2];
			runCmd += ".exe<";
			runCmd += x.path().relative_path().string();
		}
		else {
			runCmd += "python ";
			runCmd += argv[2];
			runCmd += ".py<";
			runCmd += x.path().relative_path().string();
		}
		//make run command.

		
		std::ifstream ifs(testcasePath + "/out/testcase" + std::to_string(cnt) + ".txt");
		std::string outStr((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		//get output string.

		
		char buf[256];

		auto begin = clock.now();
		FILE* fp = _popen(runCmd.c_str(), "r");
		auto end = clock.now();
		while (fgets(buf, sizeof(buf), fp) != NULL);
		//run command and get printed string.


		int endCode = _pclose(fp);
		if (endCode != 0) {//If submitted code is RE,
			std::cout << -1 << std::endl;
			continue;
		}
		else {
			
			int limitedTime = std::stoi(argv[4]);
			int time = ch::duration_cast<ch::milliseconds>(end - begin).count();
			if (std::strcmp(buf, outStr.c_str()) == 0 && time <= limitedTime) {
				std::cout << 1 << std::endl;//AC
			}
			else if (time <= limitedTime) {
				std::cout << 0 << std::endl;//WA
			}
			else {
				std::cout << 2 << std::endl;//TLE
			}

			std::cout << time << std::endl;

		}


		cnt++;
	}



	return 0;
}