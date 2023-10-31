#include "ChartDetails/Notes/Note.h"

void Note::showUpdateAttrError(const NoteAttr attr) const {
	std::cerr << "Error: Note Attribute \"" << noteParaToString(attr) << "\" not found in Note Type \"" << noteTypeToString(notetype) << "\"!" << std::endl;
	pauseConsoleAndExit();
}

void Note::showUpdateVectorError(const size_t s) const {
	switch (notetype) {
	case NoteType::Tap: case NoteType::Catch: case NoteType::Flick:
		std::cerr << "Error: An issue occurred while setting " << noteTypeToString(notetype) << " vector. Expected input size of 2 or 3 but received " << s << std::endl;
		break;
	case NoteType::Rotate: case NoteType::Trail:
		std::cerr << "Error: An issue occurred while setting " << noteTypeToString(notetype) << " vector. Expected input size of 5 or 6 but received " << s << std::endl;
		break;
	case NoteType::Slide:
		std::cerr << "Error: An issue occurred while setting " << noteTypeToString(notetype) << " vector. Expected input size of 8 or 9 but received " << s << std::endl;
		break;
	}
	pauseConsoleAndExit();
}

std::string Note::noteTypeToString(const NoteType type) const {
	switch (type) {
	case NoteType::Tap:
		return "Tap";
		break;
	case NoteType::Flick:
		return "Flick";
		break;
	case NoteType::Slide:
		return "Slide";
		break;
	case NoteType::Rotate:
		return "Rotate";
		break;
	case NoteType::Catch:
		return "Catch";
		break;
	case NoteType::Bomb:
		return "Bomb";
		break;
	case NoteType::Trail:
		return "Trail";
		break;
	default:
		std::cerr << "Error: Undefined NoteType \"" << static_cast<int>(type) << "\" while using noteTypeToString!" << std::endl;
		pauseConsoleAndExit();
		break;
	}
	return "Error";
}

void Note::increment(const NoteAttr attr, const double value) {
	setValue(attr, getValue(attr) + value);
}

int Note::noteTypeToInt(const NoteType type) {
	switch (type) {
	case NoteType::Tap:
		return 0;
		break;
	case NoteType::Flick:
		return 1;
		break;
	case NoteType::Slide:
		return 2;
		break;
	case NoteType::Rotate:
		return 4;
		break;
	case NoteType::Catch:
		return 5;
		break;
	case NoteType::Bomb:
		return 6;
		break;
	case NoteType::Trail:
		return 11;
		break;
	default:
		std::cerr << "Error: Undefined NoteType \"" << static_cast<int>(type) << "\" while using noteTypeToInt!" << std::endl;
		pauseConsoleAndExit();
		break;
	}
	return -1;
}

int Note::slideTypeToInt(const SlideType type) {
	switch (type) {
	case SlideType::Tap:
		return 0;
		break;
	case SlideType::Flick:
		return 1;
		break;
	case SlideType::SmallSlide:
		return 2;
		break;
	case SlideType::BigSlide:
		return 3;
		break;
	default:
		std::cerr << "Error: Undefined SlideType \"" << static_cast<int>(type) << "\" while using slideTypeToInt!" << std::endl;
		pauseConsoleAndExit();
		break;
	}
	return -1;
}

std::string Note::noteParaToString(const NoteAttr attr) const {
	switch (attr) {
	case NoteAttr::time:
		return "time";
		break;
	case NoteAttr::degree:
		return "degree";
		break;
	case NoteAttr::slide_type:
		return "slide_type";
		break;
	case NoteAttr::end_degree:
		return "end_degree";
		break;
	case NoteAttr::snap:
		return "snap";
		break;
	case NoteAttr::amount:
		return "amount";
		break;
	case NoteAttr::prev_curv:
		return "prev_cruv";
		break;
	case NoteAttr::next_curv:
		return "next_cruv";
		break;
	case NoteAttr::delta:
		return "delta";
		break;
	default:
		std::cerr << "Error: Undefined NoteAttr \"" << static_cast<int>(attr) << "\" while using noteParaToString!" << std::endl;
		pauseConsoleAndExit();
		break;
	}
	return "Error";
}
