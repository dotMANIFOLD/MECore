//
// Created by dotryen on 1/12/26.
//

#pragma once
#include "IValid.h"

namespace ME {
    class IDestructible : public IValid {
        public:
        virtual void Destroy() = 0;
    };
}
