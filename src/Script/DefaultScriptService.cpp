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
        
        // UUID
        engine->RegisterObjectType("Uuid", sizeof(boost::uuids::uuid), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<boost::uuids::uuid>());
        engine->SetDefaultNamespace("Uuid");
        engine->RegisterGlobalFunction("Uuid New()", asFUNCTIONPR([] { return boost::uuids::random_generator()(); }, (), boost::uuids::uuid), asCALL_CDECL);
        engine->SetDefaultNamespace("");
        
        RegisterVector3();
    }

    DefaultScriptService::~DefaultScriptService() {
        engine->ShutDownAndRelease();
    }

    void DefaultScriptService::RegisterVector3() {
        engine->RegisterObjectType("Vector3", sizeof(Vector3), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<Vector3>() | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_MORE_CONSTRUCTORS);
        // Cant do this because of W
        // Jolt vectors are actually arrays under the hood. So there is no field to get the offset of.
        // engine->RegisterObjectProperty("Vector3", "float x", 0); 
        // engine->RegisterObjectProperty("Vector3", "float y", 4);
        // engine->RegisterObjectProperty("Vector3", "float z", 8);
        
        engine->RegisterObjectMethod("Vector3", "float get_x() property", asMETHOD(Vector3, GetX), asCALL_THISCALL);
        engine->RegisterObjectMethod("Vector3", "void set_x(float) property", asMETHOD(Vector3, SetX), asCALL_THISCALL);
        engine->RegisterObjectMethod("Vector3", "float get_y() property", asMETHOD(Vector3, GetY), asCALL_THISCALL);
        engine->RegisterObjectMethod("Vector3", "void set_y(float) property", asMETHOD(Vector3, SetY), asCALL_THISCALL);
        engine->RegisterObjectMethod("Vector3", "float get_z() property", asMETHOD(Vector3, GetZ), asCALL_THISCALL);
        engine->RegisterObjectMethod("Vector3", "void set_z(float) property", asMETHOD(Vector3, SetZ), asCALL_THISCALL);
        
        engine->RegisterObjectBehaviour("Vector3", 
            asBEHAVE_CONSTRUCT, 
            "void f(float, float, float)", 
            asFUNCTIONPR([](float x, float y, float z, Vector3* self) {
                new(self) Vector3(x, y, z);
            },(float, float, float, Vector3*), void),
            asCALL_CDECL_OBJLAST);
    }
}
