#ifndef EXAM_LIB_PEAKHOSPITALIZATIONOBSERVER_H
#define EXAM_LIB_PEAKHOSPITALIZATIONOBSERVER_H
#pragma once
#include "Observer.h"
#include "SystemState.h"

namespace sim {
    class PeakHospitalizationObserver : public sim::Observer {
    public:
        PeakHospitalizationObserver() : peakHospitalization(0) {}

        void observe(const SystemState &state) override {
            int currentHospitalization = state.getCount("H");
            if (currentHospitalization > peakHospitalization) {
                peakHospitalization = currentHospitalization;
            }
        }

        int getPeakHospitalization() const {
            return peakHospitalization;
        }

    private:
        int peakHospitalization;
    };
}
#endif // EXAM_LIB_PEAKHOSPITALIZATIONOBSERVER_H
