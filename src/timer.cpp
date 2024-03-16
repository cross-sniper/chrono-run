#include <thread>
#include <chrono>

class Timer {
private:
    bool clear;
    int timeout;
public:
    Timer() : clear(false) {}
    void setTimeout(int n){
        timeout = n;
    }
    template<typename Function>
    void TimeOut(Function fn) {
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
    void Interval(Function fn) {
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
