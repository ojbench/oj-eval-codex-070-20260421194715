#ifndef _SJTU_CPP_FINAL_SRC_HPP_
#define _SJTU_CPP_FINAL_SRC_HPP_

namespace sjtu {

// FCFS: first-come, first-served (queue order)
pair<CPUState, uint> CPU_FCFS::run() {
    if (tasks.empty()) {
        state = idle;
        return make_pair(idle, 0u);
    }
    uint id = tasks.front().task_id;
    if (tasks.front().time > 0) --tasks.front().time;
    if (tasks.front().time == 0) tasks.erase(tasks.begin());
    state = busy;
    return make_pair(busy, id);
}

// SRTF: shortest remaining time first; tie -> earlier arrival (lower index)
pair<CPUState, uint> CPU_SRTF::run() {
    if (tasks.empty()) {
        state = idle;
        return make_pair(idle, 0u);
    }
    size_t best = 0;
    for (size_t i = 1; i < tasks.size(); ++i) {
        if (tasks[i].time < tasks[best].time) best = i;
    }
    uint id = tasks[best].task_id;
    if (tasks[best].time > 0) --tasks[best].time;
    if (tasks[best].time == 0) tasks.erase(tasks.begin() + static_cast<long>(best));
    state = busy;
    return make_pair(busy, id);
}

// Priority: smaller priority value is higher; tie -> earlier arrival
pair<CPUState, uint> CPU_PRIORITY::run() {
    if (tasks.empty()) {
        state = idle;
        return make_pair(idle, 0u);
    }
    size_t best = 0;
    for (size_t i = 1; i < tasks.size(); ++i) {
        if (tasks[i].priority < tasks[best].priority) best = i;
    }
    uint id = tasks[best].task_id;
    if (tasks[best].time > 0) --tasks[best].time;
    if (tasks[best].time == 0) tasks.erase(tasks.begin() + static_cast<long>(best));
    state = busy;
    return make_pair(busy, id);
}

} // namespace sjtu

#endif
