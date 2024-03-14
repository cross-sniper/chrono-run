#include <thread>
#include <chrono>

class Timer {
private:
    bool clear;

public:
    Timer() : clear(false) {}

    template<typename Function>
    void TimeOut(Function fn, int timeout) {
        clear = false;
        std::thread t([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
            if (!clear) {
                fn();
                stop();
            }
        });
        t.detach();
    }

    template<typename Function>
    void Interval(Function fn, int timeout) {
        clear = false;
        std::thread t([=]() {
            while(true){
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
                if (!clear) {
                    fn();
                }
            }
        });
        t.detach();
    }

    void stop() {
        clear = true;
    }
};
