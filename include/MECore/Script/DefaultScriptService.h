//
// Created by dotryen on 1/1/26.
//

#pragma once
#include "IScriptService.h"

namespace ME {
    class DefaultScriptService : public IScriptService {
        private:
        asIScriptEngine* engine;
        
        public:
        DefaultScriptService();
        ~DefaultScriptService() override;
        
        asIScriptEngine* GetEngine() override { return engine; }
    };
}
