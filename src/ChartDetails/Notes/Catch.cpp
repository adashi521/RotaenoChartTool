#include "ChartDetails/Notes/Catch.h"

void Catch::setValue(const NoteAttr attr, const double value) {
    switch (attr) {
    case NoteAttr::time:
        time = value;
        break;
    case NoteAttr::degree:
        degree = getBool(value);
        break;
    default:
        showUpdateAttrError(attr);
        break;
    }
}

void Catch::setVector(const std::deque<double>& deq) {
    std::size_t pos = 0;
    if (deq.size() == 2 || deq.size() == 3) {
        if (deq.size() == 3) {
            pos++;
        }
        time = deq.at(pos + 0);
        degree = getBool(deq.at(pos + 1));
    }
    else {
        showUpdateVectorError(deq.size());
    }
}

double Catch::getValue(const NoteAttr attr) const {
    switch (attr) {
    case NoteAttr::time:
        return time;
        break;
    case NoteAttr::degree:
        return getBool(degree);
        break;
    default:
        showUpdateAttrError(attr);
        break;
    }
    return -1;
}

std::shared_ptr<Note> Catch::clone() const {
    return std::make_shared<Catch>(*this);
}

std::deque<double> Catch::getVector() const {
    return std::deque<double>{ static_cast<double>(Note::noteTypeToInt(notetype)), time, getBool(degree)};
}

std::deque<double> Catch::getRoundVector() const {
    return std::deque<double>{ static_cast<double>(Note::noteTypeToInt(notetype)), std::round(time), getBool(degree)};
}

double Catch::getBool(const double d) const {
    return static_cast<double>(static_cast<bool>(d));
}
