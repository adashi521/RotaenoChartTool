#include "MainProcess.h"

void MainProcess::foodMaker2025() {
    std::string command;
    double start = -1, end = -1, degree = 0;

    command = InputScreen::inputArgument(chart, "Food Maker 2025",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-"
        );
    if (command == "x") {
        return;
    }

    // hamburger degree: 5
    degree = 5;
    const std::size_t start_pos = chart.findNotePosByTime(start), end_pos = chart.findNotePosByTime(end, 1);
    for (std::size_t i = start_pos; i <= end_pos; i++) {
        if (chart.getNote(i).getType() != NoteType::Tap) {
            continue;
        }
        chart.pushBackNote({1, chart.getNote(i).getTime() + 25, chart.getNote(i).getValue(NoteAttr::degree) + degree});
        chart.pushBackNote({1, chart.getNote(i).getTime() + 25, chart.getNote(i).getValue(NoteAttr::degree) - degree});
        chart.pushBackNote({1, chart.getNote(i).getTime() + 50, chart.getNote(i).getValue(NoteAttr::degree)});
    }


    // fries degree: 10
    degree = 10;
    double now = start, now_degree = 0;
    for (std::size_t i = start_pos; i <= end_pos; i++) {
        if (chart.getNote(i).getType() != NoteType::Slide || chart.getNote(i).getValue(NoteAttr::slide_type) != 3 || chart.getNote(i).getValue(NoteAttr::amount) != 1) {
            continue;
        }
        now = chart.getNote(i).getTime();
        now_degree = chart.getNote(i).getValue(NoteAttr::degree);
        chart.getNote(i).setVector({ 2, now, now_degree, 2, now_degree, 12, 3, 0, 0 });
        chart.pushBackNote({ 2, now, now_degree + degree/2, 2, now_degree + degree, 12, 3, 0, 0 });
        chart.pushBackNote({ 2, now, now_degree - degree/2, 2, now_degree - degree, 12, 3, 0, 0 });
    }

    // nugget degree: 5
    degree = 3;
    now = start, now_degree = 0;
    for (std::size_t i = start_pos; i <= end_pos; i++) {
        if (chart.getNote(i).getType() != NoteType::Flick) {
            continue;
        }
        now = chart.getNote(i).getTime();
        now_degree = chart.getNote(i).getValue(NoteAttr::degree);
        chart.getNote(i).setVector({1, now + 25, now_degree - degree});
        chart.pushBackNote({2, now, now_degree - degree, 3, now_degree - degree, 1, 1, 0, 0});
        chart.pushBackNote({2, now, now_degree + degree, 3, now_degree + degree, 1, 1, 0, 0});
        chart.pushBackNote({1, now + 25, now_degree + degree});
        chart.pushBackNote({1, now + 50, now_degree + degree});
        chart.pushBackNote({1, now + 50, now_degree - degree});
    }

    // hotdog degree: 25
    degree = 20;
    double bpm = 0, next_degree = 0, next_time_gap = 0;
    int status = 0;
    std::vector<double> catch_pair_right, catch_pair_left;

    for (std::size_t i = start_pos; i <= end_pos; i++) {
        if (chart.getNote(i).getType() == NoteType::Catch) {
            if (chart.getNote(i).getValue(NoteAttr::degree) == 0) {
                catch_pair_right.push_back(chart.getNote(i).getTime());
            }
            else {
                catch_pair_left.push_back(chart.getNote(i).getTime());
            }
        }
    }
    if (catch_pair_right.size() % 2 == 1) {
        catch_pair_right.pop_back();
    }
    std::vector<double> catch_pair = catch_pair_right;
    std::size_t catch_pair_right_size = catch_pair_right.size();
    catch_pair.insert(catch_pair.end(), catch_pair_left.begin(), catch_pair_left.end());

    double end_seg = 0, snap = 16, right = 0;
    for (std::size_t i = 0; i + 1 < catch_pair.size(); i += 2) {
        if (i < catch_pair_right_size) {
            right = 0;
        }
        else {
            right = 1;
        }
        now = catch_pair.at(i);
        end_seg = catch_pair.at(i + 1);
        now_degree = 0;
        while (now < end_seg - 1) {
            bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
            now_degree = chart.findDegreeByTime(now);
            next_time_gap = 60000 / bpm * 4 / snap;
            next_degree = chart.findDegreeByTime(now + next_time_gap);
            
            now_degree += right * 180;
            next_degree += right * 180;
            if (status == 0) {
                chart.pushBackNote({ 2, now, now_degree, 2, next_degree + degree * (right*2-1), 12, 3, 0, 50 });
            }
            else if (status == 1) {
                chart.pushBackNote({ 2, now, now_degree + degree * (right*2-1), 2, next_degree, 12, 3, 50, 0 });
            }
            else if (status == 2) {
                chart.pushBackNote({ 2, now, now_degree, 2, next_degree - degree * (right*2-1), 12, 3, 0, 50 });
            }
            else if (status == 3) {
                chart.pushBackNote({ 2, now, now_degree - degree * (right*2-1), 2, next_degree, 12, 3, 50, 0 });
            }
            
            status = (status + 1) % 4;
            now += next_time_gap;
        }

        next_time_gap = 60000 / bpm * 4 / snap / 2;
        now = catch_pair.at(i) + next_time_gap;
        while (now < end_seg - 1) {
            bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
            chart.pushBackNote({ 5, now, right });
            now += next_time_gap;
        }
    }

    showCompleteAndSave();
}