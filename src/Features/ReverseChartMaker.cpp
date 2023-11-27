#include "MainProcess.h"

void MainProcess::ReverseChartMaker() {
    std::string command;
    double start = -1, end = -1, shift = 0;
    std::size_t start_pos, end_pos;

    command = InputScreen::inputArgument(chart, "Reverse Chart Maker",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        shift, "reverse shift", "0-");
    if (command == "x") {
        return;
    }

    double time = 0;
    std::size_t speed_pos = 0;
    std::deque<double> record;
    for (std::size_t i = 0; i < chart.getNoteSize(); i++) {
        time = chart.getNote(i).getValue(NoteAttr::time);
        if (time >= start && time <= end) {
            if (Chart::isNote(chart.getNote(i))) {
                if (time == 0) {
                    time++;
                    chart.getNote(i).setValue(NoteAttr::time, time);
                }
                speed_pos = chart.findSpeedPosByTime(time);
                if (abs(chart.getSpeed(speed_pos).getValue(SpeedAttr::time) - time) > 1) {
                    chart.pushBackSpeed({time + 1, -1, 0});
                    chart.pushBackSpeed({time, -shift, 0});
                    chart.pushBackSpeed({time - 1, shift, 0});
                    record.push_back(time);
                }
            }
        }
    }
    std::size_t now = 0;
    for (std::size_t i = 0; i < chart.getNoteSize(); i++) {
        NoteType type = chart.getNote(i).getType();
        time = chart.getNote(i).getValue(NoteAttr::time);
        if (now < record.size() && time - record.at(now) >= 2) {
            now++;
        }
        if (time >= start && time <= end) {
            if (type == NoteType::Trail) {
                if (time == 0) {
                    time++;
                }
                if (now < record.size() && abs(time - record.at(now)) <= 2) {
                    //chart.note.push_back(deque<std::size_t>{11, time + shift, chart.note.at(i).at(2), chart.note.at(i).at(3), chart.note.at(i).at(4), chart.note.at(i).at(5)});
                    time += 3;
                }
                chart.getNote(i).setValue(NoteAttr::time, time);
            }
        }
    }

    showCompleteAndSave();
}