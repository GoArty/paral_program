#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <condition_variable>

using namespace std;

const int NUM_PHILOSOPHERS = 10;
const int THINKING_TIME_MIN = 1;
const int THINKING_TIME_MAX = 3;
const int EATING_TIME_MIN = 1;
const int EATING_TIME_MAX = 3;
const int SIMULATION_TIME = 10;

mutex forks[NUM_PHILOSOPHERS];
condition_variable cv;
bool stop_simulation = false;

void philosopher(int id) {
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    while (!stop_simulation) {
        this_thread::sleep_for(chrono::seconds(THINKING_TIME_MIN + rand() % (THINKING_TIME_MAX - THINKING_TIME_MIN + 1)));
        printf("Философ %d подумал.\n", id);

        unique_lock<mutex> lock_left(forks[left_fork]);
        printf("Философ %d взял левую вилку %d.\n", id, left_fork);

        unique_lock<mutex> lock_right(forks[right_fork], defer_lock);
        if (lock_right.try_lock()) {
            printf("Философ %d взял правую вилку %d.\n", id, right_fork);

            this_thread::sleep_for(chrono::seconds(EATING_TIME_MIN + rand() % (EATING_TIME_MAX - EATING_TIME_MIN + 1)));
            printf("Философ %d поел.\n", id);

            printf("Философ %d отложил вилку %d и %d.\n", id, left_fork, right_fork);
        } else {

            printf("Философ %d не смог взять правую вилку %d.\n", id, right_fork);
        }
    }
}

int main() {
    vector<thread> philosophers;

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers.emplace_back(philosopher, i);
    }

    this_thread::sleep_for(chrono::seconds(SIMULATION_TIME));

    stop_simulation = true;
    cv.notify_all();

    for (auto& t : philosophers) {
        t.join();
    }

    return 0;
}
