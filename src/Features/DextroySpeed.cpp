#include "MainProcess.h"

void MainProcess::dextorySpeed() {
    std::string command;
    double start = -1, end = -1, mean = 0, std = 0, start_degree = 0, th;
    double gap = 60;

    command = InputScreen::inputArgument(chart, "Dextroy Speed Maker",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        th, "_th note", "0-");
    if (command == "x") {
        return;
    }

    auto generateUniformRandomInteger = [](int lower, int upper, int exclude = -1) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(lower, upper - (exclude != -1));

        int result = dist(gen);

        if (result == exclude) {
            result = upper;
        }
        return result;
    };

    auto generateUniformRandomDouble = [](double lower, double upper) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(lower, upper);
        return dist(gen);
    };


    double start_dis = chart.findDistanceByTime(start), end_dis = chart.findDistanceByTime(end);
    double dis[6] = { 11970, 11971, 45005, 45006, 45732, 45733 };
    for (std::size_t i = 0; i < 6; i++) {
        dis[i] = chart.findDistanceByTime(dis[i]);
    }

    int last_choice = generateUniformRandomInteger(0, 5);
    double last_bias = 0;
    double last_dis = dis[last_choice] - last_bias;
    double speed_sum = end_dis - last_dis - 9800 + last_bias;
    double now_time = end, now_bpm = chart.getBPM(chart.findBPMPosByTime(end, 1)).getValue(BPMAttr::bpm);
    chart.pushBackSpeed({ now_time - 2, speed_sum, 0.0});
    while (now_time >= start) {
        now_bpm = chart.getBPM(chart.findBPMPosByTime(now_time, 1)).getValue(BPMAttr::bpm);
        now_time -= 60000 / now_bpm / th * 4;
        last_choice = generateUniformRandomInteger(0, 5, last_choice);
        last_choice = generateUniformRandomInteger(0, 5, last_choice);
        if (abs(now_time - start) <= 10) {
            chart.pushBackSpeed({ round(now_time + 2), 0, 0 });
            chart.pushBackSpeed({ round(now_time + 1), -speed_sum + end - start + 600, 0 });
            break;
        }
        else {
            last_bias = 0;
            chart.pushBackSpeed({ round(now_time - 1), 0, 0 });
            chart.pushBackSpeed({ round(now_time - 2), last_dis - dis[last_choice] + last_bias, 0 });
            speed_sum += last_dis - dis[last_choice] + last_bias;
            last_dis = dis[last_choice] - last_bias;
        }
    }

    showCompleteAndSave();
}