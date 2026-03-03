//
// Created by ryen on 12/4/24.
//

#pragma once

#include "Types.h"

namespace ME {
    struct Transform {
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;

        Transform() = default;
        Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) {
            this->position = position;
            this->rotation = rotation;
            this->scale = scale;
        }

        static constexpr Transform Identity();

        inline Vector3 Right() const;
        inline Vector3 Up() const;
        inline Vector3 Forward() const;

        // Localizes a point.
        inline Vector3 EnterPoint(const Vector3& worldPoint) const;
        // Localizes a direction.
        inline Vector3 EnterDirection(const Vector3& worldDir) const;
        // Localizes a vector.
        inline Vector3 EnterVector(const Vector3& worldVector) const;
        inline Quaternion EnterQuaternion(const Quaternion& worldRot) const;
        inline Transform Enter(const Transform& world) const;

        // De-localizes a point. Inverse of EnterPoint
        inline Vector3 ExitPoint(const Vector3& localPoint) const;
        // De-localizes a direction. Inverse of EnterDirection
        inline Vector3 ExitDirection(const Vector3& localDir) const;
        // De-localizes a vector. Inverse of EnterVector
        inline Vector3 ExitVector(const Vector3& localVector) const;
        inline Quaternion ExitQuaternion(const Quaternion& localRot) const;
        inline Transform Exit(const Transform& local) const;

        inline Matrix4x4 ToTRS() const;
        inline Matrix4x4 ToInverseTRS() const;
    };
}

#include "Transform.inl"
