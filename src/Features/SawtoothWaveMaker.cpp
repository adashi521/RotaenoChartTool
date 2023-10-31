#include "MainProcess.h"

void MainProcess::sawtoothWaveMaker() {
    std::string command;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;
    double amplitude = 60, period = 0, beat = 4;
    double pc = 0, nc = 0;

    command = InputScreen::inputArgument(chart, "Sawtooth Wave Maker",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        amplitude, "amplitude", "0-180",
        beat, "input _th gap", "1-128",
        period, "wave half period (4th beat)", "0-",
        pc, "previous curve", "0-100",
        nc, "next curve", "0-100"
        );
    if (command == "x") {
        return;
    }

    pc = pc / 100;
    nc = nc / 100;

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);
    std::deque<std::size_t> trail_pos;
    for (std::size_t i = start_pos; i < chart.getNoteSize() && i <= end_pos; i++) {
        if (chart.getNote(i).getType() == NoteType::Trail) {
            trail_pos.push_back(i);
        }
    }

    beat = beat / 4;
    double now = start, bpm = chart.getBPM(chart.findBPMPosByTime(start, 1)).getValue(BPMAttr::time), now_degree = 0, move = 0;
    double period_start = start, period_end = start + 60000 / bpm * period;
    const double time_gap = 60000 / bpm / beat / 2;
    int state = 0, state_period = 0;
    while (now <= end) {
        now_degree = fmod(chart.findDegreeByTime(round(now)), 180);
        //cout << now << ", " << period_start << ", " << period_end << endl;
        //cout << move << endl;
        if (period != 0) {
            if (state_period == 0 && state % 8 == 1) {
                move = Chart::getYfromX(0, amplitude, pc, nc, (now - period_start + time_gap * 3) / (period_end - period_start));
            }
            else if (state_period == 1 && state % 8 == 1) {
                move = Chart::getYfromX(0, amplitude, pc, nc, (period_end - now - time_gap * 3) / (period_end - period_start));
            }
        }
        else {
            move = amplitude;
        }

        if (state % 4 == 0) {
            chart.pushBackNote({ 11, round(now), round(now_degree), 0, 0, 0 });
        }
        else if (state % 8 == 2){
            chart.pushBackNote({ 11, round(now), round(now_degree + move), 0, 0, 0 });
        }
        else if (state % 8 == 6) {
            chart.pushBackNote({ 11, round(now), round(now_degree - move), 0, 0, 0 });
        }
        else if ((state % 8 == 1 || state % 8 == 3) && move >= 90) {
            chart.pushBackNote({ 11, round(now), round(now_degree + move / 2), 0, 0, 0 });
        }
        else if ((state % 8 == 5 || state % 8 == 7) && move >= 90) {
            chart.pushBackNote({ 11, round(now), round(now_degree - move / 2), 0, 0, 0 });
        }

        state = (state + 1) % 8;
        now += time_gap;

        if (period != 0 && now + time_gap > period_end) {
            period_start = period_end;
            period_end += 60000 / bpm * period;
            state_period = state_period ^ 1;
        }
    }
    for (int i = static_cast<int>(trail_pos.size()) - 1; i >= 0; i--) {
        chart.deleteNote(trail_pos.at(i));
    }
    

    showCompleteAndSave();
}