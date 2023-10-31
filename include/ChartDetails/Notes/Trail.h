#pragma once
#include "Note.h"

class Trail : public Note {
public:
    Trail(const double time, const double degree, const double delta, const double prev_curv, const double next_curv) :
        Note(NoteType::Trail, time, degree), delta(delta), prev_curv(prev_curv), next_curv(next_curv) {}
    Trail(const std::deque<double>& deq) :
        Note(NoteType::Trail) {
        setVector(deq);
    }
    void setValue(const NoteAttr attr, const double value) override;
    void setVector(const std::deque<double>& deq) override;
    double getValue(const NoteAttr attr) const override;
    std::shared_ptr<Note> clone() const override;
    std::deque<double> getVector() const override;
    std::deque<double> getRoundVector() const override;

private:
    double delta, prev_curv, next_curv;
};