//
// Created by dotryen on 12/20/25.
//

#pragma once

#include <angelscript.h>

#include "MECore/Service/IService.h"

namespace ME {
    class IScriptService : IService {
        public:
        virtual asIScriptEngine* GetEngine() = 0;
        // virtual asIScriptModule* GetEngineModule() = 0;
        // virtual asIScriptModule* GetMainModule() = 0;
    };
}
