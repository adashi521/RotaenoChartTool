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

    // command
    // dextroy [IV]_6.gwc2
    /*
    dx 10421 11195 16
    dx 8766 9594 16
    dx 7111 7939 16
    dx 3556 6222 16
    dx 0 2667 16
    */
    /*
    dx 93314 94345 16
    */
    //std::vector<double> dis = { 11198, 11199, 11202, 11203, 11206, 11207 };
    std::vector<double> dis = { 94718, 94719, 94765, 94766, 94812, 94813 };
    static std::vector<double> vec(dis.size());
    static int run_count = 0;
    std::fill(vec.begin(), vec.end(), 1);

    auto generateUniformRandomInteger = [](int last_choise) -> int {
        double prob_save = 0; 
        if (last_choise >= 0 && last_choise < vec.size()) {
            prob_save = vec.at(last_choise);
            vec.at(last_choise) = 0;
        }
        double total_weight = std::accumulate(vec.begin(), vec.end(), 0.0);
        int output = vec.size() - 1;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0, total_weight);

        double rand_num = dist(gen);
        double cum_sum = 0;
        for (int i = 0; i < vec.size(); ++i) {
            cum_sum += vec.at(i);
            if (rand_num <= cum_sum) {
                output = i;
                break;
            }
        }
        if (last_choise >= 0 && last_choise < vec.size()) {
            vec.at(last_choise) = prob_save;
        }
        vec.at(output) /= 2;
        return output;
    };

    auto generateUniformRandomDouble = [](double lower, double upper) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(lower, upper);
        return dist(gen);
    };
    
    for (std::size_t i = 0; i < dis.size(); i++) {
        dis[i] = chart.findDistanceByTime(dis[i]);
    }

    int last_choice = generateUniformRandomInteger(-1);
    double last_bias = generateUniformRandomDouble(100, 250);
    double end_dis = chart.findDistanceByTime(end);
    double now_time = end, now_bpm = chart.getBPM(chart.findBPMPosByTime(end, 1)).getValue(BPMAttr::bpm);
    double now_dis = end_dis - dis[last_choice] + last_bias;
    chart.pushBackSpeed({ round(now_time - 1), now_dis, 0 });
    //chart.pushBackSpeed({ round(now_time), 0, 0 });
    now_dis = dis[last_choice] - last_bias;
    

    while (now_time >= start) {
        now_bpm = chart.getBPM(chart.findBPMPosByTime(now_time, 1)).getValue(BPMAttr::bpm);
        now_time -= 60000 / now_bpm / th * 4;
        last_choice = generateUniformRandomInteger(last_choice);
        last_bias = generateUniformRandomDouble(100, 250);

        if (abs(now_time - start) <= 10) {
            chart.pushBackSpeed({ round(now_time + 2), 0, 0 });
            chart.pushBackSpeed({ round(now_time + 1), now_dis - end_dis, 0 });
            chart.pushBackSpeed({ round(now_time), 1, 1 });
            break;
        }
        else {
            chart.pushBackSpeed({ round(now_time - 1), 0, 0 });
            chart.pushBackSpeed({ round(now_time - 2), now_dis - dis[last_choice] + last_bias, 0 });
            now_dis = dis[last_choice] - last_bias;
        }
    }
    run_count++;

    showCompleteAndSave();
}