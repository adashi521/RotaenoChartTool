#include "MainProcess.h"

void MainProcess::makeCircuit() {
    std::string command;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;
    double gap = 60;

    command = InputScreen::inputArgument(chart, "Make Circuit",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        gap, "gap", "0-180");
    if (command == "x") {
        return;
    }

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);
    std::deque<std::size_t> trail_pos;
    for (std::size_t i = start_pos; i < chart.getNoteSize() && i <= end_pos; i++) {
        if (chart.getNote(i).getType() == NoteType::Trail) {
            trail_pos.push_back(i);
        }
    }
    double now = start, bpm = chart.getBPM(chart.findBPMPosByTime(start, 1)).getValue(BPMAttr::bpm), degree1 = 0, degree2 = 0;
    bool even = true;
    while (now <= end) {
        degree1 = fmod(chart.findDegreeByTime(round(now)), 180);
        degree2 = fmod(chart.findDegreeByTime(round(now + 60000 / bpm / 16)), 180);
        if (even) {
            chart.pushBackNote({ 11, round(now), round(degree1 - gap / 2), (round)(180 - gap) * (-1), 0, 0 });
            //chart.note.push_back({ 11, round(now), round(degree1 - gap / 2), (round)(180 - gap + (degree1 - degree2) / 2) * (-1), 0, 0 });
            chart.pushBackNote({ 5, round(now), 0 });
            chart.pushBackNote({ 5, round(now), 1 });
            chart.pushBackNote({ 5, round(now) + 1, 0 });
            chart.pushBackNote({ 5, round(now) + 1, 1 });
        }
        else {
            chart.pushBackNote({ 11, round(now), round(degree1 + gap / 2), (round)(180 - gap), 0, 0 });
        }

        even = even ^ 1;
        now += 60000 / bpm / 16;
    }
    for (int i = static_cast<int>(trail_pos.size()) - 1; i >= 0; i--) {
        chart.deleteNote(trail_pos.at(i));
    }

    showCompleteAndSave();
}