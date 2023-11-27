#include "MainProcess.h"

void MainProcess::RrharilSpeed() {
    std::string command;
    double step;
    command = InputScreen::inputArgument(chart, "Rrharil Speed Maker",
        step, "step (0,1,2)", "0 1 2");
    if (command == "x") {
        return;
    }

    // const std::deque<double> red_rotate_timing_bad = { 8842, 51789, 56842, 58737, 100737};
    // const std::deque<double> red_rotate_timing_good{ 18947, 49263, 54789, 107368, 112421, 112421, 121263, 107368 };
    // const std::deque<double> blue_rotate_timing_bad{ 59684 , 61579 , 109579, 113053};
    // const std::deque<double> blue_rotate_timing_good{ 29053, 50526, 54316, 64421, 108632, 111158, 117474, 117474, 108632 };

    std::deque<double> red_rotate_timing = {54789, 112421, 49263, 107368, 112421, 121263, 56842, 58737, 8842, 107368, 51789, 18947, 100737};
    std::deque<double> blue_rotate_timing = {29053, 117474, 108632, 109579, 59684, 61579, 113053, 64421, 108632, 117474, 50526, 54316, 111158};


    auto gaussian = [](double x, double var, double mean) {
        return exp(-1 / var * pow((x - mean), 2));
    };

    auto sigmoid = [](double x, double height, double factor, double mean) {
        return height / (1 + exp(factor * (x - mean)));
    };

    auto getStartDistanceBias_old = [&](double start_time, double start_count, double factor, double var) {
        double dis = 0;
        double now_bpm, now_time = start_time, target_time, now_speed;
        const double mean = (start_count - 1) / 2;
        for (double i = start_count - 2; i >= 0; i--) {
            now_bpm = chart.getBPM(chart.findBPMPosByTime(now_time)).getValue(BPMAttr::bpm);
            target_time = now_time - 60000 / now_bpm / 2;
            now_speed = factor * gaussian(i, var, mean);
            dis += (round(now_time) - round(target_time)) * now_speed;
        }
        return dis;
    };

    if (step == 0) {
        // rrharil[IV]_5.gwc2
        std::random_device rd;
        std::mt19937 g(rd());

        // std::deque<double> red_rotate_timing(red_rotate_timing_bad.begin(), red_rotate_timing_bad.end());
        // red_rotate_timing.insert(red_rotate_timing.end(), red_rotate_timing_good.begin(), red_rotate_timing_good.end());
        // std::shuffle(red_rotate_timing.begin(), red_rotate_timing.end(), g);
        std::reverse(red_rotate_timing.begin(), red_rotate_timing.end());
        
        const double factor = 1, var = 3.2;
        double now_time = 5684;
        double now_bpm;
        double now_speed = 0;   
        double s = static_cast<double>(red_rotate_timing.size());
        double now_distance = chart.findDistanceByTime(6316), target_distance;

        for (std::size_t i = 0; i < red_rotate_timing.size() - 1; i++) {
            now_bpm = chart.getBPM(chart.findBPMPosByTime(now_time)).getValue(BPMAttr::bpm);
            target_distance = chart.findDistanceByTime(red_rotate_timing.at(i));
            std::cout << red_rotate_timing.at(i) << std::endl;
            if(red_rotate_timing.at(i) > 84632) {
                target_distance -= 25;
            }

            now_speed = factor * gaussian(static_cast<double>(i), var, (s - 1) / 2);
            //now_speed = sigmoid(s - static_cast<double>(i), 316.0 / 948.0, 1.0, (s - 1) / 2);
            if (i != 0) {
                chart.pushBackSpeed({ now_time, -now_speed, 0 });
            }
            chart.pushBackSpeed({ now_time - 1, now_distance - target_distance, 0 });
            now_distance = target_distance;
            now_time -= 60000 / now_bpm / 2;
        }
        target_distance = chart.findDistanceByTime(red_rotate_timing.back());
        if(red_rotate_timing.back() > 84632) {
            target_distance -= 1000;
        }
        chart.pushBackSpeed({ now_time, 0, 1 });
        now_time -= 60000 / now_bpm;
        chart.pushBackSpeed({ now_time - 1, -5, 1 });
        chart.pushBackSpeed({ now_time, now_distance - target_distance, 0 });
        chart.pushBackSpeed({ now_time + 1, -5, 0 });
        now_time -= 60000 / now_bpm;
        chart.pushBackSpeed({ now_time + 1, 0, 0 });
    }
    else if(step == 1) {
        // rrharil[IV]_6.gwc2
        std::random_device rd;
        std::mt19937 g(rd());

        // std::deque<double> blue_rotate_timing(blue_rotate_timing_bad.begin(), blue_rotate_timing_bad.end());
        // blue_rotate_timing.insert(blue_rotate_timing.end(), blue_rotate_timing_good.begin(), blue_rotate_timing_good.end());
        // std::shuffle(blue_rotate_timing.begin(), blue_rotate_timing.end(), g);
        std::reverse(blue_rotate_timing.begin(), blue_rotate_timing.end()); 

        const double factor = 1, var = 3.2;
        const double init_time = 3473;
        double now_time = 3158;
        double now_bpm;
        double now_speed = 0;
        double s = static_cast<double>(blue_rotate_timing.size());
        double now_distance = chart.findDistanceByTime(now_time) + 553, target_distance;


        for (std::size_t i = 0; i < blue_rotate_timing.size() - 1; i++) {
            now_bpm = chart.getBPM(chart.findBPMPosByTime(now_time)).getValue(BPMAttr::bpm);
            target_distance = chart.findDistanceByTime(blue_rotate_timing.at(i));
            std::cout << blue_rotate_timing.at(i) << std::endl;
            if(blue_rotate_timing.at(i) > 84632) {
                target_distance -= 25;
            }

            now_speed = factor * gaussian(static_cast<double>(i), var, (s - 1) / 2);
            if (i == 0) {
                chart.getSpeed(chart.findSpeedPosByTime(init_time)).increment(SpeedAttr::speed, now_distance - target_distance);
                //chart.pushBackSpeed({ init_time, now_distance - target_distance, 0 });
            }
            else {
                chart.pushBackSpeed({ now_time, -now_speed, 0 });
                chart.pushBackSpeed({ now_time - 1, now_distance - target_distance, 0 });
            }
            now_distance = target_distance;
            now_time -= 60000 / now_bpm / 2;
        }
    }
    else if (step == 2) {
        double height = 3;
        double var2 = pow(323.37, 2);
        double var3 = pow(399.86, 2);
        
        
        const std::vector<double> pos_lst{ 0 , 136 , 208 , 259 , 301 , 335 , 366 , 393 , 419 , 442 , 464 , 484 , 504 , 523 , 541 , 558 , 575 , 592 , 608 , 624 , 640 , 656 , 671 , 687 , 702 , 717 , 732 , 748 , 763 , 779 , 795 , 811 , 827 , 844 , 861 , 878 , 896 , 915 , 935 , 956 , 977 , 1001 , 1026 , 1053 , 1084 , 1119 , 1160 , 1211 , 1283 , 1418 };
        std::vector<double> speed_lst_1;
        std::vector<double> speed_lst_2;
        const int mean = floor(pos_lst.back()/2);

        for (std::size_t i = 0; i < pos_lst.size(); ++i) {
            speed_lst_1.push_back(height * gaussian(pos_lst[i], var2, mean));
        }

        for (std::size_t i = 0; i < pos_lst.size(); ++i) {
            speed_lst_2.push_back(height * gaussian(pos_lst[i], var3, mean) + 1);
        }

        const double gap = chart.findDistanceByTime(124737) - chart.findDistanceByTime(84632);
        double bias = 0;
        for (std::size_t i = pos_lst.size() - 1; i > 0; i--) {
            chart.pushBackSpeed({80848 + pos_lst[i] * 2, -0, 0});
            chart.pushBackSpeed({80848 + pos_lst[i] * 2 - 1, -(gap - bias), 0});
            bias += (pos_lst[i] - pos_lst[i-1]) * speed_lst_1[i];
            chart.pushBackSpeed({80848 + pos_lst[i] + pos_lst[i-1], 0, 0});
            chart.pushBackSpeed({80848 + pos_lst[i] + pos_lst[i-1] - 1, gap - bias, 0});
            bias += (pos_lst[i] - pos_lst[i-1]) * speed_lst_2[i];
        }
    }

    showCompleteAndSave();
}