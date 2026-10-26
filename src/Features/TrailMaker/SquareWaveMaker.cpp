#include "MainProcess.h"

void MainProcess::squareWaveMaker() {
    std::string command;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;
    double amplitude_start = 60, amplitude_end = 0, period = 0, wavelength = 4, shift = 0;
    double pc = 0, nc = 0;

    command = InputScreen::inputArgument(chart, "Sawtooth Wave Maker",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        amplitude_start, "amplitude start", "0-",
        amplitude_end, "amplitude end", "0-",
        wavelength, "half wavelength (_th note)", "0-",
        period, "half period (4th beat)", "0-"
        //shift, "degree shift", "-"
        //pc, "previous curve", "0-100",
        //nc, "next curve", "0-100"
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

    double now = start, bpm = chart.getBPM(chart.findBPMPosByTime(start, 1)).getValue(BPMAttr::bpm);
    double line_degree = chart.findDegreeByTime(start), now_degree = line_degree, move = 0, next_fix_degree = 0;;
    double period_start = start, period_end = start + 60000 / bpm * period;
    const double time_gap = 60000 / bpm / wavelength * 4;
    bool odd = false, state_period = false;
    while (now <= end) {
        // odd: 0(top), 1(bottom)
        // state_period: 0(up), 1(down)

        line_degree = chart.findDegreeByTime(now);
        if (period != 0) {
            if (state_period == false) {
                move = Chart::getYfromX(amplitude_start, amplitude_end, pc, nc, (now - period_start + time_gap) / (period_end - period_start));
            }
            else {
                move = Chart::getYfromX(amplitude_start, amplitude_end, pc, nc, (period_end - now + time_gap) / (period_end - period_start));
            }
            if (odd == true){
                move = -move;
            }
        }
        else {
            move = (odd == true)? amplitude_start : -amplitude_start;
        }

        next_fix_degree = abs(now_degree - line_degree) - 80;
        if (next_fix_degree < 0) {
            next_fix_degree = 0;
        }
        if (odd) {
            next_fix_degree *= -1;
        }

        if (now_degree != line_degree) {
            if (next_fix_degree == 0) {
                chart.pushBackNote({ 11, round(now - 1), round(now_degree), 0, 0, 0 });
            }
            else {
                chart.pushBackNote({ 11, round(now - 2), round(now_degree), round(next_fix_degree), 0, 0 });
            }
        }

        chart.pushBackNote({ 11, round(now), round(line_degree), round(move), 0, 0 });

        now_degree = line_degree + move;
        now += time_gap;
        odd = odd ^ 1;

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