#include "Util.h"
#include <deque>
#include <chrono>

class ExecutionsPerSecond {
public:
    double measure() {
        auto now = std::chrono::high_resolution_clock::now();
        history.push_back(now);
        while (history.size() > 5) {
            history.pop_front();
        }
        if (history.size() >= 2) {
            std::chrono::duration<double> diff = history.back() - history.front();
            return 1.0 / (diff.count() / history.size());
        }
        return 0.0;
    }
protected:
    std::deque<std::chrono::time_point<std::chrono::high_resolution_clock>> history;
};
