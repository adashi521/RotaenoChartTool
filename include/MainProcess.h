#pragma once
#include <iostream>
#include <string>
#include <fstream>
#define  _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include "Chart.h"
#include "InputScreen.h"
#include "external/truncated_normal.hpp"

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
	void showComplete();
	void showCompleteAndSave();
	void pauseConsole();

	// Features
	void timeShift();
	void chartRotate();
	void chartRotateLinear();
	void noteConversion();
	void getDistance();
	void realCopy();
	void noteFollowTrail();
	
	// Trail Maker
	void squareWaveMaker();
	void randomSquareWaveMaker();
	void sawtoothWaveMaker();
	void randomSawtoothWaveMaker();
    void trailSegmentation();

        // Special Cases
	void hiddenChartMaker();
	void makeCircuit();
	void RrharilSpeed();
	void ReverseChartMaker();
	void dextorySpeed();
	void dextorySpeed2();
	void randomBombGenerator();
    void rotateTrailMaker();
    void heavensCageFlick();
	void cyberneticVampireFlick();
	void aleph0Speed();
	void foodMakerSingle2025();
	void foodMaker2025();
};