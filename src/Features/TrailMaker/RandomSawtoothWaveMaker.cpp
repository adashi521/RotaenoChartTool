#include "MainProcess.h"

void MainProcess::randomSawtoothWaveMaker() {
    std::string command;
    std::random_device rd;
    double start = -1, end = -1, mean = 0, std = 0, start_degree = 0, wavelength = 4, follow = 0, pc = 0, nc = 0, up_down = 1;
    std::size_t start_pos, end_pos;
    double gap = 60;

    command = InputScreen::inputArgument(chart, "Random Sawtooth Wave Maker",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        follow, "follow trail (0/1)", "0 1",
        //up_down, "forced up&down (0/1)", "0 1",
        wavelength, "half wavelength (_th note)", "0-",
        mean, "mean(degree)", "-",
        std, "stddev(degree)", "0-");
    if (command == "x") {
        return;
    }

    auto generateRandomNumber = [](double mean, double stddev) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dist(mean, stddev);
        return dist(gen);
    };

    auto generateRandomBool = []() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution d(0.5);
        return d(gen);
    };

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);
    std::deque<std::size_t> trail_pos;
    for (std::size_t i = start_pos; i < chart.getNoteSize() && i <= end_pos; i++) {
        if (chart.getNote(i).getType() == NoteType::Trail) {
            trail_pos.push_back(i);
        }
    }

    double now = start, next_time_gap = 0, bpm = 0, now_degree = chart.findDegreeByTime(start), random_degree = 0;
    std::size_t segment = 0;
    bool even = generateRandomBool();
    int seed = rd();
    if (follow == 0) {
        while (round(now) <= end) {
            bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
            next_time_gap = 60000 / bpm / wavelength * 4;

            if (up_down == 0) {
                random_degree = generateRandomNumber(mean, std);
            }
            else {
                seed = rd();
                if (even) {
                    if (mean > 0) {
                        random_degree = mit::truncated_normal_a_sample(mean, std, 0, seed);
                    }
                    else {
                        random_degree = mit::truncated_normal_a_sample(0, std, 0, seed);
                    }
                }
                else {
                    if (mean < 0) {
                        random_degree = mit::truncated_normal_b_sample(mean, std, 0, seed);
                    }
                    else {
                        random_degree = mit::truncated_normal_b_sample(0, std, 0, seed);
                    }
                }
                even = even ^ 1;
            }

            segment = static_cast<std::size_t>(abs(random_degree) / 90) + 1;
            for (std::size_t i = 0; i < segment; i++) {
                if (round(now + (next_time_gap / segment * i) > end)) {
                    break;
                }
                chart.pushBackNote({ 11, round(now + (next_time_gap / segment * i)), round(now_degree + (random_degree / segment * i)), 0, pc, nc });
            }
            //std::cout << "random_degree: " << random_degree << std::endl;
            now_degree += random_degree;
            now += next_time_gap;
        }
    }
    else {
        double line_degree = 0;
        bool state = false;

        while (round(now) - 1 <= end) {
            bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
            next_time_gap = 60000 / bpm / wavelength * 2;

            if (state == false) {
                if (up_down == 0) {
                    even = generateRandomBool();
                }
                line_degree = chart.findDegreeByTime(now);
                seed = rd();
                if (even) {
                    if (mean > 0) {
                        random_degree = mit::truncated_normal_a_sample(mean, std, 0, seed);
                    }
                    else {
                        random_degree = mit::truncated_normal_a_sample(-mean, std, 0, seed);
                    }
                }
                else {
                    if (mean < 0) {
                        random_degree = mit::truncated_normal_b_sample(mean, std, 0, seed);
                    }
                    else {
                        random_degree = mit::truncated_normal_b_sample(-mean, std, 0, seed);
                    }
                }
                even = even ^ 1;
                state = true;
            }
            else {
                line_degree = chart.findDegreeByTime(now + next_time_gap);
                random_degree = line_degree - now_degree;
                state = false;
            }

            segment = static_cast<std::size_t>(abs(random_degree) / 90) + 1;
            for (std::size_t i = 0; i < segment; i++) {
                if (round(now + (next_time_gap / segment * i) > end)) {
                    break;
                }
                chart.pushBackNote({ 11, round(now + (next_time_gap / segment * i)), round(now_degree + (random_degree / segment * i)), 0, pc, nc });
            }
            //std::cout << "random_degree: " << random_degree << std::endl;
            now_degree += random_degree;
            now += next_time_gap;
        }
    }

    for (int i = static_cast<int>(trail_pos.size()) - 1; i >= 0; i--) {
        chart.deleteNote(trail_pos.at(i));
    }

    showCompleteAndSave();
}