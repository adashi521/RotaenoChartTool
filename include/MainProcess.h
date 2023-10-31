#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <random>
#include "Chart.h"
#include "InputScreen.h"

class MainProcess {
public:
	MainProcess(int argc, char* argv[]);
	int start();


private:
	Chart chart;
	std::string filePath, title;
	std::fstream input_file, output_file;


	void writeFile();
	void menu();

	// Features
	void hiddenChartMaker();
	void timeShift();
	void chartRotate();
	void chartRotateLinear();
	void noteConversion();
	void getDistance();
	void realCopy();
	void makeCircuit();
	void randomRightAngleMaker();
	void noteFollowTrail();
	void dextorySpeed();
	void sawtoothWaveMaker();
	void RrharilSpeed();

	void showComplete();
	void showCompleteAndSave();
	void pauseConsole();
};