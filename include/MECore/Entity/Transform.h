//
// Created by dotryen on 2/27/26.
//

#pragma once

#include "MECore/Math/Transform.h"

namespace ME {
    struct LocalTransform {
        Transform value;

        LocalTransform() = default;
        ~LocalTransform() = default;
        LocalTransform(const Transform& val) : value(val) {}

        operator Transform() const { return value; }
    };

    struct WorldTransform {
        Transform value;

        WorldTransform() = default;
        ~WorldTransform() = default;
        WorldTransform(const Transform& val) : value(val) {}

        operator Transform() const { return value; }
    };
}
