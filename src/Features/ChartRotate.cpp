#include "MainProcess.h"

void MainProcess::chartRotate() {
    std::string command;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;
    double degree = 0;

    command = InputScreen::inputArgument(chart, "Chart Rotate",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        degree, "degree", "-");
    if (command == "x") {
        return;
    }

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);
    for (std::size_t i = start_pos; i < chart.getNoteSize() && i <= end_pos; i++) {
        if (chart.getNote(i).getType() != NoteType::Catch) {
            chart.getNote(i).increment(NoteAttr::degree, degree);
            if (chart.getNote(i).getType() == NoteType::Slide) {
                chart.getNote(i).increment(NoteAttr::end_degree, degree);
            }
        }
        else {
            chart.catchRotate(i, degree);
        }
    }

    showCompleteAndSave();
}