#pragma once
#include "Note.h"

class Slide : public Note {
public:
    SlideType slidetype;
    Slide(const double time, const double degree, const SlideType slidetype, const double end_degree, const double snap, const double amount, const double prev_curv, const double next_curv) :
        Note(NoteType::Slide, time, degree), slidetype(slidetype), end_degree(end_degree), snap(static_cast<int>(snap)), amount(static_cast<int>(amount)), prev_curv(prev_curv), next_curv(next_curv) {}
    Slide(const std::deque<double>& deq) :
        Note(NoteType::Slide) {
        setVector(deq);
    }

    void setValue(const NoteAttr attr, const double value) override;
    void setVector(const std::deque<double>& deq) override;
    double getValue(const NoteAttr attr) const override;
    std::shared_ptr<Note> clone() const override;
    std::deque<double> getVector() const override;
    std::deque<double> getRoundVector() const override;

private:
    int snap, amount;
    double end_degree, prev_curv, next_curv;
};

