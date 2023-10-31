#pragma once
#include <string>
#include <iostream>
#include <deque>
#include <algorithm>
#include <limits>
#include "Chart.h"

class InputScreen {
public:
	template <typename... Args>
	static std::string inputArgument(Chart& chart, const std::string& title, Args&... args);
	static void clearScreen();

private:
	InputScreen();
	static std::deque<std::deque<std::string>> screen_deq;

	static void clearAll();
	static void setTitle(const std::string& input);
    static void setScreenString(const std::string& input);
	static std::string inputFromUser(const std::string& input, const std::string& range, const std::size_t count = 0);
	static void addToStringDeq(const std::string input, const std::size_t count = 0);
	static void printStringDeq(const std::size_t count = std::numeric_limits<std::size_t>::max() - 2);
	static bool inRange(const std::string& target, const std::string& range_str);
	static bool isNumber(const std::string& s);
};

#include "InputScreen.tpp"

