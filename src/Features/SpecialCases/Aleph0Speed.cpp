#include "MainProcess.h"

void MainProcess::aleph0Speed() {
    std::string command;
    double start = -1, end = -1, ms = 17;

    command = InputScreen::inputArgument(chart, "Aleph-0 Speed Maker",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        ms, "gap ms", "0-");
    if (command == "x") {
        return;
    }

    // command
    // Aleph-0 [IV]_2.gwc2
    /*
    as 27309 40594 35.71
    as 108936 110856 40

    as 111816 115656 40

    as 116616 118536 40

    as 119496 123336 40
    */

    double now_time = start, now_bpm = chart.getBPM(chart.findBPMPosByTime(end, 1)).getValue(BPMAttr::bpm);
    int count = 1;
    bool first = false;

    double v1 = 0.3, v2 = 1;
    auto getSpeed = [&](double t) {
        return 8000 + (v2 - v1) * (t - start);
    };
    
    std::vector<double> for_rotaeno_bug_ms_1 = {109256.0, 109576.0, 113256.0, 113576.0, 115256.0, 115576.0, 117256.0, 117576.0, 119576.0, 121256.0, 121576.0, 123256.0};
    std::vector<double> for_rotaeno_bug_ms_2 = {110256.0, 110576.0, 112256.0, 112576.0, 114256.0, 114576.0, 118256.0, 120256.0, 120576.0, 122256.0, 122576.0};
    auto bug_check = [&](std::vector<double> bug_vector, double input) {
        return std::any_of(bug_vector.begin(), bug_vector.end(), [&](double value) {
            return std::fabs(input - value) < 3.0;
        });
    };
    


    bool up = true;
    while (now_time <= end) {
        //now_bpm = chart.getBPM(chart.findBPMPosByTime(now_time, 1)).getValue(BPMAttr::bpm);
        if (up) {
            if (bug_check(for_rotaeno_bug_ms_1, now_time)) {
                chart.pushBackSpeed({ now_time, getSpeed(now_time) + v2 - 60, 0 });
            }
            else {
                chart.pushBackSpeed({ now_time, getSpeed(now_time) + v2, 0 });
            }
            if (now_time + 1 > end) {
                chart.pushBackSpeed({ now_time + 1, v1, 0 });
            }
            else {
                chart.pushBackSpeed({ now_time + 1, v2, 0 });
            }
        }
        else {
            if (bug_check(for_rotaeno_bug_ms_2, now_time)) {
                chart.pushBackSpeed({ now_time, -getSpeed(now_time) + v1 + 60, 0 });
            }
            else {
                chart.pushBackSpeed({ now_time, -getSpeed(now_time) + v1, 0 });
            }
            chart.pushBackSpeed({ now_time + 1, v1, 0 });
            
        }
        now_time += ms;
        up = !up;
    }

    showCompleteAndSave();
}