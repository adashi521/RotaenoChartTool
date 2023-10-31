#include "MainProcess.h"

void MainProcess::randomRightAngleMaker() {
    std::string command;
    double start = -1, end = -1, mean = 0, std = 0, start_degree = 0, th;
    std::size_t start_pos, end_pos;
    double gap = 60;

    command = InputScreen::inputArgument(chart, "Random Right Angle Maker",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        th, "_th note", "0-",
        mean, "mean(degree)", "-",
        std, "stddev(degree)", "0-");
    if (command == "x") {
        return;
    }

    auto generateRandomNumber = [](double mean, double stddev) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dist(mean, stddev);
        return dist(gen);
    };

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);
    std::deque<std::size_t> trail_pos;
    for (std::size_t i = start_pos; i < chart.getNoteSize() && i <= end_pos; i++) {
        if (chart.getNote(i).getType() == NoteType::Trail) {
            trail_pos.push_back(i);
        }
    }

    double now = start, bpm = 0, now_degree = chart.findDegreeByTime(start), random_degree = 0;
    bool even = true;
    while (now <= end) {
        random_degree = generateRandomNumber(mean, std);
        if (even) {
            chart.pushBackNote({ 11, round(now), round(now_degree), (round)(random_degree), 0, 0 });
        }
        else {
            chart.pushBackNote({ 11, round(now), round(now_degree), (round)(random_degree), 0, 0 });
        }
        bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
        even = even ^ 1;
        now += 60000 / bpm / th * 4;
        now_degree += random_degree;
    }

    for (int i = static_cast<int>(trail_pos.size()) - 1; i >= 0; i--) {
        chart.deleteNote(trail_pos.at(i));
    }

    showCompleteAndSave();
}