#include "ChartDetails/ChartElement.h"

void ChartElement::pauseConsoleAndExit() {
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Application closing, press enter to continue...";
	std::cin.get();
	exit(1);
}
