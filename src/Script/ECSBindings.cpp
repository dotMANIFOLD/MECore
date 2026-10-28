//
// Created by dotryen on 10/27/26.
//

#include <flecs.h>
#include <string>

#include "MECore/Script/Bindings.h"

using namespace flecs;

namespace ME {
    static void RegisterEntity(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterObjectType("Entity", sizeof(entity), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entity>());
        assert(r >= 0);

        r = engine->RegisterFuncdef("void EntityIterator(Entity ent)");
        assert(r >= 0);

        r = engine->RegisterObjectMethod("Entity", "uint64 get_id() property",
            asMETHOD(entity, id),
            asCALL_THISCALL);
        assert(r >= 0);

        r = engine->RegisterObjectMethod("Entity", "string get_name() property",
            asFUNCTIONPR([](entity* entity) {
                return std::string(entity->name());
            }, (entity*), std::string), asCALL_CDECL_OBJLAST);
        assert(r >= 0);

        r = engine->RegisterObjectMethod("Entity", "void Children(EntityIterator@ iter)",
            asFUNCTIONPR([](asIScriptFunction* func, entity* initialEnt) {
                auto ctx = asGetActiveContext();
                initialEnt->children([ctx, func](entity ent) {
                    ctx->PushState();
                    ctx->Prepare(func);
                    ctx->SetArgObject(0, &ent);
                    ctx->Execute();
                    // TODO: handle exceptions
                    ctx->PopState();
                });
            }, (asIScriptFunction*, entity*), void),
            asCALL_CDECL_OBJLAST);
    }

    static void RegisterWorld(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterObjectType("World", sizeof(world), asOBJ_REF | asOBJ_NOCOUNT);
        assert(r >= 0);

        r = engine->RegisterObjectMethod("World", "void Children(EntityIterator@ iter)",
            asFUNCTIONPR([](asIScriptFunction* func, world* world) {
                auto ctx = asGetActiveContext();
                world->children([ctx, func](entity ent) {
                    ctx->PushState();
                    ctx->Prepare(func);
                    ctx->SetArgObject(0, &ent);
                    ctx->Execute();
                    // TODO: handle exceptions
                    ctx->PopState();
                });
            }, (asIScriptFunction*, world*), void),
            asCALL_CDECL_OBJLAST);
        assert(r >= 0);
    }

    void RegisterECS(asIScriptEngine* engine) {
        RegisterEntity(engine);
        RegisterWorld(engine);
    }
}
