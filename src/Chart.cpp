#include "Chart.h"

Chart::Chart() {
	clear();
}

void Chart::pushBackBPM(const std::deque<double> input) {
	bpm.emplace_back(std::make_shared<BPM>(input));
}

void Chart::pushBackSpeed(const std::deque<double> input) {
	speed.emplace_back(std::make_shared<Speed>(input));
}

void Chart::pushBackNote(const std::deque<double> input) {
	NoteType type = Note::numToNoteType(input.at(0));
	switch (type) {
	case NoteType::Tap:
		note.emplace_back(std::make_shared<Tap>(input));
		break;
	case NoteType::Flick:
		note.emplace_back(std::make_shared<Flick>(input));
		break;
	case NoteType::Slide:
		note.emplace_back(std::make_shared<Slide>(input));
		break;
	case NoteType::Rotate:
		note.emplace_back(std::make_shared<Rotate>(input));
		break;
	case NoteType::Catch:
		note.emplace_back(std::make_shared<Catch>(input));
		break;
	case NoteType::Trail:
		note.emplace_back(std::make_shared<Trail>(input));
		break;
	default:
		break;
	}
}

/* deprecated
void Chart::pushBackBPM(const BPM& input) {
	std::shared_ptr<BPM> copied = input.clone();
	bpm.push_back(copied);
}

void Chart::pushBackSpeed(const Speed& input) {
	std::shared_ptr<Speed> copied = input.clone();
	speed.push_back(copied);
}

void Chart::pushBackNote(const Note& input) {
	std::shared_ptr<Note> copied = input.clone();
	note.push_back(copied);
}
*/

void Chart::deleteBPM(const std::size_t front_pos, const std::size_t back_pos) {
	deleteRange(bpm, front_pos, back_pos);
}

void Chart::deleteSpeed(const std::size_t front_pos, const std::size_t back_pos) {
	deleteRange(speed, front_pos, back_pos);
}

void Chart::deleteNote(const std::size_t front_pos, const std::size_t back_pos) {
	deleteRange(note, front_pos, back_pos);
}

BPM& Chart::getBPM(const std::size_t pos) {
	if (pos < bpm.size()) {
		return *(bpm[pos]);
	}
	else {
		throw std::out_of_range("BPM index out of range!");
	}
}

Speed& Chart::getSpeed(const std::size_t pos) {
	if (pos < speed.size()) {
		return *(speed[pos]);
	}
	else {
		throw std::out_of_range("Speed index out of range!");
	}
}

Note& Chart::getNote(const std::size_t pos) {
	if (pos < note.size()) {
		return *(note[pos]);
	}
	else {
		throw std::out_of_range("Note index out of range!");
	}
}

std::size_t Chart::getBPMSize() const {
	return bpm.size();
}

std::size_t Chart::getSpeedSize() const {
	return speed.size();
}

std::size_t Chart::getNoteSize() const {
	return note.size();
}

void Chart::readFile(std::fstream& input_file) {
	std::string str;
	unsigned int listen = 0;
	while (getline(input_file, str)) {
		if (str.size() == 0) {
			listen = 0;
		}

		switch (listen) {
		case 1:
			readBPM(str);
			break;
		case 2:
			readSpeed(str);
			break;
		case 3:
			readNote(str);
			break;
		}

		if (str == "# BPM") {
			listen = 1;
		}
		else if (str == "# Speed") {
			listen = 2;
		}
		else if (str == "# Note") {
			listen = 3;
		}
	}
}

std::size_t Chart::findBPMPosByTime(const double time, const int mode) const {
	return findPosByTime(bpm, time, mode);
}

std::size_t Chart::findSpeedPosByTime(const double time, const int mode) const {
	return findPosByTime(speed, time, mode);
}

std::size_t Chart::findNotePosByTime(const double time, const int mode) const {
	return findPosByTime(note, time, mode);
}

double Chart::findDistanceByTime(const double time) const {
	std::size_t p1, p2;
	double d1, t1, t2, v1, v2;
	p1 = findPosByTime(speed_distance, time, 1);
	t1 = speed_distance.at(p1)->getValue(SpeedAttr::time);
	v1 = speed_distance.at(p1)->getValue(SpeedAttr::speed);
	d1 = speed_distance.at(p1)->getDistance();
	p2 = p1 + 1;

	if (p2 < speed_distance.size() && speed_distance.at(p2)->getValue(SpeedAttr::smooth) == 1) {
		t2 = speed_distance.at(p2)->getValue(SpeedAttr::time);
		v2 = speed_distance.at(p2)->getValue(SpeedAttr::speed);
		return d1 + v1 * (time - t1) + pow(time - t1, 2) * (v2 - v1) / (t2 - t1) / 2;
	}
	else {
		return d1 + (time - t1) * v1;
	}
}

