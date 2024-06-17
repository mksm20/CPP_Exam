#ifndef EXAM_LIB_PEAKHOSPITALIZATIONOBSERVER_H
#define EXAM_LIB_PEAKHOSPITALIZATIONOBSERVER_H
#pragma once
#include "Observer.h"
#include "SystemState.h"
#include <memory>
#include <coroutine>

namespace sim {

    class PeakHospitalizationObserver : public Observer {
    public:
        PeakHospitalizationObserver() : peakHospitalization(0), coro(nullptr) {}

        struct observation_promise_type;
        using handle_type = std::coroutine_handle<observation_promise_type>;

        struct observation_promise_type {
            PeakHospitalizationObserver* observer;
            std::shared_ptr<SystemState> state;

            observation_promise_type(PeakHospitalizationObserver* obs, std::shared_ptr<SystemState> st)
                    : observer(obs), state(st) {}

            auto get_return_object() {
                return handle_type::from_promise(*this);
            }

            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() { std::terminate(); }
        };

        struct observation_task {
            struct promise_type {
                observation_task get_return_object() {
                    return observation_task(std::coroutine_handle<promise_type>::from_promise(*this));
                }
                std::suspend_always initial_suspend() { return {}; }
                std::suspend_always final_suspend() noexcept { return {}; }
                void return_void() {}
                void unhandled_exception() { std::terminate(); }
            };

            using handle_type = std::coroutine_handle<promise_type>;
            handle_type coro;

            observation_task(handle_type h) : coro(h) {}
            observation_task() : coro(nullptr) {}
            observation_task(const observation_task&) = delete;
            observation_task(observation_task&& other) noexcept : coro(other.coro) {
                other.coro = nullptr;
            }
            ~observation_task() {
                if (coro) coro.destroy();
            }

            observation_task& operator=(observation_task&& other) noexcept {
                if (this != &other) {
                    if (coro) coro.destroy();
                    coro = other.coro;
                    other.coro = nullptr;
                }
                return *this;
            }

            bool move_next() {
                if (coro && !coro.done()) {
                    coro.resume();
                }
                return coro && !coro.done();
            }
        };

        observation_task start_observation(std::shared_ptr<SystemState> state, double endTime, double& currentTime) {
            co_await std::suspend_always{};
            while (currentTime < endTime) {
                int currentHospitalization = state->getCount("H");
                if (currentHospitalization > peakHospitalization) {
                    peakHospitalization = currentHospitalization;
                }
                co_await std::suspend_always{};
            }
            co_return; // Properly return from the coroutine once the time condition is met
        }

        void observe(const std::shared_ptr<SystemState>& state, double endTime, double& currentTime) {
            coro = start_observation(state, endTime, currentTime);
        }

        bool move_next() override {
            return coro.move_next();
        }

        int getPeakHospitalization() const {
            return peakHospitalization;
        }

    private:
        observation_task coro;
        int peakHospitalization;
    };

}

#endif // EXAM_LIB_PEAKHOSPITALIZATIONOBSERVER_H
