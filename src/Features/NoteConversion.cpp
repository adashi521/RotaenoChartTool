#include "MainProcess.h"

void MainProcess::noteConversion() {
    std::string command;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;
    double type1 = 0, type2 = 0;

    command = InputScreen::inputArgument(chart, "Note Conversion",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        type1, "from (0:tap, 1:flick, 2:slide(only head) 3:slide(full), 5:catch, 6:bomb)", "0 1 2 3 5 6",
        type2, "to (0:tap, 1:flick, 2:small slide, 3:big slide, 5:catch, 6:bomb)", "0 1 2 3 5 6");
    if (command == "x") {
        return;
    }

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end);

    for (std::size_t i = start_pos; i < chart.getNoteSize() && i <= end_pos; i++) {
        Note& note = chart.getNote(i);
        int numtype = note.getIntType();
        double now_time = note.getValue(NoteAttr::time);
        double now_degree = note.getValue(NoteAttr::degree);
        now_degree = fmod(now_degree, 360);
        if (now_degree < 0) {
            now_degree += 360;
        }

        if (numtype == static_cast<int>(type1) || (numtype == 2 && static_cast<int>(type1) == 3)) {
            if (type1 == 0 || type1 == 1) {
                if (type2 == 0 || type2 == 1) {
                    chart.pushBackNote({ type2, now_time, now_degree });
                }
                if (type2 == 2 || type2 == 3) {
                    chart.pushBackNote({ 2, now_time, now_degree, type2, now_degree, 1, 1, 0, 0 });
                }
                if (type2 == 5 || type2 == 6) {
                    if (now_degree >= 180) {
                        chart.pushBackNote({ type2, now_time, 1 });
                    }
                    else {
                        chart.pushBackNote({ type2, now_time, 0 });
                    }
                }
            }
            else if (type1 == 2) {
                if (type2 == 0 || type2 == 1 || type2 == 2 || type2 == 3) {
                    chart.pushBackNote({ type1, now_time, now_degree, type2, note.getValue(NoteAttr::end_degree), note.getValue(NoteAttr::snap), note.getValue(NoteAttr::amount), note.getValue(NoteAttr::prev_curv), note.getValue(NoteAttr::next_curv) });
                }
                else if (type2 == 5 || type2 == 6) {
                    if (now_degree >= 180) {
                        chart.pushBackNote({ type2, now_time, 1 });
                    }
                    else {
                        chart.pushBackNote({ type2, now_time, 0 });
                    }
                }
            }
            else if (type1 == 3) {
                const double now_bpm = chart.getBPM(chart.findBPMPosByTime(note.getValue(NoteAttr::time))).getValue(BPMAttr::bpm);
                const double degree = note.getValue(NoteAttr::degree);
                const double end_degree = note.getValue(NoteAttr::end_degree);
                const double snap = note.getValue(NoteAttr::snap);
                const double amount = note.getValue(NoteAttr::amount);
                const double degree_gap = (end_degree - degree) / amount;
                const double time_gap = 60000 / now_bpm / snap;
                for (int i = 0; i < amount; i++) {
                    if (type2 == 0 || type2 == 1) {
                        chart.pushBackNote({ type2, now_time + time_gap * i, now_degree + degree_gap * i });
                    }
                    if (type2 == 2 || type2 == 3) {
                        chart.pushBackNote({ 2, now_time + time_gap * i, now_degree + degree_gap * i, type2, now_degree + degree_gap * i, 1, 1, 0, 0 });
                    }
                    if (type2 == 5 || type2 == 6) {
                        if (now_degree + degree_gap * i >= 180) {
                            chart.pushBackNote({ type2, now_time + time_gap * i, 1 });
                        }
                        else {
                            chart.pushBackNote({ type2, now_time + time_gap * i, 0 });
                        }
                    }
                }
            }
            else if (type1 == 5 || type1 == 6) {
                double new_degree = chart.findDegreeByTime(now_time);
                if (static_cast<bool>(now_degree) == 1) {
                    new_degree += 180;
                }
                if (type2 == 0 || type2 == 1) {
                    chart.pushBackNote({ type2, now_time, new_degree });
                }
                if (type2 == 2 || type2 == 3) {
                    chart.pushBackNote({ 2, now_time, new_degree, type2, new_degree, 1, 1, 0, 0 });
                }
                if (type2 == 5 || type2 == 6) {
                    chart.pushBackNote({ type2, now_time, now_degree });
                }
            }
        }
    }

    for (int i = static_cast<int>(end_pos); i >= static_cast<int>(start_pos); i--) {
        int numtype = chart.getNote(i).getIntType();
        if (numtype == static_cast<int>(type1) || (numtype == 2 && static_cast<int>(type1) == 3)) {
            chart.deleteNote(i);
        }
    }

    showCompleteAndSave();
}