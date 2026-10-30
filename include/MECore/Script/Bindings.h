//
// Created by dotryen on 1/3/26.
//

#pragma once

#include <angelscript.h>

namespace ME {
    void RegisterMath(asIScriptEngine* engine);
    void RegisterLog(asIScriptEngine* engine);
    void RegisterTime(asIScriptEngine* engine);
    void RegisterResources(asIScriptEngine* engine);
    void RegisterECS(asIScriptEngine* engine);
    #if ME_IMGUI
    void RegisterImGui(asIScriptEngine* engine);
    #endif

    inline void RegisterAll(asIScriptEngine* engine) {
        RegisterMath(engine);
        RegisterLog(engine);
        RegisterTime(engine);
        RegisterResources(engine);
        RegisterECS(engine);
        #if ME_IMGUI
        RegisterImGui(engine);
        #endif
    }
}
