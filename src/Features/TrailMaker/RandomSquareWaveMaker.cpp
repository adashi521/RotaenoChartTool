#include "MainProcess.h"

void MainProcess::randomSquareWaveMaker() {
    std::string command;
    std::random_device rd;
    double start = -1, end = -1, mean = 0, std = 0, start_degree = 0, wavelength = 4, follow = 0, up_down = 0;
    std::size_t start_pos, end_pos;
    double gap = 60;

    command = InputScreen::inputArgument(chart, "Random Square Wave Maker",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        follow, "follow trail (0/1)", "0 1",
        up_down, "forced up&down (0/1)", "0 1",
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

    auto normalCDF = [](double value, double mean, double std) {
        double z = (value - mean) / std;

        double t = 1.0 / (1.0 + 0.2316419 * std::abs(z));
        double ans = 1 - (1 / (sqrt(2 * M_PI)) * exp(-0.5 * z * z)) * (0.31938153 * t - 0.356563782 * pow(t, 2) +
                                                                       1.781477937 * pow(t, 3) - 1.821255978 * pow(t, 4) +
                                                                       1.330274429 * pow(t, 5));
        if (z < 0)
        {
            ans = 1.0 - ans;
        }

        return ans;
    };

    auto less90Degree = [](double d1, double d2) {
        d1 = fmod(d1, 360);
        d2 = fmod(d2, 360);
        if (d1 < 0) {
            d1 += 360;
        }
        if (d2 < 0) {
            d2 += 360;
        }
        if (abs(d1 - d2) < 90 || abs(d1 - d2) - 360 > -90) {
            return true;
        }
        else {
            return false;
        }
    };

    auto old_truncated_normal = [](double mean, double stddev) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dist(abs(mean), stddev);
        while (true) {
            double number = dist(gen);
            if (number > 0) {
                return number;
            }
        }
    };

    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);
    std::deque<std::size_t> trail_pos;
    for (std::size_t i = start_pos; i < chart.getNoteSize() && i <= end_pos; i++) {
        if (chart.getNote(i).getType() == NoteType::Trail) {
            trail_pos.push_back(i);
        }
    }

    double now = start, bpm = 0, now_degree = chart.findDegreeByTime(start), random_degree = 0;
    bool even = generateRandomBool();
    int seed = rd();
    if (follow == 0) {
        while (now <= end) {
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
            chart.pushBackNote({ 11, round(now), round(now_degree), round(random_degree), 0, 0 });
            bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
            now += 60000 / bpm / wavelength * 4;
            now_degree += random_degree;
        }
    }
    else if (follow == 1) {
        // if (up_down == 0) {
        //     double line_degree;
        //     while (now <= end) {
        //         line_degree = chart.findDegreeByTime(now);
        //         random_degree = generateRandomNumber(mean, std);
        //         chart.pushBackNote({ 11, round(now), round(now_degree), round(random_degree - now_degree + line_degree), 0, 0 });
        //         bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
        //         even = even ^ 1;
        //         now += 60000 / bpm / wavelength * 4;
        //         now_degree = line_degree + random_degree;
        //     }
        // }
        double line_degree = 0;
        double next_fix_degree = 0;
        while (now <= end) {
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
            next_fix_degree = abs(now_degree - line_degree) - 80;
            if (next_fix_degree < 0) {
                next_fix_degree = 0;
            }
            if (!even) {
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
            chart.pushBackNote({ 11, round(now), round(line_degree), round(random_degree), 0, 0 });

            
            
            now_degree = line_degree + random_degree;
            bpm = chart.getBPM(chart.findBPMPosByTime(now, 1)).getValue(BPMAttr::bpm);
            even = even ^ 1;
            now += 60000 / bpm / wavelength * 4;
        }
    }

    for (int i = static_cast<int>(trail_pos.size()) - 1; i >= 0; i--) {
        chart.deleteNote(trail_pos.at(i));
    }

    showCompleteAndSave();
}