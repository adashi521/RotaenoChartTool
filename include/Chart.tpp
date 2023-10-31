template<typename T>
inline void Chart::deleteRange(std::deque<std::shared_ptr<T>>& deq, std::size_t front_pos, std::size_t back_pos) {
	if (back_pos == std::numeric_limits<std::size_t>::max()) {
		back_pos = front_pos;
	}
	else if (front_pos > back_pos) {
		std::swap(front_pos, back_pos);
	}
		
	if (back_pos < deq.size()) {
		deq.erase(deq.begin() + front_pos, deq.begin() + back_pos + 1);
	}
	else {
		std::cerr << "Invalid positions provided!" << std::endl;
	}
	return;
}

template <typename T>
inline void Chart::printInfo(std::stringstream& chart, const std::deque<std::shared_ptr<T>>& input) {
	auto is_integer = [](double num) {
		return floor(num) == num;
	};
	auto printNum = [&](double num) {
		if (is_integer(num)) {
			chart << static_cast<long long>(num);
		}
		else {
			chart << num;
		}
	};

	for (std::shared_ptr<T> element : input) {
		chart << std::fixed;
		chart << std::setprecision(2);
		for (std::size_t i = 0; i < element->getVector().size() - 1; i++) {
			printNum(element->getRoundVector().at(i));
			chart << ",";
		}
		printNum(element->getRoundVector().back());
		chart << std::endl;
	}
	return;
}

template <typename T>
inline std::size_t Chart::findPosByTime(const std::deque<std::shared_ptr<T>>& input, const double time, const int mode) const {
	std::size_t start = 0, end = input.size();
	std::size_t i;
	if (input.empty()) {
		return -1;
	}
	while (1) {
		i = (start + end) / 2;
		if (i >= input.size() - 1) {
			return input.size() - 1;
		}
		else if (i <= 0) {
			return 0;
		}
		else if (input.at(i)->getTime() <= time && input.at(i + 1)->getTime() >= time) {
			break;
		}
		else if (input.at(i)->getTime() < time) {
			start = i;
		}
		else if (input.at(i)->getTime() > time) {
			end = i;
		}
	}

	if (input.at(i)->getTime() < time && input.at(i + 1)->getTime() == time) {
		i++;
	}

	// example: find time 3
	// 1, 1, 2, 2, 2, 4, 4, 4, 5
	// mode0: return pos 2
	// mode1: return pos 4
	// mode2: return pos 5
	// mode3: return pos 7
	double new_time = time;
	switch (mode) {
	case 0:
		new_time = input.at(i)->getTime();
		while (1) {
			if (i > 0 && input.at(i - 1)->getTime() == new_time) {
				i--;
			}
			else {
				break;
			}
		}
		break;
	case 1:
		new_time = input.at(i)->getTime();
		while (1) {
			if (i < input.size() - 1 && input.at(i + 1)->getTime() == new_time) {
				i++;
			}
			else {
				break;
			}
		}
		break;
	case 2:
		if (i < input.size() - 1 && input.at(i)->getTime() != new_time)
			new_time = input.at(i + 1)->getTime();
		while (1) {
			if (i > 0 && input.at(i - 1)->getTime() == new_time) {
				i--;
			}
			else {
				break;
			}
		}
		break;
	case 3:
		if (i < input.size() - 1 && input.at(i)->getTime() != new_time)
			new_time = input.at(i + 1)->getTime();
		while (1) {
			if (i < input.size() - 1 && input.at(i + 1)->getTime() == new_time) {
				i++;
			}
			else {
				break;
			}
		}
		break;
	}

	return i;
}

template<typename T>
inline std::size_t Chart::findPosByDistance(const std::deque<std::shared_ptr<T>>& input, const double pos, const int mode) const {
	std::size_t start = 0, end = input.size();
	std::size_t i;
	if (input.empty()) {
		return -1;
	}
	while (1) {
		i = (start + end) / 2;
		if (i >= input.size() - 1) {
			return input.size() - 1;
		}
		else if (i <= 0) {
			return 0;
		}
		else if (input.at(i)->getDistance() <= pos && input.at(i + 1)->getDistance() >= pos) {
			break;
		}
		else if (input.at(i)->getDistance() < pos) {
			start = i;
		}
		else if (input.at(i)->getDistance() > pos) {
			end = i;
		}
	}

	if (input.at(i)->getDistance() < pos && input.at(i + 1)->getDistance() == pos) {
		i++;
	}

	double new_time = pos;
	switch (mode) {
	case 0:
		new_time = input.at(i)->getDistance();
		while (1) {
			if (i > 0 && input.at(i - 1)->getDistance() == new_time) {
				i--;
			}
			else {
				break;
			}
		}
		break;
	case 1:
		new_time = input.at(i)->getDistance();
		while (1) {
			if (i < input.size() - 1 && input.at(i + 1)->getDistance() == new_time) {
				i++;
			}
			else {
				break;
			}
		}
		break;
	case 2:
		if (i < input.size() - 1 && input.at(i)->getDistance() != new_time)
			new_time = input.at(i + 1)->getDistance();
		while (1) {
			if (i > 0 && input.at(i - 1)->getDistance() == new_time) {
				i--;
			}
			else {
				break;
			}
		}
		break;
	case 3:
		if (i < input.size() - 1 && input.at(i)->getDistance() != new_time)
			new_time = input.at(i + 1)->getDistance();
		while (1) {
			if (i < input.size() - 1 && input.at(i + 1)->getDistance() == new_time) {
				i++;
			}
			else {
				break;
			}
		}
		break;
	}

	return i;
}