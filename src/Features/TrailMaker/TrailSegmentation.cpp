#include "MainProcess.h"

void MainProcess::trailSegmentation() {

    std::string command;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;
    double gap = 60;

    command = InputScreen::inputArgument(chart, "Trail Segmentation",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        gap, "gap (_th note)", "1-128");
    if (command == "x") {
        return;
    }

    double now = start, bpm = chart.getBPM(chart.findBPMPosByTime(start, 1)).getValue(BPMAttr::bpm);
    std::deque<std::size_t> pos_list;
    double trail_node_time_1 = 0, trail_degree_1 = 0;
    double trail_node_time_2 = 0, trail_degree_2 = 0;
    double c1 = 0, c2 = 0, d = 0, d1 = 0, d2 = 0;
    double result = 0;
    bool even = true;

    auto getTrailOrRotateDegree = [&] (std::size_t pos) {
        if (chart.getNote(pos).getType() == NoteType::Trail) {
            return chart.getNote(pos).getValue(NoteAttr::degree);
        }
        else if (chart.getNote(pos).getType() == NoteType::Rotate) {
            return chart.getNote(pos).getValue(NoteAttr::degree) + chart.getNote(pos).getValue(NoteAttr::delta);
        }
        else {
            std::cerr << "Error: Get NoteType \"" << chart.getNote(pos).getIntType() << "\" while using getTrailOrRotateDegree!" << std::endl;
            return static_cast<double>(-1);
        }
    };

    while (now < end) {   
        trail_node_time_1 = chart.findTrailNodeTimeByTime(now, 1);
        trail_node_time_2 = chart.findTrailNodeTimeByTime(now, 2);
        
        pos_list.clear();
        pos_list.push_back(chart.findSpecificNotePosByTime(trail_node_time_1, NoteType::Trail, 1));
        pos_list.push_back(chart.findSpecificNotePosByTime(trail_node_time_2, NoteType::Trail, 2));
        pos_list.push_back(chart.findSpecificNotePosByTime(trail_node_time_1, NoteType::Rotate, 1));
        pos_list.push_back(chart.findSpecificNotePosByTime(trail_node_time_2, NoteType::Rotate, 2));

        std::sort(pos_list.begin(), pos_list.end());
        pos_list.erase(std::unique(pos_list.begin(), pos_list.end()), pos_list.end());
        pos_list.erase(std::remove(pos_list.begin(), pos_list.end(), -1), pos_list.end());

        if (pos_list.size() == 0) {
            result = 90;
        }

        std::size_t i = 0;
        std::cout << "now: " << now << std::endl;
        for (std::size_t j = 0; j < pos_list.size(); j++) {
            std::cout << "pos_list[" << j << "]: " << chart.getNote(pos_list.at(j)).getTime() << std::endl;
        }

        for (i; i < pos_list.size(); i++) {
            if (now < chart.getNote(pos_list.at(i)).getTime()) {
                if (i == 0) {
                    result = chart.getNote(pos_list.front()).getValue(NoteAttr::degree);
                }
                else {
                    trail_degree_1 = getTrailOrRotateDegree(pos_list.at(i - 1));
                    trail_degree_2 = getTrailOrRotateDegree(pos_list.at(i));
                    c1 = chart.getNote(pos_list.at(i - 1)).getValue(NoteAttr::next_curv);
                    c2 = chart.getNote(pos_list.at(i)).getValue(NoteAttr::prev_curv);
                    d = chart.findDistanceByTime(now);
                    d1 = chart.findDistanceByTime(chart.getNote(pos_list.at(i - 1)).getTime());
                    d2 = chart.findDistanceByTime(chart.getNote(pos_list.at(i)).getTime());
                    result = Chart::getYfromX(trail_degree_1, trail_degree_2, c1/100, c2/100, (d - d1) / (d2 - d1));
                }
                break;
            }
            else if (now == chart.getNote(pos_list.at(i)).getTime()) {
                result = getTrailOrRotateDegree(pos_list.at(i));
                break;
            }
            if (i == pos_list.size() - 1) {
                result = getTrailOrRotateDegree(pos_list.back());
                break;
            }
        }
        std::cout << "result: " << result << std::endl;
        if (abs(now - chart.getNote(pos_list.at(i)).getTime()) >= 3) {
            chart.pushBackNote({ 11, round(now), result, 0, 0, 0 });
        }

        //std::cout << "now: " << now << " trail_time_1: " << chart.getNote(pos_list.at(i - 1)).getTime() << " trail_time_2: " << chart.getNote(pos_list.at(i)).getTime() << " trail_degree_1: " << trail_degree_1 << " trail_degree_2: " << trail_degree_2 << " result: " << result << std::endl;
        now += 60000 / bpm / gap * 4;
    }

    showCompleteAndSave();

}