#include "InputScreen.h"

std::deque<std::deque<std::string>> InputScreen::screen_deq;

std::string InputScreen::inputFromUser(const std::string& input, const std::string& range, const std::size_t count) {
    std::string command;

    while (1) {
        clearScreen();
        printStringDeq(count);
        std::cout << std::endl << "Input \"x\" to exit" << std::endl;
        if (range.find("all") != std::string::npos) {
            std::cout << "Input \"all\" to select the whole chart" << std::endl;
        }
        std::cout << std::endl << "Input :";
        std::cin >> command;
        if (inRange(command, range) || command == "x") {
            if (command == "all") {
                addToStringDeq("0", count);
            }
            else {
                addToStringDeq(command, count);
            }
            break;
        }
    }
    return command;
}

void InputScreen::addToStringDeq(const std::string input, const std::size_t count) {
    screen_deq.at(count + 2).back() = input;
}

void InputScreen::printStringDeq(const std::size_t count) {
    for (std::size_t i = 0; i < InputScreen::screen_deq.size(); i++) {
        if (i == count + 2) {
            std::cout << "¡÷";
        }
        else if (i >= 2) {
            std::cout << "  ";
        }
        for (std::string str : screen_deq.at(i)) {
            std::cout << str;
        }
        std::cout << std::endl;
    }
}

void InputScreen::clearAll() {
    InputScreen::screen_deq.clear();
}

void InputScreen::setTitle(const std::string& input) {
    InputScreen::screen_deq.push_back(std::deque<std::string>{input});
    InputScreen::screen_deq.push_back(std::deque<std::string>({""}));
}

void InputScreen::setScreenString(const std::string& input) {
    InputScreen::screen_deq.push_back(std::deque<std::string>{input + " = ", ""});
}

bool InputScreen::inRange(const std::string& target, const std::string& range_str) {
    std::deque<std::string> range_elements;
    std::istringstream iss(range_str);
    std::string token;
    bool is_range = false;
    double target_num = 0;

    if (isNumber(target)) {
        target_num = stod(target);
    }
    while (getline(iss, token, ' ')) {
        is_range = false;
        std::size_t hyphen_pos = token.find('-');
        if (token.size() == 1 && isNumber(target)) {
            return true;
        }
        else if (hyphen_pos == token.size() - 1) {
            if (isNumber(token.substr(0, hyphen_pos))) {
                double start = stod(token.substr(0, hyphen_pos));
                if (isNumber(target) && target_num >= start) {
                    return true;
                }
                is_range = true;
            }
        }
        else if (hyphen_pos == 0) {
            if (isNumber(token.substr(hyphen_pos + 1))) {
                double end = stod(token.substr(hyphen_pos + 1));
                if (isNumber(target) && target_num <= end) {
                    return true;
                }
                is_range = true;
            }
        }
        else if (hyphen_pos != std::string::npos) {
            if (isNumber(token.substr(0, hyphen_pos)) && isNumber(token.substr(hyphen_pos + 1))) {
                double start = stod(token.substr(0, hyphen_pos));
                double end = stod(token.substr(hyphen_pos + 1));
                if (isNumber(target) && target_num >= start && target_num <= end) {
                    return true;
                }
                is_range = true;

            }
        }
        if ((isNumber(target) && isNumber(token)) || is_range == false) {
            range_elements.push_back(token);
        }
    }
    return find(range_elements.begin(), range_elements.end(), target) != range_elements.end();
}

bool InputScreen::isNumber(const std::string& s) {
    try {
        std::size_t pos;
        double num = std::stod(s, &pos);
        return pos == s.length();
    }
    catch (const std::exception&) {
        return false;
    }
}

void InputScreen::clearScreen() {
#ifdef _WIN32
    system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
    system("clear");
#endif
}
