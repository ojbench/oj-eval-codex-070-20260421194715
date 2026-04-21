#ifndef _SJTU_CPP_FINAL_SRC_HPP_
#define _SJTU_CPP_FINAL_SRC_HPP_

# include <vector>
# include task.hpp

using namespace std;

typedef unsigned int uint;

namespace sjtu {
    // CPU base class (mirrors provided skeleton)
    class CPU {
    protected:
        CPUState state;
        vector<Task> tasks;

    public:
        CPU() : tasks() { state = idle; }

        // Add a new task.
        int addTask(const Task &t) {
            tasks.push_back(t);
            return 1;
        }

        // Change the priority of one process, return 1 if success and return 0 if fail.
        int changePriority(uint task_id, uint priority) {
            for (auto &task: tasks)
                if (task.task_id == task_id) {
                    task.priority = priority;
                    return 1;
                }
            return 0;
        }

        // Run one time unit and return (state, task_id)
        virtual pair<CPUState, uint> run() = 0;
        virtual ~ CPU() = default;
    };

    // FCFS method based CPU.
    class CPU_FCFS : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) { state = idle; return make_pair(idle, 0u); }
            uint id = tasks.front().task_id;
            if (tasks.front().time > 0) --tasks.front().time;
            if (tasks.front().time == 0) tasks.erase(tasks.begin());
            state = busy;
            return make_pair(busy, id);
        }
    };

    // SRTF method based CPU.
    class CPU_SRTF : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) { state = idle; return make_pair(idle, 0u); }
            size_t best = 0;
            for (size_t i = 1; i < tasks.size(); ++i)
                if (tasks[i].time < tasks[best].time) best = i;
            uint id = tasks[best].task_id;
            if (tasks[best].time > 0) --tasks[best].time;
            if (tasks[best].time == 0) tasks.erase(tasks.begin() + static_cast<long>(best));
            state = busy;
            return make_pair(busy, id);
        }
    };

    // Priority method based CPU.
    class CPU_PRIORITY : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) { state = idle; return make_pair(idle, 0u); }
            size_t best = 0;
            for (size_t i = 1; i < tasks.size(); ++i)
                if (tasks[i].priority < tasks[best].priority) best = i;
            uint id = tasks[best].task_id;
            if (tasks[best].time > 0) --tasks[best].time;
            if (tasks[best].time == 0) tasks.erase(tasks.begin() + static_cast<long>(best));
            state = busy;
            return make_pair(busy, id);
        }
    };
}

#endif
