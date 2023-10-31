#include "MainProcess.h"

void MainProcess::realCopy() {
    std::string command;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;
    double des = 0;

    command = InputScreen::inputArgument(chart, "Real Copy",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        des, "destination(ms)", "-");
    if (command == "x") {
        return;
    }

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);
    for (std::size_t i = start_pos; i < chart.getNoteSize() && i <= end_pos; i++) {
        chart.pushBackNote(chart.getNote(i).getVector());
        chart.getNote(chart.getNoteSize() - 1).increment(NoteAttr::time, des - start);
    }

    start_pos = chart.findSpeedPosByTime(start);
    end_pos = chart.findSpeedPosByTime(end, 1);
    for (std::size_t i = start_pos; i < chart.getSpeedSize() && i <= end_pos; i++) {
        chart.pushBackSpeed(chart.getSpeed(i).getVector());
        chart.getSpeed(chart.getSpeedSize() - 1).increment(SpeedAttr::time, des - start);
    }

    showCompleteAndSave();
}