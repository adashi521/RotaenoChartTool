#include "MainProcess.h"

void MainProcess::ReverseChartMaker() {
    std::string command;
    double start = -1, end = -1, shift = 0;
    std::size_t start_pos, end_pos;

    command = InputScreen::inputArgument(chart, "Reverse Chart Maker",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        shift, "reverse shift", "0-");
    if (command == "x") {
        return;
    }

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);

    double time = 0, prev_time = -1;
    std::size_t speed_pos = 0;
    std::deque<double> record;
    for (size_t i = start_pos; i < end_pos; i++) {
        time = chart.getNote(i).getValue(NoteAttr::time);
        if (prev_time == time) {
            continue;
        }
        
        
        if (Chart::isNote(chart.getNote(i)) || chart.getNote(i).getType() == NoteType::Custom) {
            if (time == 0) {
                time++;
                chart.getNote(i).setValue(NoteAttr::time, time);
            }
            speed_pos = chart.findSpeedPosByTime(time);
            if (abs(chart.getSpeed(speed_pos).getValue(SpeedAttr::time) - time) > 1) {
                chart.pushBackSpeed({time + 1, -0.5, 0});
                chart.pushBackSpeed({time, -shift, 0});
                chart.pushBackSpeed({time - 1, shift, 0});
                record.push_back(time);
            }
            prev_time = time;
        }
    }

    showCompleteAndSave();
}