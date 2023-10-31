#include "MainProcess.h"

void MainProcess::RrharilSpeed() {
    std::string command;
    double step;
    command = InputScreen::inputArgument(chart, "Rrharil Speed Maker",
        step, "step (0,1)", "0 1");
    if (command == "x") {
        return;
    }

    const std::deque<double> red_rotate_timing_bad = { 8842, 51789, 56842, 58737, 100737};
    const std::deque<double> red_rotate_timing_good{ 18947, 49263, 54789, 107368, 112421, 118737, 121263, 107368 };
    const std::deque<double> bule_rotate_timing_bad{ 59684 , 61579 , 109579, 113053};
    const std::deque<double> bule_rotate_timing_good{ 29053, 50526, 54316, 64421, 108632, 111158, 117474, 117474, 108632 };

    auto gaussian = [](double x, double factor, double div, double middle) {
        return factor* exp(-1 / div * pow((x - middle), 2));
    };

    auto sigmoid = [](double x, double height, double factor, double middle) {
        return height / (1 + exp(factor * (x - middle)));
    };

    auto getStartDistanceBias_old = [&](double start_time, double start_count, double factor, double div) {
        double dis = 0;
        double now_bpm, now_time = start_time, target_time, now_speed;
        const double middle = (start_count - 1) / 2;
        for (double i = start_count - 2; i >= 0; i--) {
            now_bpm = chart.getBPM(chart.findBPMPosByTime(now_time)).getValue(BPMAttr::bpm);
            target_time = now_time - 60000 / now_bpm / 2;
            now_speed = gaussian(i, factor, div, middle);
            dis += (round(now_time) - round(target_time)) * now_speed;
        }
        return dis;
    };

    if (step == 0) {
        // rrharil[IV]_5.gwc2
        std::random_device rd;
        std::mt19937 g(rd());

        std::deque<double> red_rotate_timing(red_rotate_timing_bad.begin(), red_rotate_timing_bad.end());
        red_rotate_timing.insert(red_rotate_timing.end(), red_rotate_timing_good.begin(), red_rotate_timing_good.end());
        std::shuffle(red_rotate_timing.begin(), red_rotate_timing.end(), g);

        const double factor = 1, div = 3.2;
        double now_time = 5684;
        double now_bpm;
        double now_speed = 0;   
        double s = static_cast<double>(red_rotate_timing.size());
        double now_distance = chart.findDistanceByTime(6316), target_distance;

        for (std::size_t i = 0; i < red_rotate_timing.size(); i++) {
            now_bpm = chart.getBPM(chart.findBPMPosByTime(now_time)).getValue(BPMAttr::bpm);
            target_distance = chart.findDistanceByTime(red_rotate_timing.at(i));

            now_speed = gaussian(static_cast<double>(i), factor, div, (s - 1) / 2);
            //now_speed = sigmoid(s - static_cast<double>(i), 316.0 / 948.0, 1.0, (s - 1) / 2);
            if (i != 0) {
                chart.pushBackSpeed({ now_time, -now_speed, 0 });
            }
            chart.pushBackSpeed({ now_time - 1, now_distance - target_distance, 0 });
            now_distance = target_distance;
            now_time -= 60000 / now_bpm / 2;
        }
    }
    else if(step == 1) {
        // rrharil[IV]_6.gwc2
        std::random_device rd;
        std::mt19937 g(rd());

        std::deque<double> bule_rotate_timing(bule_rotate_timing_bad.begin(), bule_rotate_timing_bad.end());
        bule_rotate_timing.insert(bule_rotate_timing.end(), bule_rotate_timing_good.begin(), bule_rotate_timing_good.end());
        std::shuffle(bule_rotate_timing.begin(), bule_rotate_timing.end(), g);

        const double factor = 1, div = 3.2;
        const double bias_time = 3474;
        double now_time = 3158;
        double now_bpm;
        double now_speed = 0;
        double s = static_cast<double>(bule_rotate_timing.size());
        double now_distance = chart.findDistanceByTime(now_time) + 553, target_distance;


        for (std::size_t i = 0; i < bule_rotate_timing.size() - 1; i++) {
            now_bpm = chart.getBPM(chart.findBPMPosByTime(now_time)).getValue(BPMAttr::bpm);
            target_distance = chart.findDistanceByTime(bule_rotate_timing.at(i));

            now_speed = gaussian(static_cast<double>(i), factor, div, (s - 1) / 2);
            if (i == 0) {
                chart.pushBackSpeed({ bias_time, now_distance - target_distance, 0 });
                chart.pushBackSpeed({ bias_time + 1, chart.getSpeed(chart.findSpeedPosByTime(bias_time - 1)).getValue(SpeedAttr::speed), 0 });
            }
            else {
                chart.pushBackSpeed({ now_time, -now_speed, 0 });
                chart.pushBackSpeed({ now_time - 1, now_distance - target_distance, 0 });
            }
            now_distance = target_distance;
            now_time -= 60000 / now_bpm / 2;
        }
    }

    showCompleteAndSave();
}