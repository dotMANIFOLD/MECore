//
// Created by ryen on 12/31/24.
//

#include "MECore/physics/Layers.h"

namespace ME::physics {
    BPLayerInterface::BPLayerInterface() {
        objToBP[static_cast<JPH::uint8>(PhysicsLayer::Static)] = JPH::BroadPhaseLayer{ static_cast<JPH::uint8>(PhysicsBPLayer::Static) };
        objToBP[static_cast<JPH::uint8>(PhysicsLayer::Dynamic)] = JPH::BroadPhaseLayer{ static_cast<JPH::uint8>(PhysicsBPLayer::Dynamic) };
    }

    uint BPLayerInterface::GetNumBroadPhaseLayers() const {
        return static_cast<uint>(PhysicsBPLayer::Count);
    }


    JPH::BroadPhaseLayer BPLayerInterface::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const {
        return objToBP[static_cast<JPH::uint8>(inLayer)];
    }

    #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* BPLayerInterface::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const {
        switch ((JPH::BroadPhaseLayer::Type)inLayer)
        {
            case (JPH::BroadPhaseLayer::Type)PhysicsBPLayer::Static: return "STATIC";
            case (JPH::BroadPhaseLayer::Type)PhysicsBPLayer::Dynamic: return "DYNAMIC";
            default: JPH_ASSERT(false); return "INVALID";
        }
    }
    #endif
}
