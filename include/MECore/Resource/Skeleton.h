//
// Created by ryen on 3/31/25.
//

#pragma once

#include <ozz/animation/runtime/skeleton.h>

#include "Resource.h"
#include "MECore/math/Transform.h"

namespace ME {
    struct Attachment {
        int jointId;
        Transform transform;
    };

    class Skeleton : public Resource {
        private:
        ozz::animation::Skeleton skeleton;

        public:
        Skeleton() = delete;

        const ozz::animation::Skeleton& GetSkeleton() const { return skeleton; }
    };
}
