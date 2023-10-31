#pragma once
#include <deque>
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <limits>
#include "ChartDetails/ChartElement.h"
#include "ChartDetails/BPM.h"
#include "ChartDetails/Speed.h"
#include "ChartDetails/SpeedDistance.h"
#include "ChartDetails/TrailDistance.h"
#include "ChartDetails/Notes/Note.h"
#include "ChartDetails/Notes/Tap.h"
#include "ChartDetails/Notes/Flick.h"
#include "ChartDetails/Notes/Slide.h"
#include "ChartDetails/Notes/Rotate.h"
#include "ChartDetails/Notes/Catch.h"
#include "ChartDetails/Notes/Trail.h"
#include "ChartDetails/Notes/Bomb.h"

class Chart {
public:
	Chart();

	void pushBackBPM(const std::deque<double> input);
	void pushBackSpeed(const std::deque<double> input);
	void pushBackNote(const std::deque<double> input);
	/* deprecated
	void pushBackBPM(const BPM& input);
	void pushBackSpeed(const Speed& input);
	void pushBackNote(const Note& input);
	*/
	void deleteBPM(const std::size_t front_pos, const std::size_t back_pos = std::numeric_limits<std::size_t>::max());
	void deleteSpeed(const std::size_t front_pos, const std::size_t back_pos = std::numeric_limits<std::size_t>::max());
	void deleteNote(const std::size_t front_pos, const std::size_t back_pos = std::numeric_limits<std::size_t>::max());
	BPM& getBPM(const std::size_t pos);
	Speed& getSpeed(const std::size_t pos);
	Note& getNote(const std::size_t pos);
	std::size_t getBPMSize() const;
	std::size_t getSpeedSize() const;
	std::size_t getNoteSize() const;
	void readFile(std::fstream& input_file);
	std::size_t findBPMPosByTime(const double time, const int mode = 0) const;	// return pos
	std::size_t findSpeedPosByTime(const double time, const int mode = 0) const;	// return pos
	std::size_t findNotePosByTime(const double time, const int mode = 0) const;	// return pos
	double findDistanceByTime(const double time) const; // return distance
	double findDegreeByTime(const double time) const; // return degree
	static double getYfromX(const double a, const double b, const double ca, const double cb, const double x); // From Chris
	void distancePreprocessing();
	void catchRotate(const std::size_t pos, const double degree);
	void sortAll();
	void clear();
	static bool isNote(const Note& input);
	
	std::string getChart();
	
private:
	std::deque<std::shared_ptr<BPM>> bpm;
	std::deque<std::shared_ptr<Speed>> speed;
	std::deque<std::shared_ptr<Note>> note;
	//std::deque<std::deque<double>> note;	//deprecated
	/*
	BPM:
		time/BPM
	Speed:
		time/speed/smooth
	Note:
		0(tap)/time/degree
		1(flick)/time/degree
		2(slide)/time/degree/slidetype/end_degree/snap/amount/prev_curv/next_curv
		4(rotate)/time/degree/delta/prev_curv/next_curv
		5(catch)/time/degree
		11(trail)/time/degree/delta/prev_curv/next_curv
	*/
	std::deque<std::shared_ptr<SpeedDistance>> speed_distance;
	std::deque<std::shared_ptr<TrailDistance>> trail_distance;

	void readBPM(const std::string str);
	void readSpeed(const std::string str);
	void readNote(const std::string str);
	std::deque<double> readInfo(const std::string str);

	template <typename T>
		void deleteRange(std::deque<std::shared_ptr<T>>& deq, std::size_t front_pos, std::size_t back_pos);
	template <typename T>
		void printInfo(std::stringstream& chart, const std::deque<std::shared_ptr<T>>& input);
	template <typename T>
		std::size_t findPosByTime(const std::deque<std::shared_ptr<T>>& input, const double time, const int mode = 0) const;
	template <typename T>
		std::size_t findPosByDistance(const std::deque<std::shared_ptr<T>>& input, const double pos, const int mode = 0) const;
	
	void sortBPMByTime();
	void sortSpeedByTime();
	void sortNoteByTime();
	void printNote(const std::size_t pos) const;
	static void pauseConsoleAndExit();
};

#include "Chart.tpp"