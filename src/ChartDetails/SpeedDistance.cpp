#include "ChartDetails/SpeedDistance.h"

void SpeedDistance::showUpdateVectorError(const size_t s) const {
    std::cerr << "Error: An issue occurred while setting SpeedDistance vector. Expected input size of 4 but received " << s << std::endl;
    pauseConsoleAndExit();
}
