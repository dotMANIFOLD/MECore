//
// Created by ryen on 12/31/24.
//

#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include "Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h"

namespace ME::physics {
    // Simple filter for Object v Object situations
    class ObjectPairFilter : public JPH::ObjectLayerPairFilter {
        public:
        virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::ObjectLayer inLayer2) const override;
    };

    // Simple filter for Object v Broadphase situations
    class ObjectVBPFilter : public JPH::ObjectVsBroadPhaseLayerFilter {
        public:
        virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
    };
}
