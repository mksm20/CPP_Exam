#ifndef EXAM_LIB_SYSTEMSTATE_H
#define EXAM_LIB_SYSTEMSTATE_H
#pragma once
#include <map>
#include <string>
#include <vector>
#include "Species.h"
#include "SymbolTable.h"
#include <coroutine>

namespace sim {
    class SystemState {
    public:
        SystemState() = default;
        SystemState(const SystemState& other);
        struct promise_type {
            SystemState get_return_object() {
                return SystemState{handle_type::from_promise(*this)};
            }

            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() { std::terminate(); }
        };

        using handle_type = std::coroutine_handle<promise_type>;

        // Constructor to initialize from a coroutine handle
        SystemState(handle_type h) : coro(h) {}

        void addSpecies(const Species& species);
        int getCount(const std::string& species) const;
        void updateCount(const std::string& species, int count);
        void prettyPrint() const;
        const std::map<std::string, int>& getState() const;
        std::map<std::string, int>& getState();
        void recordState(double time);
        const std::map<std::string, std::vector<int>>& getTrajectory() const;
        const std::vector<double>& getTimePoints() const;
        void replaceState();
        void removeSpecies(const std::string &species, int count);
    private:
        handle_type coro;
        SymbolTable<std::string, int> state;
        std::map<std::string, std::vector<int>> trajectory;
        std::vector<double> timePoints;



    };

}
#endif //EXAM_LIB_SYSTEMSTATE_H
