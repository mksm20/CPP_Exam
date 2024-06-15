#ifndef EXAM_LIB_OBSERVER_H
#define EXAM_LIB_OBSERVER_H
#pragma once
#include "SystemState.h"

namespace sim {
    class Observer {
    public:
        virtual void observe(const SystemState &state) = 0;
        virtual ~Observer() = default;
    };
}

#endif //EXAM_LIB_OBSERVER_H
