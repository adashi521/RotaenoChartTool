// RoteanoChartTool.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。

#include <iostream>
#include <cstdlib>
#include "MainProcess.h"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "Please drag the chart file onto this program!" << std::endl;
		std::cout << "Application closing, press enter to continue..." << std::endl;
		std::cin.get();
	}
	else if (argc > 2) {
		std::cout << "WTF are u doing" << std::endl;
		std::cout << "Application closing, press enter to continue..." << std::endl;
		std::cin.get();
	}
	else {
		std::cout << "Application starting..." << std::endl;
		MainProcess mp(argc, argv);
		mp.start();
	}
	return 0;
}