double Chart::findDegreeByTime(const double time) const {
	std::size_t p1, p2;
	double t1, t2, d1, d, d2, a1, a2, c1, c2, result;
	d = findDistanceByTime(time);
	p1 = findPosByDistance(trail_distance, d, 1);
	p2 = p1 + 1;

	t1 = trail_distance.at(p1)->getValue(NoteAttr::time);
	d1 = trail_distance.at(p1)->getDistance();
	a1 = trail_distance.at(p1)->getValue(NoteAttr::degree);
	c1 = trail_distance.at(p1)->getValue(NoteAttr::next_curv);
	if (p2 < trail_distance.size()) {
		t2 = trail_distance.at(p2)->getValue(NoteAttr::time);
		d2 = trail_distance.at(p2)->getDistance();
		a2 = trail_distance.at(p2)->getValue(NoteAttr::degree);
		c2 = trail_distance.at(p2)->getValue(NoteAttr::prev_curv);
		a1 = fmod(a1, 360);
		if (a1 < 0) {
			a1 += 360;
		}
		a2 = fmod(a2, 360);
		if (a2 < 0) {
			a2 += 360;
		}
		if (a1 >= 180) {
			if (abs(a1 - (a2 + 360)) <= 90) {
				a2 += 360;
			}
			else if (abs(a1 - (a2 + 180)) <= 90) {
				a2 += 180;
			}
			else if (abs(a1 - (a2 - 180)) <= 90) {
				a2 -= 180;
			}
		}
		else {
			if (abs(a1 - (a2 - 360)) < 90) {
				a2 -= 360;
			}
			else if (abs(a1 - (a2 - 180)) < 90) {
				a2 -= 180;
			}
			else if (abs(a1 - (a2 + 180)) < 90) {
				a2 += 180;
			}
		}
		result = getYfromX(a1, a2, c1/100, c2/100, (d - d1) / (d2 - d1));
		result = fmod(result, 180);
	}
	else {
		result = fmod(a1, 180);
	}
	if (result < 0) {
		result += 180;
	}
	return result;
}

void Chart::distancePreprocessing() {
	double time_start = 0, time_end = 0, speed_start = 0, speed_end = 0, smooth = 0, distance_start = 0;
	auto cal = [&]() {
		if (smooth == 1) {
			return (speed_start + speed_end) * (time_end - time_start) / 2;
		}
		else {
			return speed_start * (time_end - time_start);
		}
	};
	speed_distance.clear();
	
	/* time/speed/smooth/distance */
	if (speed.size() > 0) {
		for (std::size_t i = 0; i < speed.size(); i++) {
			time_end = speed.at(i)->getValue(SpeedAttr::time);
			speed_end = speed.at(i)->getValue(SpeedAttr::speed);
			smooth = speed.at(i)->getValue(SpeedAttr::smooth);
			if (i == 0) {
				time_start = 0;
				if (time_end != 0) {
					if (smooth == 1) {
						speed_start = speed_end;
						smooth = 0;
					}
					else {
						speed_start = 1;
					}
					speed_distance.emplace_back(std::make_shared<SpeedDistance>(0, speed_start, 0, 0));
				}
			}
			else {
				time_start = speed.at(i - 1)->getValue(SpeedAttr::time);
				speed_start = speed.at(i - 1)->getValue(SpeedAttr::speed);
			}
			distance_start += cal();
			speed_distance.emplace_back(std::make_shared<SpeedDistance>(time_end, speed_end, smooth, distance_start));
		}
	}
	else {
		speed_distance.emplace_back(std::make_shared<SpeedDistance>(0, 1, 0, 0));
	}
	
	trail_distance.clear();
	/*4(rotate)/time/degree/delta/prev/next*/
	/*11(trail)/time/degree/delta/prev/next*/
	/* time/distance/degree/delta/prev/next */
	for (std::size_t i = 0; i < note.size(); i++) {
		if (note.at(i)->getType() == NoteType::Trail || note.at(i)->getType() == NoteType::Rotate) {
			trail_distance.emplace_back(std::make_shared<TrailDistance>(
				note.at(i)->getValue(NoteAttr::time), note.at(i)->getValue(NoteAttr::degree), note.at(i)->getValue(NoteAttr::delta), note.at(i)->getValue(NoteAttr::prev_curv), note.at(i)->getValue(NoteAttr::next_curv), findDistanceByTime(note.at(i)->getValue(NoteAttr::time))
				));
			if (note.at(i)->getValue(NoteAttr::delta) != 0) {
				trail_distance.emplace_back(std::make_shared<TrailDistance>(
					note.at(i)->getValue(NoteAttr::time) + 1, note.at(i)->getValue(NoteAttr::degree) + note.at(i)->getValue(NoteAttr::delta), 0, 0, 0, findDistanceByTime(note.at(i)->getValue(NoteAttr::time) + 1)
					));
			}
		}
	}
	sort(trail_distance.begin(), trail_distance.end(), [](const std::shared_ptr<TrailDistance>& a, const std::shared_ptr<TrailDistance>& b) {
		if (a->getDistance() < b->getDistance()) {
			return true;
		}
		else if (a->getDistance() == b->getDistance()) {
			return a->getTime() < b->getTime();
		}
		else {
			return false;
		}
		});
}

