#pragma once
#include "Speed.h"

class SpeedDistance : public Speed {
public:
	SpeedDistance(const double time, const double speed, const double smooth, const double distance) :
		Speed(time, speed, smooth), distance(distance) {}
	SpeedDistance(const std::deque<double>& deq) : Speed(inputDistance(deq)){}
	double getDistance() const { return distance; }

private:
	double distance;
	std::deque<double> inputDistance(const std::deque<double>& data) {
		std::deque<double> copied{data};
		if (copied.size() == 3 || copied.size() == 4) {
			distance = copied.back();
			copied.pop_back();
		}
		else {
			showUpdateVectorError(copied.size());
		}
		return copied;
	}
	void showUpdateVectorError(const size_t s) const override;
};