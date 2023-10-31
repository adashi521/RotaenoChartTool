#pragma once
#include <iostream>
#include <deque>
#include "ChartElement.h"

enum class BPMAttr { time, bpm };

class BPM : public ChartElement {
public:
	BPM(const double time, const double bpm) : ChartElement(time), bpm(bpm){}
	BPM(const std::deque<double>& deq) { setVector(deq); }

	void setValue(const BPMAttr attr, const double value);
	void setVector(const std::deque<double>& deq) override;
	void increment(const BPMAttr attr, const double value);
	std::shared_ptr<BPM> clone() const;
	double getValue(const BPMAttr attr) const;
	std::deque<double> getVector() const override;
	std::deque<double> getRoundVector() const override;

private:
	double bpm;
	void showUpdateAttrError(const BPMAttr attr) const;
	void showUpdateVectorError(const std::size_t s) const override;
};
