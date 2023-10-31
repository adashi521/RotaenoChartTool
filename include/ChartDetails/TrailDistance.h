#pragma once
#include "Notes/Trail.h"

class TrailDistance : public Trail {
public:
	TrailDistance(const double time, const double degree, const double delta, const double prev_curv, const double next_curv, const double distance)
		: Trail(time, degree, delta, prev_curv, next_curv), distance(distance) {}
	TrailDistance(const std::deque<double>& deq) : Trail(inputDistance(deq)) {}
	double getDistance() const { return distance; }

private:
	double distance;
	std::deque<double> inputDistance(const std::deque<double>& data) {
		std::deque<double> copied = data;
		if (copied.size() == 6 || copied.size() == 7) {
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

