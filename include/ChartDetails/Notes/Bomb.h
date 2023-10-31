#pragma once
#include "Note.h"

class Bomb : public Note {
public:
	Bomb(const double time, const double degree) :
		Note(NoteType::Bomb, time, degree) {}
	Bomb(const std::deque<double>& deq) :
		Note(NoteType::Bomb) {
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

