#include "MainProcess.h"

void MainProcess::getDistance() {
    double time;
    std::string command;

    command = InputScreen::inputArgument(chart, "Get Distance",
        time, "time(ms)", "-");
    if (command == "x") {
        return;
    }

    std::cout << std::endl << "distance: " << chart.findDistanceByTime(time) << std::endl;
    std::cout << "degree: " << chart.findDegreeByTime(time) << std::endl;

    showComplete();
}