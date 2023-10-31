#include "ChartDetails/Notes/Flick.h"

void Flick::setValue(const NoteAttr attr, const double value) {
    switch (attr) {
    case NoteAttr::time:
        time = value;
        break;
    case NoteAttr::degree:
        degree = value;
        break;
    default:
        showUpdateAttrError(attr);
        break;
    }
}

void Flick::setVector(const std::deque<double>& deq) {
    std::size_t pos = 0;
    if (deq.size() == 2 || deq.size() == 3) {
        if (deq.size() == 3) {
            pos++;
        }
        time = deq.at(pos + 0);
        degree = deq.at(pos + 1);
    }
    else {
        showUpdateVectorError(deq.size());
    }
}

std::shared_ptr<Note> Flick::clone() const {
    return std::make_shared<Flick>(*this);
}

double Flick::getValue(const NoteAttr attr) const {
    switch (attr) {
    case NoteAttr::time:
        return time;
        break;
    case NoteAttr::degree:
        return degree;
        break;
    default:
        showUpdateAttrError(attr);
        break;
    }
    return -1;
}

std::deque<double> Flick::getVector() const {
    return std::deque<double>{ static_cast<double>(Note::noteTypeToInt(notetype)), time, degree};
}

std::deque<double> Flick::getRoundVector() const {
    return std::deque<double>{ static_cast<double>(Note::noteTypeToInt(notetype)), std::round(time), std::round(degree)};
}
