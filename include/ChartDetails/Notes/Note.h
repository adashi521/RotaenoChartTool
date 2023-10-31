#pragma once
#include <string>
#include <iostream>
#include <deque>
#include <type_traits>
#include "../ChartElement.h"

enum class NoteType { Tap, Flick, Slide, Rotate, Catch, Bomb, Trail };
enum class SlideType { Tap, Flick, SmallSlide, BigSlide};
enum class NoteAttr { time, degree, slide_type, end_degree, snap, amount, prev_curv, next_curv, delta };

class Note : public ChartElement{
public:
    Note(const NoteType notetype, const double time, const double degree) : notetype(notetype), ChartElement(time), degree(degree) {}
	Note(const NoteType notetype) : notetype(notetype), degree(0) {}

    NoteType getType() const { return notetype; };
	int getIntType() const { return noteTypeToInt(notetype); };
    virtual void setValue(const NoteAttr attr, const double value) = 0;
	void increment(const NoteAttr attr, const double value);
    virtual double getValue(const NoteAttr attr) const = 0;
	virtual std::shared_ptr<Note> clone() const = 0;

    static int noteTypeToInt(const NoteType type);
    static int slideTypeToInt(const SlideType type);
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
        static NoteType numToNoteType(const T val);
	template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		static SlideType numToSlideType(const T val);

protected:
    double degree;
    const NoteType notetype;
    void showUpdateAttrError(const NoteAttr attr) const;
	void showUpdateVectorError(const size_t s) const override;

private:
	std::string noteTypeToString(const NoteType type) const;
    std::string noteParaToString(const NoteAttr attr) const;
};

template<typename T, typename>
inline NoteType Note::numToNoteType(const T val) {
	int type = static_cast<int>(val);
	switch (type) {
	case 0:
		return NoteType::Tap;
		break;
	case 1:
		return NoteType::Flick;
		break;
	case 2:
		return NoteType::Slide;
		break;
	case 4:
		return NoteType::Rotate;
		break;
	case 5:
		return NoteType::Catch;
		break;
	case 11:
		return NoteType::Trail;
		break;
	default:
		std::cerr << "Error: Integer \"" << type << "\" is not a vaild type in NoteType" << std::endl;
		pauseConsoleAndExit();
		break;
	}
	return NoteType();
}

template<typename T, typename>
inline SlideType Note::numToSlideType(const T val) {
	int type = static_cast<int>(val);
	switch (type) {
	case 0:
		return SlideType::Tap;
		break;
	case 1:
		return SlideType::Flick;
		break;
	case 2:
		return SlideType::SmallSlide;
		break;
	case 3:
		return SlideType::BigSlide;
		break;
	default:
		std::cerr << "Error: Integer \"" << type << "\" is not a vaild type in SlideType" << std::endl;
		pauseConsoleAndExit();
		break;
	}
	return SlideType();
}
