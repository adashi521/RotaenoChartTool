#include "MainProcess.h"

void MainProcess::foodMakerSingle2025() {
    std::string command;
    double start = -1, end = -1, degree = 0, mode = 0, snap = 0, right = 0;

    command = InputScreen::inputArgument(chart, "Food Maker Single 2025",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        mode, "mode(0:hamburger, 1:fries, 2:hotdog, 3:hotdog_cv)", "0 1 2 3",
        degree, "degree", "0-",
        snap, "snap", "1-",
        right, "right", "0 1"
        );
    if (command == "x") {
        return;
    }

    if (mode == 0) {
        // degree: 5
        std::size_t start_pos, end_pos;
        start_pos = chart.findNotePosByTime(start);
        end_pos = chart.findNotePosByTime(end, 1);
        double middle_pos = 0;
        for (std::size_t i = start_pos; i <= end_pos; i++) {
            if (chart.getNote(i).getType() != NoteType::Tap) {
                continue;
            }
            chart.pushBackNote({1, chart.getNote(i).getTime() + 25, chart.getNote(i).getValue(NoteAttr::degree) + degree});
            chart.pushBackNote({1, chart.getNote(i).getTime() + 25, chart.getNote(i).getValue(NoteAttr::degree) - degree});
            chart.pushBackNote({1, chart.getNote(i).getTime() + 50, chart.getNote(i).getValue(NoteAttr::degree)});
        }
    }
    else if (mode == 1) {
        // degree: 15
        std::size_t start_pos, end_pos;
        start_pos = chart.findNotePosByTime(start);
        end_pos = chart.findNotePosByTime(end, 1);
        double now = start, now_degree = 0;
        for (std::size_t i = start_pos; i <= end_pos; i++) {
            if (chart.getNote(i).getType() != NoteType::Slide || chart.getNote(i).getValue(NoteAttr::slide_type) != 3 || chart.getNote(i).getValue(NoteAttr::amount) != 1) {
                continue;
            }
            now = chart.getNote(i).getTime();
            now_degree = chart.getNote(i).getValue(NoteAttr::degree);
            chart.getNote(i).setVector({ 2, now, now_degree, 2, now_degree, 12, 3, 0, 0 });
            chart.pushBackNote({ 2, now, now_degree, 2, now_degree + degree, 12, 3, 0, 0 });
            chart.pushBackNote({ 2, now, now_degree, 2, now_degree - degree, 12, 3, 0, 0 });
        }
    }
    else if (mode == 2) {
        // degree: 25
        double now = start, bpm = 0, now_degree = 0, next_degree = 0, next_time = now;
        int status = 0;
        while (now < end - 1) {
            bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
            now_degree = chart.findDegreeByTime(now);
            next_time = now + 60000 / bpm * 4 / snap * 2;
            next_degree = chart.findDegreeByTime(next_time);
            
            now_degree += right * 180;
            next_degree += right * 180;
            if (status == 0) {
                chart.pushBackNote({ 2, now, now_degree + degree * (right*2-1), 1, next_degree - degree * (right*2-1), 16, (32/snap)*4, 50, 50 });
            }
            else if (status == 1) {
                chart.pushBackNote({ 2, now, now_degree - degree * (right*2-1), 2, next_degree + degree * (right*2-1), 16, (32/snap)*4, 50, 50 });
            }
            
            status = (status + 1) % 2;
            now = next_time;
        }

        now = start;
        while (now < end - 1) {
            bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
            chart.pushBackNote({ 5, now, right });
            next_time = now + 60000 / bpm * 4 / snap / 2;
            now = next_time;
        }
    }
    else if (mode == 3) {
        // degree: 25
        double now = start, bpm = 0, now_degree = 0, next_degree = 0, next_time = now;
        int status = 0;
        while (now < end - 1) {
            bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
            now_degree = chart.findDegreeByTime(now);
            if (status % 2 == 0) {
                next_time = now + 60000 / bpm * 4 / snap;
            }
            else {
                next_time = now + 60000 / bpm * 4 / snap * 3;
            }
            next_degree = chart.findDegreeByTime(next_time);
            
            now_degree += right * 180;
            next_degree += right * 180;
            if (status == 0) {
                chart.pushBackNote({ 2, now, now_degree + degree * (right*2-1), 1, next_degree - degree * (right*2-1), 32, (32/snap)*4, 0, 0 });
            }
            else if (status == 1) {
                chart.pushBackNote({ 2, now, now_degree - degree * (right*2-1), 3, next_degree - degree * (right*2-1), 16, (32/snap)*6, 0, 0 });
            }
            else if (status == 2) {
                chart.pushBackNote({ 2, now, now_degree - degree * (right*2-1), 1, next_degree + degree * (right*2-1), 32, (32/snap)*4, 0, 0 });
            }
            else if (status == 3) {
                chart.pushBackNote({ 2, now, now_degree + degree * (right*2-1), 3, next_degree + degree * (right*2-1), 16, (32/snap)*6, 0, 0 });
            }
            
            status = (status + 1) % 4;
            now = next_time;
        }

        now = start;
        while (now < end - 1) {
            bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
            chart.pushBackNote({ 5, now, right });
            next_time = now + 60000 / bpm * 4 / snap;
            now = next_time;
        }
    }

    showCompleteAndSave();
}