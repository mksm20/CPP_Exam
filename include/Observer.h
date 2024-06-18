#ifndef EXAM_LIB_OBSERVER_H
#define EXAM_LIB_OBSERVER_H
#pragma once
#include "SystemState.h"
#include <memory>

namespace sim {

    class Observer {
    public:
        virtual void observe(const std::shared_ptr<SystemState>& state, double endTime, double& currentTime) = 0;
        virtual bool move_next() = 0;
        virtual ~Observer() = default;
    };

}

#endif // EXAM_LIB_OBSERVER_H
