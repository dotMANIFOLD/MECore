//
// Created by dotryen on 12/19/25.
//

#pragma once
#include "ITimeService.h"

namespace ME {
    class IScaledTimeService : ITimeService {
        public:
        [[nodiscard]] virtual double GetScale() const = 0;
        virtual void SetScale(double scale) = 0;
    };
}
