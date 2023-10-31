#pragma once
#include "Note.h"

class Rotate : public Note {
public:
    Rotate(const double time, const double degree, const double delta, const double prev_curv, const double next_curv) : 
        Note(NoteType::Rotate, time, degree), delta(delta), prev_curv(prev_curv), next_curv(next_curv) {}
    Rotate(const std::deque<double>& deq) :
        Note(NoteType::Rotate) {
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