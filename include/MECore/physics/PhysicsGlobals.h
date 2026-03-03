//
// Created by ryen on 12/31/24.
//

#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>

namespace ME::physics {
    inline JPH::TempAllocator* tempAllocator = nullptr;
    inline JPH::JobSystemThreadPool* jobSystemPool = nullptr;

    bool Initialize();
    void Shutdown();
}
