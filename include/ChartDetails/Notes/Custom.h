#pragma once
#include "Note.h"

class Custom : public Note {
public:
    Custom(const int type, const double time, const double degree) : Note(NoteType::Custom, time, degree), custom_type(type) {}
    Custom(const std::deque<double> &deq) : Note(NoteType::Custom), custom_type(deq.at(0)) {
        setVector(deq);
    }

    const int custom_type;

    int getIntType() const override { return custom_type; };
    void setValue(const NoteAttr attr, const double value) override;
    void setVector(const std::deque<double>& deq) override;
    double getValue(const NoteAttr attr) const override;
    std::shared_ptr<Note> clone() const override;
    std::deque<double> getVector() const override;
    std::deque<double> getRoundVector() const override;

private:
};