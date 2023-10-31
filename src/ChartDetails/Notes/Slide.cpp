#include "ChartDetails/Notes/Slide.h"

void Slide::setValue(const NoteAttr attr, const double value) {
    switch (attr) {
    case NoteAttr::time:
        time = value;
        break;
    case NoteAttr::degree:
        degree = value;
        break;
    case NoteAttr::slide_type:
        switch (static_cast<int>(value)) {
        case 0:
            slidetype = SlideType::Tap;
            break;
        case 1:
            slidetype = SlideType::Flick;
            break;
        case 2:
            slidetype = SlideType::SmallSlide;
            break;
        case 3:
            slidetype = SlideType::BigSlide;
            break;
        default:
            break;
        }
        break;
    case NoteAttr::end_degree:
        end_degree = value;
        break;
    case NoteAttr::snap:
        snap = static_cast<int>(value);
        break;
    case NoteAttr::amount:
        amount = static_cast<int>(value);
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

void Slide::setVector(const std::deque<double>& deq) {
    size_t pos = 0;

    if (deq.size() == 8 || deq.size() == 9) {
        if (deq.size() == 9) {
            pos++;
        }
        time = deq.at(pos + 0);
        degree = deq.at(pos + 1);
        slidetype = numToSlideType(deq.at(pos + 2));
        end_degree = deq.at(pos + 3);
        snap = static_cast<int>(deq.at(pos + 4));
        amount = static_cast<int>(deq.at(pos + 5));
        prev_curv = deq.at(pos + 6);
        next_curv = deq.at(pos + 7);
    }
    else {
        showUpdateVectorError(deq.size());
    }
}

std::shared_ptr<Note> Slide::clone() const {
    return std::make_shared<Slide>(*this);
}

double Slide::getValue(const NoteAttr attr) const {
    switch (attr) {
    case NoteAttr::time:
        return time;
        break;
    case NoteAttr::degree:
        return degree;
        break;
    case NoteAttr::slide_type:
        return Note::slideTypeToInt(slidetype);
        break;
    case NoteAttr::end_degree:
        return end_degree;
        break;
    case NoteAttr::snap:
        return static_cast<double>(snap);
        break;
    case NoteAttr::amount:
        return static_cast<double>(amount);
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

std::deque<double> Slide::getVector() const {
    return std::deque<double>{ static_cast<double>(Note::noteTypeToInt(notetype)), time, degree, static_cast<double>(Note::slideTypeToInt(slidetype)), end_degree, static_cast<double>(snap), static_cast<double>(amount), prev_curv, next_curv};
}

std::deque<double> Slide::getRoundVector() const {
    return std::deque<double>{ static_cast<double>(Note::noteTypeToInt(notetype)), std::round(time), std::round(degree), static_cast<double>(Note::slideTypeToInt(slidetype)), std::round(end_degree), static_cast<double>(snap), static_cast<double>(amount), std::round(prev_curv), std::round(next_curv)};
}
