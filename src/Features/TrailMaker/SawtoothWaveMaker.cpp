#include "MainProcess.h"

void MainProcess::sawtoothWaveMaker() {
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

    auto generateRandomBool = []() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution d(0.5);
        return d(gen);
    };

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

    double now = start, next_time_gap = 0, bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm), now_degree = chart.findDegreeByTime(start), move = 0;
    double period_start = start, period_end = start + 60000 / bpm * period;
    std::size_t segment = 0;
    double line_degree = chart.findDegreeByTime(now);
    bool even = generateRandomBool(), state = false, state_period = false, first_check = false;

    while (round(now) - 1 <= end) {
        bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
        next_time_gap = 60000 / bpm / wavelength * 2;

        if (state == false) {
            if (!first_check) {
                if (chart.getNote(chart.findSpecificNotePosByTime(now, NoteType::Rotate)).getTime() == now) {
                    line_degree = chart.findDegreeByTime(now + 1);
                    now_degree = line_degree;
                }
                first_check = true;
            }
            else {
                line_degree = chart.findDegreeByTime(now);
            }

            if (period == 0) {
                move = amplitude_start;
            }
            else {
                if (state_period == 0) {
                    move = Chart::getYfromX(amplitude_start, amplitude_end, pc, nc, (now - period_start + next_time_gap * 3) / (period_end - period_start));
                }
                else {
                    move = Chart::getYfromX(amplitude_start, amplitude_end, pc, nc, (period_end - now - next_time_gap * 3) / (period_end - period_start));
                }
                
            }

            if (!even) {
                move = -move;
            }
            even = even ^ 1;
            state = true;
        }
        else {
            line_degree = chart.findDegreeByTime(now + next_time_gap);
            move = line_degree - now_degree;
            state = false;
        }

        segment = static_cast<std::size_t>(abs(move) / 90) + 1;
        for (std::size_t i = 0; i < segment; i++) {
            if (round(now + (next_time_gap / segment * i) - 1 > end)) {
                break;
            }
            chart.pushBackNote({ 11, round(now + (next_time_gap / segment * i)), round(now_degree + (move / segment * i)), 0, pc, nc });
        }

        //std::cout << "now: " << now << "period_start: " << period_start << "period_end: " << period_end << "move: " << move << std::endl;
        
        now_degree += move;
        now += next_time_gap;

        if (period != 0 && now + next_time_gap > period_end) {
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