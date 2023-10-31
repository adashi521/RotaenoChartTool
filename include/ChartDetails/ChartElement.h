#pragma once
#include <deque>
#include <string>
#include <memory>
#include <cmath>
#include <iostream>

class ChartElement {
public:
	ChartElement(const double time) : time(time){}
	ChartElement(): time(0) {}

	virtual void setVector(const std::deque<double>& deq) = 0;
	virtual std::deque<double> getVector() const = 0;
	virtual std::deque<double> getRoundVector() const = 0;

	double getTime() const { return time; }

protected:
	double time;
	virtual void showUpdateVectorError(std::size_t s) const = 0;
	static void pauseConsoleAndExit();
};