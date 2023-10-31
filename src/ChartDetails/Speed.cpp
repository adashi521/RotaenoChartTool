#include "ChartDetails/Speed.h"

void Speed::setValue(const SpeedAttr attr, const double value) {
    switch (attr) {
    case SpeedAttr::time:
        time = value;
        break;
    case SpeedAttr::speed:
        speed = value;
        break;
    case SpeedAttr::smooth:
        smooth = getBool(value);
        break;
    default:
        showUpdateAttrError(attr);
        break;
    }
}

void Speed::setVector(const std::deque<double>& deq) {
    if (deq.size() == 3) {
        time = deq.at(0);
        speed = deq.at(1);
        smooth = getBool(deq.at(2));
    }
    else {
        showUpdateVectorError(deq.size());
    }
}

void Speed::increment(const SpeedAttr attr, const double value) {
    setValue(attr, getValue(attr) + value);
}

std::shared_ptr<Speed> Speed::clone() const {
    return std::make_shared<Speed>(*this);
}

double Speed::getValue(const SpeedAttr attr) const {
    switch (attr) {
    case SpeedAttr::time:
        return time;
        break;
    case SpeedAttr::speed:
        return speed;
        break;
    case SpeedAttr::smooth:
        return getBool(smooth);
        break;
    default:
        showUpdateAttrError(attr);
        break;
    }
    return 0;
}

std::deque<double> Speed::getVector() const {
    return std::deque<double>{time, speed, static_cast<double>(smooth)};
}

std::deque<double> Speed::getRoundVector() const {
    return std::deque<double>{std::round(time), speed, getBool(smooth)};
}

void Speed::showUpdateAttrError(const SpeedAttr attr) const {
    std::cerr << "Error: Speed Attribute \"" << static_cast<int>(attr) << "\" not found!" << std::endl;
    pauseConsoleAndExit();
}

void Speed::showUpdateVectorError(const size_t s) const {
    std::cerr << "Error: An issue occurred while setting Speed vector. Expected input size of 3 but received " << s << std::endl;
    pauseConsoleAndExit();
}

double Speed::getBool(const double d) const {
    return static_cast<double>(static_cast<bool>(d));
}