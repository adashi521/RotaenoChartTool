#include "MainProcess.h"

void MainProcess::rotateTrailMaker() {
    std::string command;
    std::random_device rd;
    double start = -1, end = -1, clockwise = 0, laps = 0;
    std::size_t start_pos, end_pos;
    double gap = 60;

    command = InputScreen::inputArgument(chart, "Rotate Trail Maker",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        clockwise, "clockwise (0, 1)", "0 1",
        laps, "laps", "1-10"
        );
    if (command == "x") {
        return;
    }

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);
    laps = floor(laps);

    int prev_trail_pos = -1;
    double rotate_degree = 0, degree_gap = 0, time_gap = 0;
    double prev_degree = 0, next_degree = 0;
    int insert_count = 0;
    for (std::size_t i = start_pos; i <= end_pos; i++) {
        if (prev_trail_pos == -1) {
            if (chart.getNote(i).getType() == NoteType::Trail || chart.getNote(i).getType() == NoteType::Rotate) {
                prev_trail_pos = i;
                prev_degree = Chart::normalizeAngle(chart.getNote(prev_trail_pos).getValue(NoteAttr::degree) + chart.getNote(prev_trail_pos).getValue(NoteAttr::delta));
            }
            continue;
        }

        if (chart.getNote(i).getType() == NoteType::Trail || chart.getNote(i).getType() == NoteType::Rotate) {
            next_degree = Chart::normalizeAngle(chart.getNote(i).getValue(NoteAttr::degree));
            if (clockwise == 1) {
                if (abs(next_degree - prev_degree) <= 90) {
                    rotate_degree = 180 * laps + (next_degree - prev_degree);
                }
                else if (next_degree > prev_degree) {
                    rotate_degree = 180 * laps + (next_degree - prev_degree - 180);
                }
                else {
                    rotate_degree = 180 * laps + (next_degree - prev_degree + 180);
                }
            }
            else if (clockwise == 0) {
                if (abs(next_degree - prev_degree) <= 90) {
                    rotate_degree = 180 * laps - (next_degree - prev_degree);
                }
                else if (next_degree > prev_degree) {
                    rotate_degree = 180 * laps - (next_degree - prev_degree - 180);
                }
                else {
                    rotate_degree = 180 * laps - (next_degree - prev_degree + 180);
                }
                rotate_degree *= -1;
            }

            //std::cout << prev_degree << ", " << next_degree << ", " << rotate_degree << std::endl;
            insert_count = int(floor(abs(rotate_degree - 2) / 180) * 2 + 2) * 2;
            degree_gap = rotate_degree / insert_count;
            time_gap = (chart.getNote(i).getTime() - chart.getNote(prev_trail_pos).getTime()) / insert_count;
            for (int i = 1; i < insert_count; i++) {
                // 11(trail)/time/degree/delta/prev_curv/next_curv
                chart.pushBackNote({11, round(chart.getNote(prev_trail_pos).getTime() + time_gap * i), round(Chart::normalizeAngle(prev_degree + degree_gap * i)), 0, 0, 0});
            }
            prev_trail_pos = i;
            prev_degree = next_degree;
        }
    }

    showCompleteAndSave();
}