#ifndef _SJTU_CPP_FINAL_SRC_HPP_
#define _SJTU_CPP_FINAL_SRC_HPP_

// Implement the three schedulers without adding headers.

namespace sjtu {

    // FCFS scheduler
    class CPU_FCFS : public CPU {
    public:
        std::pair<CPUState, unsigned int> run() override {
            if (tasks.empty()) {
                state = idle;
                return std::make_pair(idle, 0u);
            }
            unsigned int id = tasks.front().task_id;
            if (tasks.front().time > 0) --tasks.front().time;
            if (tasks.front().time == 0) tasks.erase(tasks.begin());
            state = busy;
            return std::make_pair(busy, id);
        }
    };

    // SRTF scheduler (preemptive)
    class CPU_SRTF : public CPU {
    public:
        std::pair<CPUState, unsigned int> run() override {
            if (tasks.empty()) {
                state = idle;
                return std::make_pair(idle, 0u);
            }
            size_t best = 0;
            for (size_t i = 1; i < tasks.size(); ++i) {
                if (tasks[i].time < tasks[best].time) best = i;
            }
            unsigned int id = tasks[best].task_id;
            if (tasks[best].time > 0) --tasks[best].time;
            if (tasks[best].time == 0) tasks.erase(tasks.begin() + static_cast<long>(best));
            state = busy;
            return std::make_pair(busy, id);
        }
    };

    // Priority scheduler (smaller priority value is higher)
    class CPU_PRIORITY : public CPU {
    public:
        std::pair<CPUState, unsigned int> run() override {
            if (tasks.empty()) {
                state = idle;
                return std::make_pair(idle, 0u);
            }
            size_t best = 0;
            for (size_t i = 1; i < tasks.size(); ++i) {
                if (tasks[i].priority < tasks[best].priority) best = i;
            }
            unsigned int id = tasks[best].task_id;
            if (tasks[best].time > 0) --tasks[best].time;
            if (tasks[best].time == 0) tasks.erase(tasks.begin() + static_cast<long>(best));
            state = busy;
            return std::make_pair(busy, id);
        }
    };
}

#endif
