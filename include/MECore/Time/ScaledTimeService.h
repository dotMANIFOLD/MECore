//
// Created by dotryen on 12/19/25.
//

#pragma once

#include "IScaledTimeService.h"

namespace ME {
    class ScaledTimeService : IScaledTimeService {
        private:
        double elapsed;
        double delta;
        double scale;
        uint64_t id;
        uint64_t lastChecked;
        
        public:
        ScaledTimeService();
        
        [[nodiscard]] double GetElapsed() const override { return elapsed; }
        [[nodiscard]] double GetDelta() const override { return delta; }
        [[nodiscard]] double GetScale() const override { return scale; }
        [[nodiscard]] uint64_t GetID() const override { return id; }
        
        void Update() override;
        void Reset() override;
    };
}
