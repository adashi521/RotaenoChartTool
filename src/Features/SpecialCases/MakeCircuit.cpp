#include "MainProcess.h"

void MainProcess::makeCircuit() {
    std::string command;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;
    double gap = 60;

    command = InputScreen::inputArgument(chart, "Make Circuit",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        gap, "gap", "0-180");
    if (command == "x") {
        return;
    }

    double now = start, bpm = chart.getBPM(chart.findBPMPosByTime(start, 1)).getValue(BPMAttr::bpm), line_degree = 0;
    bool even = true;
    while (now < end) {
        line_degree = chart.findDegreeByTime(round(now));
        chart.pushBackNote({21, round(now), round(line_degree + gap)});
        chart.pushBackNote({21, round(now), round(line_degree - gap)});
        chart.pushBackNote({21, round(now), round(line_degree + gap + 180)});
        chart.pushBackNote({21, round(now), round(line_degree - gap + 180)});
        now += 60000 / bpm / 4;
    }

    showCompleteAndSave();
}