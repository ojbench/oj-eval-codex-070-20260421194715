#ifndef _SJTU_CPP_FINAL_SRC_HPP_
#define _SJTU_CPP_FINAL_SRC_HPP_

// Do not add extra headers per problem constraints.

using namespace std;

typedef unsigned int uint;

namespace sjtu {
    struct Task {
        uint task_id;
        uint priority;
        uint time;

        explicit Task(uint _task_id = 0, uint _priority = 0, uint _time = 0) {
            task_id = _task_id;
            priority = _priority;
            time = _time;
        }

        Task(const Task &rhs) {
            task_id = rhs.task_id;
            priority = rhs.priority;
            time = rhs.time;
        }
    };

    enum CPUState { idle = 0, busy = 1 };

    class CPU {
    protected:
        CPUState state;
        // Use a simple dynamic array-like container without including <vector>.
        // Implement a minimal vector to satisfy constraints of not adding headers.
        struct TaskVec {
            Task *data;
            size_t sz;
            size_t cap;
            TaskVec(): data(0), sz(0), cap(0) {}
            ~TaskVec(){ if(data) delete [] data; }
            void push_back(const Task &t){
                if(sz==cap){
                    size_t ncap = cap? cap*2: 4;
                    Task *nd = new Task[ncap];
                    for(size_t i=0;i<sz;++i) nd[i]=data[i];
                    if(data) delete [] data;
                    data = nd; cap = ncap;
                }
                data[sz++] = t;
            }
            size_t size() const { return sz; }
            Task &operator[](size_t i){ return data[i]; }
            const Task &operator[](size_t i) const { return data[i]; }
            void erase_index(size_t idx){
                if(idx>=sz) return;
                for(size_t i=idx+1;i<sz;++i) data[i-1]=data[i];
                --sz;
            }
            bool empty() const { return sz==0; }
        } tasks;

    public:
        CPU() : tasks() { state = idle; }

        int addTask(const Task &t) {
            tasks.push_back(t);
            return 1;
        }

        int changePriority(uint task_id, uint priority) {
            for (size_t i=0;i<tasks.size();++i)
                if (tasks[i].task_id == task_id) {
                    tasks[i].priority = priority;
                    return 1;
                }
            return 0;
        }

        virtual pair<CPUState, uint> run() = 0;
        virtual ~CPU() = default;
    };

    class CPU_FCFS : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) {
                state = idle; return make_pair(idle, 0u);
            }
            // Pick the earliest added task: tasks[0]
            uint id = tasks[0].task_id;
            // Consume one unit of time
            if (tasks[0].time > 0) {
                tasks[0].time -= 1;
            }
            if (tasks[0].time == 0) {
                tasks.erase_index(0);
            }
            state = busy;
            return make_pair(busy, id);
        }
    };

    class CPU_SRTF : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) { state = idle; return make_pair(idle, 0u); }
            // Select task with minimal remaining time; tie-breaker: earlier arrival (lower index)
            size_t best = 0;
            for (size_t i=1;i<tasks.size();++i){
                if (tasks[i].time < tasks[best].time) best = i;
            }
            uint id = tasks[best].task_id;
            if (tasks[best].time > 0) tasks[best].time -= 1;
            if (tasks[best].time == 0) tasks.erase_index(best);
            state = busy;
            return make_pair(busy, id);
        }
    };

    class CPU_PRIORITY : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) { state = idle; return make_pair(idle, 0u); }
            // Select task with highest priority (smaller priority value), tie by earlier arrival (lower index)
            size_t best = 0;
            for (size_t i=1;i<tasks.size();++i){
                if (tasks[i].priority < tasks[best].priority) best = i;
            }
            uint id = tasks[best].task_id;
            if (tasks[best].time > 0) tasks[best].time -= 1;
            if (tasks[best].time == 0) tasks.erase_index(best);
            state = busy;
            return make_pair(busy, id);
        }
    };
}

#endif

