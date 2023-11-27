#include "MainProcess.h"

MainProcess::MainProcess(int argc, char* argv[]) {
	filePath = std::string(argv[1]);
}

int MainProcess::start() {
    input_file.open(filePath, std::ios::in);
    if (!input_file.is_open()) {
        std::cerr << "Error! File Cannot Open!" << std::endl;
        return -1;
    }
    else {
        chart.readFile(input_file);
    }
    
    menu();
    input_file.close();
    std::cout << "Application closing, press enter to continue..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    return 0;
}

void MainProcess::menu() {
    std::string command;
    chart.sortAll();
    chart.distancePreprocessing();

    while (1) {
        InputScreen::clearScreen();
        std::cout << "1.Time Shift" << std::endl;
        std::cout << "2.Chart Rotate" << std::endl;
        std::cout << "3.Chart Rotate(linearly)" << std::endl;
        std::cout << "4.Note Conversion" << std::endl;
        std::cout << "5.Get Distance" << std::endl;
        std::cout << "6.Real Copy" << std::endl;
        std::cout << "7.\'Circuit\' Maker" << std::endl;
        std::cout << "8.Random Right Angle Maker" << std::endl;
        std::cout << "9.Note Follow Trail" << std::endl;
        std::cout << "10.Sawtooth Wave Maker" << std::endl;
        std::cout << "Input \"x\" to exit" << std::endl << std::endl;
        std::cout << "Input:";
        std::cin >> command;
        if (command == "x") {
            break;
        }
        else if (command == "1") {
            timeShift();
        }
        else if (command == "2") {
            chartRotate();
        }
        else if (command == "3") {
            chartRotateLinear();
        }
        else if (command == "4") {
            noteConversion();
        }
        else if (command == "5") {
            getDistance();
        }
        else if (command == "6") {
            realCopy();
        }
        else if (command == "7") {
            makeCircuit();
        }
        else if (command == "8") {
            randomRightAngleMaker();
        }
        else if (command == "9") {
            noteFollowTrail();
        }
        else if (command == "10") {
            sawtoothWaveMaker();
        }
        else if (command == "dx") {
            dextorySpeed();
        }
        else if (command == "h") {
            hiddenChartMaker();
        }
        else if (command == "rr") {
            RrharilSpeed();
        }
        else if (command == "re"){
            ReverseChartMaker();
        }
        else if (command == "t") {
            int t;
            std::cin >> t;
            std::cout << chart.findDegreeByTime(t);
            std::cout << std::endl << "Complete!" << std::endl;
            pauseConsole();
        }
    }
    return;
}

void MainProcess::showComplete() {
    chart.sortAll();
    chart.distancePreprocessing();
    std::cout << std::endl << "Complete!" << std::endl;
    pauseConsole();
}

void MainProcess::showCompleteAndSave() {
    chart.sortAll();
    chart.distancePreprocessing();
    writeFile();
    std::cout << std::endl << "Complete!" << std::endl;
    pauseConsole();
}

void MainProcess::writeFile() {
    std::string str1, str2;
    std::size_t found = filePath.find_last_of(".");
    str1 = filePath.substr(0, found);
    str2 = filePath.substr(found);
    str1 = str1 + "_(modified)" + str2;

    output_file.open(str1, std::ios::out | std::ios::trunc);
    if (!output_file.is_open()) {
        std::cerr << "Error! No enough space!" << std::endl;
    }
    else {
        output_file << std::fixed;
        output_file << chart.getChart();
        output_file.close();
    }
}

void MainProcess::pauseConsole() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Press enter to continue...";
    std::cin.get();
}