//
// Created by ryen on 12/31/24.
//

#include "MECore/physics/Filters.h"
#include "MECore/physics/Layers.h"

namespace ME::physics {
    bool ObjectPairFilter::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::ObjectLayer inLayer2) const {
        switch (inLayer1) {
            case static_cast<int>(PhysicsLayer::Static):
                return inLayer2 == static_cast<JPH::uint16>(PhysicsLayer::Dynamic);
            case static_cast<int>(PhysicsLayer::Dynamic):
                return true;
            default:
                return false;
        }
    }

    bool ObjectVBPFilter::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const {
        switch (inLayer1) {
            case static_cast<int>(PhysicsLayer::Static):
                return inLayer2 == JPH::BroadPhaseLayer(static_cast<JPH::uint16>(PhysicsBPLayer::Dynamic));
            case static_cast<int>(PhysicsLayer::Dynamic):
                return true;
            default:
                return false;
        }
    }
}
