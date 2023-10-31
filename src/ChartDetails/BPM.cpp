#include "ChartDetails/BPM.h"

void BPM::setValue(const BPMAttr attr, const double value) {
    switch (attr) {
    case BPMAttr::time:
        time = value;
        break;
    case BPMAttr::bpm:
        bpm = value;
        break;
    default:
        showUpdateAttrError(attr);
        break;
    }
}

void BPM::setVector(const std::deque<double>& deq) {
    if (deq.size() == 2) {
        time = deq.at(0);
        bpm = deq.at(1);
    }
    else {
        showUpdateVectorError(deq.size());
    }
}

void BPM::increment(const BPMAttr attr, const double value) {
    setValue(attr, getValue(attr) + value);
}

std::shared_ptr<BPM> BPM::clone() const {
    return std::make_shared<BPM>(*this);
}

double BPM::getValue(const BPMAttr attr) const {
    switch (attr) {
    case BPMAttr::time:
        return time;
        break;
    case BPMAttr::bpm:
        return bpm;
        break;
    default:
        showUpdateAttrError(attr);
        break;
    }
    return 0;
}

std::deque<double> BPM::getVector() const {
    return std::deque<double>{time, bpm};
}

std::deque<double> BPM::getRoundVector() const {
    return std::deque<double>{std::round(time), bpm};
}

void BPM::showUpdateAttrError(const BPMAttr attr) const {
    std::cerr << "Error: BPM Attribute \"" << static_cast<int>(attr) << "\" not found!" << std::endl;
    pauseConsoleAndExit();
}

void BPM::showUpdateVectorError(const std::size_t s) const {
    std::cerr << "Error: An issue occurred while setting BPM vector. Expected input size of 2 but received " << s << std::endl;
    pauseConsoleAndExit();
}
