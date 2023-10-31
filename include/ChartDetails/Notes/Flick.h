#pragma once
#include "Note.h"

class Flick : public Note {
public:
    Flick(const double time, const double degree) :
        Note(NoteType::Flick, time, degree) {}
    Flick(const std::deque<double>& deq) :
        Note(NoteType::Flick) {
        setVector(deq);
    }

    void setValue(const NoteAttr attr, const double value) override;
    void setVector(const std::deque<double>& deq) override;
    double getValue(const NoteAttr attr) const override;
    std::shared_ptr<Note> clone() const override;
    std::deque<double> getVector() const override;
    std::deque<double> getRoundVector() const override;

private:
};

