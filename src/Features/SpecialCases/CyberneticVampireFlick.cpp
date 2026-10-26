#include "MainProcess.h"

void MainProcess::cyberneticVampireFlick() {
    std::string command;
    double start = -1, end = -1, degree = 0;
    std::size_t start_pos, end_pos;

    command = InputScreen::inputArgument(chart, "Cybernetic Vampire Flick",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        degree, "degree (default:10)", "0-"
        );
    if (command == "x") {
        return;
    }

    double now = start, bpm = 0, now_degree = 0, next_degree = 0;
    const int snap = 32;
    int status = 0;
    int count = 0, gap = degree, vec_count = 0;
    std::vector<int> vec = {-10, 15, -20, 25, -29, 32, -35, 38};

    while (now < end - 1) {
        bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
        now_degree = chart.findDegreeByTime(now);
        next_degree = chart.findDegreeByTime(now + 60000 / bpm * 4 / snap * 4);
        
        // deprecated 1
        // chart.pushBackNote({ 2, now, now_degree, 3, now_degree, 1, 1, 0, 0 });
        // chart.pushBackNote({ 1, now, now_degree + status });
        // chart.pushBackNote({ 1, now, now_degree - status });

        // now_degree += 180;
        // chart.pushBackNote({ 2, now, now_degree, 3, now_degree, 1, 1, 0, 0 });
        // chart.pushBackNote({ 1, now, now_degree + status });
        // chart.pushBackNote({ 1, now, now_degree - status });
        
        // deprecated 2
        // if (status == 0) {
        //     chart.pushBackNote({ 2, now, now_degree, 1, next_degree - degree, 8, 4, 0, 50 });
        //     chart.pushBackNote({ 2, now, now_degree, 2, next_degree + degree, 8, 4, 0, 50 });
        //     now_degree += 180;
        //     next_degree += 180;
        //     chart.pushBackNote({ 2, now, now_degree, 1, next_degree - degree, 8, 4, 0, 50 });
        //     chart.pushBackNote({ 2, now, now_degree, 2, next_degree + degree, 8, 4, 0, 50 });
        // }
        // else if (status == 4) {
        //     chart.pushBackNote({ 2, now, now_degree - degree, 2, next_degree, 8, 4, 50, 0 });
        //     chart.pushBackNote({ 2, now, now_degree + degree, 2, next_degree, 8, 4, 50, 0 });
        //     now_degree += 180;
        //     next_degree += 180;
        //     chart.pushBackNote({ 2, now, now_degree - degree, 2, next_degree, 8, 4, 50, 0 });
        //     chart.pushBackNote({ 2, now, now_degree + degree, 2, next_degree, 8, 4, 50, 0 });
        // }
        // chart.pushBackNote({ 5, now, 0});
        // chart.pushBackNote({ 5, now, 1});
        

        
        if (status == 0) {
            gap = -(gap + (gap/abs(gap))*3);
            chart.pushBackNote({ 2, now, now_degree, 1, next_degree + vec.at(vec_count), 8, 4, 0, 50 });
            chart.pushBackNote({ 2, now, now_degree + 180, 2, next_degree + 180 + vec.at(vec_count), 8, 4, 0, 50 });
            count++;
        }
        else if (status == 4) {
            chart.pushBackNote({ 2, now, now_degree + vec.at(vec_count), 2, next_degree, 8, 4, 50, 0 });
            chart.pushBackNote({ 2, now, now_degree + vec.at(vec_count) + 180, 2, next_degree + 180, 8, 4, 50, 0 });
            count++;
            vec_count++;
        }

        status = (status + 1) % 8;
        now += 60000 / bpm * 4 / snap;
    }

    showCompleteAndSave();
}