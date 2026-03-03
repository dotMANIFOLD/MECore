//
// Created by ryen on 12/31/24.
//

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>

#include "MECore/physics/PhysicsGlobals.h"

namespace ME::physics {
    bool Initialize() {
        // this is irreversible lol
        JPH::RegisterDefaultAllocator();

        // add trace and assert callbacks
        // trace
        // assert

        JPH::Factory::sInstance = new JPH::Factory();

        // If you have any custom shapes and/or a custom default material, implement them before this.
        JPH::RegisterTypes();

        // 10 MB cache is considered a safe amount by Jolt, Wicked says otherwise.
        tempAllocator = new JPH::TempAllocatorImpl(30 * 1024 * 1024);
        jobSystemPool = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsJobs, std::thread::hardware_concurrency() - 1);

        return true;
    }

    void Shutdown() {
        delete jobSystemPool;
        delete tempAllocator;

        JPH::UnregisterTypes();

        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }
}
