#include "MainProcess.h"

void MainProcess::randomBombGenerator() {
    std::string command;
    std::random_device rd;
    double start = -1, end = -1;
    std::size_t start_pos, end_pos;
    double note_time_gap = 0, space = 0, mean = 0, sd = 1;
    const std::size_t pieces = 360;

    command = InputScreen::inputArgument(chart, "Random Bomb Generator",
        start, "start time(ms)", "- all",
        end, "end time(ms)", "-",
        note_time_gap, "time gap (_th note)", "1-512",
        space, "free space (+-degree)", "0-89",
        mean, "bomb mean (number)", "1-",
        sd, "bomb std", "0-"
        );
    if (command == "x") {
        return;
    }

    auto bankers_round = [](double value) {
        double rounded = std::round(value);
        if (std::fabs(value - rounded) == 0.5) {
            if (static_cast<int>(rounded) % 2 != 0) {
                rounded = std::floor(value);
            }
        }
        return static_cast<int>(rounded);
    };

    auto gap_trans = [=](double a) {
        return bankers_round(a / 360.0 * pieces);
    };

    auto select_event = [](const std::vector<double>& probabilities) {
        // Normalize the probabilities
        double sum = std::accumulate(probabilities.begin(), probabilities.end(), 0.0);
        std::vector<double> normalized_probabilities(probabilities.size());
        std::transform(probabilities.begin(), probabilities.end(), normalized_probabilities.begin(),
                    [sum](double p) { return p / sum; });

        // Create a distribution based on the normalized probabilities
        std::random_device rd;
        std::mt19937 gen(rd());
        std::discrete_distribution<> dist(normalized_probabilities.begin(), normalized_probabilities.end());

        // Return the index of the selected event
        return dist(gen);
    };

    auto place_bomb = [=](const std::vector<double>& probabilities, const std::size_t pos) {
        std::vector<double> new_probabilities = probabilities;
        // Set the probability to probabilities[i] * (1 - 1 / (1 + (a * (i - b))^2))
        double a = 0.2 * pieces / 360.0;
        for (std::size_t i = 0; i < new_probabilities.size(); i++) {
            int diff = static_cast<int>(i) - static_cast<int>(pos);
            new_probabilities[i] = probabilities[i] * (1 - 1 / (1 + std::pow(a * diff, 2)));
        }
        return new_probabilities;
    };


    start_pos = chart.findNotePosByTime(start);
    end_pos = chart.findNotePosByTime(end, 1);

    
    int seed = rd();
    double now = start, bpm = chart.getBPM(chart.findBPMPosByTime(start, 1)).getValue(BPMAttr::bpm);
    double line_degree = chart.findDegreeByTime(now), now_degree = line_degree;
    double time_gap = 60000 / bpm / note_time_gap * 4;
    std::size_t bomb_pos = 0, bomb_count = 0;
    std::vector<double> probabilities(pieces);
    bool checked = false;

    while (now <= end) {
        // std::cout << "now: " << now << std::endl;
        // std::cout << "Press Enter to continue...";
        // std::cin.get();
        
        bpm = chart.getBPM(chart.findBPMPosByTime(start, 1)).getValue(BPMAttr::bpm);
        time_gap = 60000 / bpm / note_time_gap * 4;
        line_degree = chart.findDegreeByTime(now);
        std::fill(probabilities.begin(), probabilities.end(), 1.0);
        int gap_pos[4] = {gap_trans(line_degree - space), gap_trans(line_degree + space), gap_trans(line_degree - space + 180), gap_trans(line_degree + space + 180)};

        if (gap_pos[0] < 0) {
            gap_pos[0] += gap_trans(360);
            std::fill(probabilities.begin(), probabilities.begin() + gap_pos[1] + 1, 0.0);
            std::fill(probabilities.begin() + gap_pos[2], probabilities.begin() + gap_pos[3] + 1, 0.0);
            std::fill(probabilities.begin() + gap_pos[0], probabilities.end(), 0.0);
        }
        else if (gap_pos[3] >= gap_trans(360)) {
            gap_pos[3] -= gap_trans(360);
            std::fill(probabilities.begin() + gap_pos[0], probabilities.begin() + gap_pos[1] + 1, 0.0);
            std::fill(probabilities.begin() + gap_pos[2], probabilities.end(), 0.0);
            std::fill(probabilities.begin(), probabilities.begin() + gap_pos[3] + 1, 0.0);
        }
        else {
            std::fill(probabilities.begin() + gap_pos[0], probabilities.begin() + gap_pos[1] + 1, 0.0);
            std::fill(probabilities.begin() + gap_pos[2], probabilities.begin() + gap_pos[3] + 1, 0.0);
        }
        
        bomb_count = mit::truncated_normal_a_sample(mean, sd, 0, seed);

        for (std::size_t i = 0; i < bomb_count; i++) {
            bomb_pos = select_event(probabilities);
            probabilities = place_bomb(probabilities, bomb_pos);
            chart.pushBackNote({21, round(now), round(bomb_pos * 360.0 / pieces)});
            // if (!checked) {
            //     std::cout << "Probabilities: ";
            //     for (std::size_t i = 0; i < probabilities.size(); i++) {
            //         std::cout << probabilities[i] << " ";
            //     }
            //     std::cout << std::endl;
            //     checked = true;
            // }
        }



        now += time_gap;
    }


    showCompleteAndSave();
}