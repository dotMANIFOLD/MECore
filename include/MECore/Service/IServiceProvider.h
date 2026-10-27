//
// Created by dotryen on 12/18/25.
//

#pragma once

#include "MECore/Filesystem/IFileSystem.h"
#include "MECore/Log/ILogService.h"
#include "MECore/Render/IRenderInterface.h"
#include "MECore/Render/IRenderPipeline.h"
#include "MECore/Script/IScriptService.h"
#include "MECore/Time/IScaledTimeService.h"

namespace ME {
    class IServiceProvider {
        public:
        virtual ~IServiceProvider() = default;
        
        virtual IRenderInterface* GetRenderInterface() = 0;
        virtual IRenderPipeline* GetRenderPipeline() = 0;
        virtual IScriptService* GetScriptService() = 0;
        
        virtual IScaledTimeService* GetGameTimeService() = 0;
        virtual ITimeService* GetRealTimeService() = 0;
        
        virtual IFileSystem* GetFileSystem() = 0;
        virtual ILogService* GetLogService() = 0;
    };
}
