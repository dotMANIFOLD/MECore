//
// Created by ryen on 12/6/24.
//

#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Math/Float2.h>
#include <Jolt/Math/Vector.h>

namespace ME {
    constexpr float DEG_TO_RAD = JPH::JPH_PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / JPH::JPH_PI;
    
    using Vector2 = JPH::Vector<2>;
    using PackedVector2 = JPH::Float2;
    using Vector3 = JPH::Vec3;
    using PackedVector3 = JPH::Float3;
    using Vector4 = JPH::Vec4;
    using PackedVector4 = JPH::Float4;

    // TODO: write color wrapper
    using Color = JPH::Vec4;
    using Quaternion = JPH::Quat;
    using Matrix4x4 = JPH::Mat44;
    using PackedMatrix4x4 = PackedVector4[4];
}
