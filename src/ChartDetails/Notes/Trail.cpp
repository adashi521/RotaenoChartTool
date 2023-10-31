#include "ChartDetails/Notes/Trail.h"

void Trail::setValue(const NoteAttr attr, const double value) {
    switch (attr) {
    case NoteAttr::time:
        time = value;
        break;
    case NoteAttr::degree:
        degree = value;
        break;
    case NoteAttr::delta:
        delta = value;
        break;
    case NoteAttr::prev_curv:
        prev_curv = value;
        break;
    case NoteAttr::next_curv:
        next_curv = value;
        break;
    default:
        showUpdateAttrError(attr);
        break;
    }
}

void Trail::setVector(const std::deque<double>& deq) {
    size_t pos = 0;
    if (deq.size() == 5 || deq.size() == 6) {
        if (deq.size() == 6) {
            pos++;
        }
        time = deq.at(pos + 0);
        degree = deq.at(pos + 1);
        delta = deq.at(pos + 2);
        prev_curv = deq.at(pos + 3);
        next_curv = deq.at(pos + 4);
    }
    else {
        showUpdateVectorError(deq.size());
    }
}

std::shared_ptr<Note> Trail::clone() const {
    return std::make_shared<Trail>(*this);
}

double Trail::getValue(const NoteAttr attr) const {
    switch (attr) {
    case NoteAttr::time:
        return time;
        break;
    case NoteAttr::degree:
        return degree;
        break;
    case NoteAttr::delta:
        return delta;
        break;
    case NoteAttr::prev_curv:
        return prev_curv;
        break;
    case NoteAttr::next_curv:
        return next_curv;
        break;
    default:
        showUpdateAttrError(attr);
        break;
    }
    return -1;
}

std::deque<double> Trail::getVector() const {
    return std::deque<double>{ static_cast<double>(Note::noteTypeToInt(notetype)), time, degree, delta, prev_curv, next_curv};
}

std::deque<double> Trail::getRoundVector() const {
    return std::deque<double>{ static_cast<double>(Note::noteTypeToInt(notetype)), std::round(time), std::round(degree), std::round(delta), std::round(prev_curv), std::round(next_curv)};
}