void Chart::catchRotate(const std::size_t pos, const double degree) {
	if (note.at(pos)->getType() != NoteType::Catch) {
		return;
	}
	double a = 0;
	a = findDegreeByTime(note.at(pos)->getValue(NoteAttr::time));
	a = fmod(a, 180);
	if (a < 0) {
		a += 180;
	}
	if ((static_cast<int>(note.at(pos)->getValue(NoteAttr::degree)) % 2) == 0) {
		a += degree;
		a = fmod(a, 360);
		if (a < 0) {
			a += 360;
		}
		if (a >= 180) {
			note.at(pos)->setValue(NoteAttr::degree, 1);
		}
	}
	else {
		a += 180 + degree;
		a = fmod(a, 360);
		if (a < 0) {
			a += 360;
		}
		if (a < 180) {
			note.at(pos)->setValue(NoteAttr::degree, 0);
		}
	}
}

void Chart::sortAll() {
	sortBPMByTime();
	sortSpeedByTime();
	sortNoteByTime();
}



void Chart::sortBPMByTime() {
	auto sortcol = [](const std::shared_ptr<BPM>& v1, const std::shared_ptr<BPM>& v2) { 
		return v1->getValue(BPMAttr::time) < v2->getValue(BPMAttr::time); 
	};
	sort(bpm.begin(), bpm.end(), sortcol);
}

void Chart::sortSpeedByTime() {
	auto sortcol = [](const std::shared_ptr<Speed>& v1, const std::shared_ptr<Speed>& v2) {
		return v1->getValue(SpeedAttr::time) < v2->getValue(SpeedAttr::time);
	};
	sort(speed.begin(), speed.end(), sortcol);
}

void Chart::sortNoteByTime() {
	auto sortcol = [](const std::shared_ptr<Note>& v1, const std::shared_ptr<Note>& v2) {
		return v1->getValue(NoteAttr::time) < v2->getValue(NoteAttr::time);
	};
	sort(note.begin(), note.end(), sortcol);
}

void Chart::printNote(const std::size_t pos) const {
	for (double value : note.at(pos)->getVector()) {
		std::cout << value << ", ";
	}
	std::cout << note.at(pos)->getVector().back() << std::endl;
}

double Chart::getYfromX(const double a, const double b, const double ca, const double cb, const double x) {
	//find cubic x value based on control point a and b.
	//Note: A & B for this function is NOT the start and end point of the curve, instead they are the control points
	auto findX = [](double a, double b, double t) {
		return t * (3 * a - 6 * a * t + 3 * b * t + t * t + 3 * a * t * t - 3 * b * t * t);
	};

	//find cubic y value based on t
	auto findY = [](double t) {
		return -2 * t * t * t + 3 * t * t;
	};

	//Approximated t value based on x
	//binary search
	double uT = 1; //tail 
	double lT = 0; //head
	double t = 0; //compare time

	//Do a binary search 10 time to appoximiate the target time
	//For each loop, the compare time = (head + tail) / 2
	//And then compare the X at compare time
	for (std::size_t i = 0; i < 20; i++)
	{
		t = (uT + lT) / 2;
		double searchX = findX(ca, 1 - cb, t);
		if (searchX < x)
			lT = t;
		else
			uT = t;
	}

	return findY(t) * (b - a) + a;
}

void Chart::clear() {
	bpm.clear();
	speed.clear();
	note.clear();
}

bool Chart::isNote(const Note& input) {
	NoteType type = input.getType();
	return (type == NoteType::Tap || type == NoteType::Flick || type == NoteType::Slide || type == NoteType::Rotate || type == NoteType::Catch);
}

std::string Chart::getChart() {
	std::stringstream chart;
	chart.str("");
	chart.clear();
	chart << "# Version 2" << std::endl;
	chart << std::endl;
	chart << "# BPM" << std::endl;
	printInfo(chart, bpm);
	chart << std::endl;
	chart << "# Speed" << std::endl;
	printInfo(chart, speed);
	chart << std::endl;
	chart << "# Note" << std::endl;
	printInfo(chart, note);

	return chart.str();
}

void Chart::readBPM(const std::string str) {
	bpm.emplace_back(std::make_shared<BPM>(readInfo(str)));
}

void Chart::readSpeed(const std::string str) {
	speed.emplace_back(std::make_shared<Speed>(readInfo(str)));
}

void Chart::readNote(const std::string str) {
	pushBackNote(readInfo(str));
}

std::deque<double> Chart::readInfo(const std::string str) {
	std::deque<double> data;
	std::size_t found = str.find(",");
	std::string str2, str3 = str;
	try {
		while (found != std::string::npos) {
			str2 = str3.substr(0, found);
			str3 = str3.substr(found + 1);
			data.push_back(stod(str2));
			found = str3.find(",");
		}
		data.push_back(stod(str3));
	}
	catch (const std::exception& e) {
		std::cerr << "Error: The following error occurred while using stod in readFile: " << e.what() << std::endl;
		pauseConsoleAndExit();
	}
	return data;
}

void Chart::pauseConsoleAndExit() {
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Application closing, press enter to continue...";
	std::cin.get();
	exit(1);
}