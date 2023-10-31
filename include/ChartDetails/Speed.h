#pragma once
#include <iostream>
#include "ChartElement.h"

enum class SpeedAttr { time, speed, smooth };

class Speed : public ChartElement {
public:
	Speed(const double time, const double speed, const double smooth) :
		ChartElement(time), speed(speed), smooth(smooth) {}
	Speed(const std::deque<double>& deq) { setVector(deq); }

	void setValue(const SpeedAttr attr, const double value);
	void setVector(const std::deque<double>& deq) override;
	void increment(const SpeedAttr attr, const double value);
	std::shared_ptr<Speed> clone() const;
	double getValue(const SpeedAttr attr) const;
	std::deque<double> getVector() const override;
	std::deque<double> getRoundVector() const override;

private:
	double speed;
	bool smooth;
	void showUpdateAttrError(const SpeedAttr attr) const;
	void showUpdateVectorError(const size_t s) const override;
	double getBool(const double d) const;
};
