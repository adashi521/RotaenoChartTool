#include "MainProcess.h"

void MainProcess::heavensCageFlick() {
    std::string command;
    double start = -1, end = -1, degree = 0;
    std::size_t start_pos, end_pos;

    command = InputScreen::inputArgument(chart, "Heaven's Cage Flick",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        degree, "degree", "0-"
        );
    if (command == "x") {
        return;
    }

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);
    double middle_pos = 0;

    for (std::size_t i = start_pos; i <= end_pos; i++) {
        if (chart.getNote(i).getType() != NoteType::Flick) {
            continue;
        }
                
        for (std::size_t j = i + 1; j < end_pos; j++) {
            if (chart.getNote(j).getTime() > chart.getNote(i).getTime()) {
                break;
            }
            else if (chart.getNote(j).getTime() == chart.getNote(i).getTime() && chart.getNote(j).getType() == NoteType::Flick) {
                if (abs(chart.getNote(i).getValue(NoteAttr::degree) - chart.getNote(j).getValue(NoteAttr::degree)) < 30) {
                    middle_pos = (chart.getNote(i).getValue(NoteAttr::degree) + chart.getNote(j).getValue(NoteAttr::degree)) / 2;
                    chart.getNote(i).setValue(NoteAttr::degree, middle_pos + degree);
                    chart.getNote(j).setValue(NoteAttr::degree, middle_pos - degree);
                    break;
                    std::cout << i << ", " << j << std::endl;
                }
            }
        }
    }

    showCompleteAndSave();
}