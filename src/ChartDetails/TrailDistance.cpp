#include "ChartDetails/TrailDistance.h"

void TrailDistance::showUpdateVectorError(const size_t s) const {
    std::cerr << "Error: An issue occurred while setting TrailDistance vector. Expected input size of 6 but received " << s << std::endl;
    pauseConsoleAndExit();
}