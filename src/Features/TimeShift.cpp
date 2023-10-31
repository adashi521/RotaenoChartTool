#include "MainProcess.h"

void MainProcess::timeShift() {
    std::string command;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;
    double shift = 0;

    command = InputScreen::inputArgument(chart, "Time Shift",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        shift, "shift(ms)", "-");
    if (command == "x") {
        return;
    }

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);
    for (std::size_t i = start_pos; i < chart.getNoteSize() && i <= end_pos; i++) {
        chart.getNote(i).increment(NoteAttr::time, shift);
    }
    start_pos = chart.findSpeedPosByTime(start);
    end_pos = chart.findSpeedPosByTime(end, 1);
    for (std::size_t i = start_pos; i < chart.getSpeedSize() && i <= end_pos; i++) {
        chart.getSpeed(i).increment(SpeedAttr::time, shift);
    }

    showCompleteAndSave();
}