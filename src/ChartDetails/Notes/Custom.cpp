#include "ChartDetails/Notes/Custom.h"

void Custom::setValue(const NoteAttr attr, const double value) {
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

void Custom::setVector(const std::deque<double>& deq) {
    size_t pos = 0;
    if (deq.size() == 3) {
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

std::shared_ptr<Note> Custom::clone() const {
    return std::make_shared<Custom>(*this);
}

double Custom::getValue(const NoteAttr attr) const {
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

std::deque<double> Custom::getVector() const {
    return std::deque<double>{ static_cast<double>(custom_type), time, degree};
}

std::deque<double> Custom::getRoundVector() const {
    return std::deque<double>{ static_cast<double>(custom_type), std::round(time), std::round(degree)};
}
