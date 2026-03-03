//
// Created by ryen on 12/31/24.
//

#pragma once

#include <cstdint>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace ME::physics {
    enum class PhysicsLayer : JPH::ObjectLayer {
        Static = 0,
        Dynamic,
        Count
    };

    enum class PhysicsBPLayer : JPH::uint8 {
        Static = 0,
        Dynamic,
        Count
    };

    // A simple object layer to broad phase layer map.
    class BPLayerInterface final : public JPH::BroadPhaseLayerInterface {
        private:
        JPH::BroadPhaseLayer objToBP[static_cast<int>(PhysicsLayer::Count)];

        public:
        BPLayerInterface();

        virtual uint GetNumBroadPhaseLayers() const override;
        virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
        #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
        #endif
    };
}
