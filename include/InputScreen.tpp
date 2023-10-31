template <typename... Args>
std::string InputScreen::inputArgument(Chart& chart, const std::string& title, Args&... args) {
    std::string pass;
    int count = 0;

    auto get_screen_output = [&](auto& lambda, double& input, const std::string& screen_str, const std::string& range, auto&... args) {
        setScreenString(screen_str);
        if constexpr (sizeof...(args) > 0) {
            lambda(lambda, args...);
        }
    };

    // Lambda for recursive processing
    auto input_all_argument = [&](auto& lambda, std::string& pass, double& input, const std::string& screen_str, const std::string& range, auto&... args) {
        if (pass == "all") {
            double t1 = chart.getNote(chart.getNoteSize() - 1).getValue(NoteAttr::time);
            double t2 = chart.getSpeed(chart.getSpeedSize() - 1).getValue(SpeedAttr::time);
            double t3 = chart.getBPM(chart.getBPMSize() - 1).getValue(BPMAttr::time);
            std::string input = std::to_string(std::max({ t1, t2, t3 }));
            if (input.find('.') != std::string::npos) {
                while (true) {
                    if (input.back() == '0') {
                        input.pop_back();
                    }
                    else if (input.back() == '.'){
                        input.pop_back();
                        break;
                    }
                    else{
                        break;
                    }
                }
            }

            addToStringDeq(input, count);
            count++;
            pass.clear();
        }
        else {
            std::string output = inputFromUser(screen_str, range, count);
            count++;
            if (output == "x") {
                pass = output;
                return;
            }
            else if (isNumber(output)) {
                input = stod(output);
            }
            pass = output;
        }
        if constexpr (sizeof...(args) > 0) {
            lambda(lambda, pass, args...);
        }
    };

    clearAll();
    setTitle(title);
    get_screen_output(get_screen_output, args...);
    input_all_argument(input_all_argument, pass, args...);
    clearScreen();
    printStringDeq();
    return pass;
}