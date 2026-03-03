//
// Created by ryen on 2/28/25.
//

#pragma once
#include "MECore/Common/IDestructible.h"

namespace ME {
    class Resource : public IDestructible {
        private:
        bool destroyed = false;
        
        public:
        bool IsValid() const override { return !destroyed; }
        void Destroy() override { destroyed = true; }
    };
}
