#pragma once
#include "Note.h"

class Tap : public Note {
public:
    Tap(const double time, const double degree) :
        Note(NoteType::Tap, time, degree) {}
    Tap(const std::deque<double>& deq) :
        Note(NoteType::Tap) {
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