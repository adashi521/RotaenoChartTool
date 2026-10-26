#include "MainProcess.h"

void MainProcess::dextorySpeed2() {
    std::string command;
    double start = -1, end = -1, mean = 0, std = 0, start_degree = 0, ms;
    double gap = 60;

    command = InputScreen::inputArgument(chart, "Dextroy Speed Maker 2",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        ms, "ms", "0-");
    if (command == "x") {
        return;
    }

    // command
    // dextroy [IV]_9.gwc2
    /*
    dx2 90293 90798 32
    */

    const double th = 32;
    const double middle = 150, move = middle / th;
    double now_time = end, now_bpm = chart.getBPM(chart.findBPMPosByTime(end, 1)).getValue(BPMAttr::bpm);
    int count = 1;
    bool first = false;

    while (now_time >= start) {
        now_bpm = chart.getBPM(chart.findBPMPosByTime(now_time, 1)).getValue(BPMAttr::bpm);

        if (count == 0) {
            chart.pushBackSpeed({ round(now_time - 1), 4000 - (th - 1) * move, 0 });
        }
        else if (first == false) {
            chart.pushBackSpeed({ round(now_time - 1), 4000 + (2*count - 1) * move, 0 });
            first = true;
        }
        else if (count % 2 == 1) {
            chart.pushBackSpeed({ round(now_time - 1), -4000 + (2*count - 1) * move, 0 });
        }
        else {
            chart.pushBackSpeed({ round(now_time - 1), 4000 - (2*count - 1) * move, 0 });
        }

        chart.pushBackSpeed({ round(now_time), 0, 0 });

        count = (count + 1) % static_cast<int>(th);
        now_time -= ms;
    }

    showCompleteAndSave();
}