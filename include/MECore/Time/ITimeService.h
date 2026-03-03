//
// Created by dotryen on 12/19/25.
//

#pragma once
#include "MECore/Service/IService.h"

namespace ME {
    class ITimeService : IService {
        public:
        virtual double GetElapsed() const = 0;
        virtual double GetDelta() const = 0;
        virtual uint64_t GetID() const = 0;
        
        virtual void Update() = 0;
        virtual void Reset() = 0;
    };
}
