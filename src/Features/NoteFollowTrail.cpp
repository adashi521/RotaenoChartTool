#include "MainProcess.h"

void MainProcess::noteFollowTrail() {
    std::string command;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;

    command = InputScreen::inputArgument(chart, "Note Follow Trail",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-");
    if (command == "x") {
        return;
    }

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);

    NoteType type;
    double final_degree, ori_degree, line_degree;
    int count = 0;
    for (std::size_t i = start_pos; i < chart.getNoteSize() && i <= end_pos; i++) {
        ori_degree = fmod(chart.getNote(i).getValue(NoteAttr::degree), 360);
        line_degree = chart.findDegreeByTime(chart.getNote(i).getValue(NoteAttr::time));
        type = chart.getNote(i).getType();
        final_degree = fmod(round(line_degree), 180);
        if (ori_degree < 0) {
            ori_degree += 360;
        }
        if (ori_degree > 180) {
            final_degree += 180;
        }
        if (type == NoteType::Tap || type == NoteType::Flick) {
            chart.getNote(i).setValue(NoteAttr::degree, final_degree);
        }
        else if (type == NoteType::Slide) {
            double now_bpm = chart.getBPM(chart.findBPMPosByTime(chart.getNote(i).getValue(NoteAttr::time))).getValue(BPMAttr::bpm);
            double snap = chart.getNote(i).getValue(NoteAttr::snap);
            double amount = chart.getNote(i).getValue(NoteAttr::amount);
            double line_degree_2;
            chart.getNote(i).setValue(NoteAttr::degree, final_degree);
            if (amount > 1) {
                line_degree = chart.findDegreeByTime(chart.getNote(i).getValue(NoteAttr::time));
                line_degree_2 = chart.findDegreeByTime(chart.getNote(i).getValue(NoteAttr::time) + (60000 / now_bpm * (amount - 1) / snap));
                line_degree = fmod(line_degree, 180);
                line_degree_2 = fmod(line_degree_2, 180);
                final_degree = line_degree_2 + (line_degree_2 - line_degree) / (amount - 1);
                final_degree = fmod(round(final_degree), 180);
                if (fmod(ori_degree, 360) >= 180) {
                    final_degree += 180;
                }
            }
            chart.getNote(i).setValue(NoteAttr::end_degree, round(final_degree));
        }
        count++;
    }

    showCompleteAndSave();
}