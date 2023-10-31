#pragma once
#include "Note.h"

class Catch : public Note {
public:
	Catch(const double time, const double degree) :
		Note(NoteType::Catch, time, degree) {}
	Catch(const std::deque<double>& deq) :
		Note(NoteType::Catch) {
		setVector(deq);
	}

	void setValue(const NoteAttr attr, const double value) override;
	void setVector(const std::deque<double>& deq) override;
	double getValue(const NoteAttr attr) const override;
	std::shared_ptr<Note> clone() const override;
	std::deque<double> getVector() const override;
	std::deque<double> getRoundVector() const override;

private:
	double getBool(const double d) const;
};

