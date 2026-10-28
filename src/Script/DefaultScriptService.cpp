//
// Created by dotryen on 1/1/26.
//

#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>
#include <scriptdictionary/scriptdictionary.h>
#include <spdlog/spdlog.h>

#include "MECore/Script/DefaultScriptService.h"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>

#include "MECore/Math/Types.h"
#include "MECore/Script/Bindings.h"

namespace ME {
    static void MessageCallback(const asSMessageInfo* msg, void* param) {
        switch (msg->type) {
            case asMSGTYPE_INFORMATION: {
                spdlog::info("AngelScript // {0} ({1}, {2}) // {3}", msg->section, msg->row, msg->row, msg->message);
                break;
            }
            case asMSGTYPE_WARNING: {
                spdlog::warn("AngelScript // {0} ({1}, {2}) // {3}", msg->section, msg->row, msg->row, msg->message);
                break;
            }
            case asMSGTYPE_ERROR: {
                spdlog::error("AngelScript // {0} ({1}, {2}) // {3}", msg->section, msg->row, msg->row, msg->message);
                break;
            }
        }
    }
    
    DefaultScriptService::DefaultScriptService() {
        engine = asCreateScriptEngine();
        int r;
        r = engine->SetMessageCallback(asFUNCTION(MessageCallback), nullptr, asCALL_CDECL);
        assert(r >= 0);
        
        // STANDARD LIB
        RegisterStdString(engine);
        RegisterScriptArray(engine, true);
        RegisterScriptDictionary(engine);
        RegisterStdStringUtils(engine);

        RegisterAll(engine);

        // UUID
        // engine->RegisterObjectType("Uuid", sizeof(boost::uuids::uuid), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<boost::uuids::uuid>());
        // engine->SetDefaultNamespace("Uuid");
        // engine->RegisterGlobalFunction("Uuid New()", asFUNCTIONPR([] { return boost::uuids::random_generator()(); }, (), boost::uuids::uuid), asCALL_CDECL);
        // engine->SetDefaultNamespace("");;
    }

    DefaultScriptService::~DefaultScriptService() {
        engine->ShutDownAndRelease();
    }
}
