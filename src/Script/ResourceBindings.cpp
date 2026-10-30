//
// Created by dotryen on 10/29/26.
//

#include <cassert>

#include "Texture.h"
#include "MECore/Resource/Material/Material.h"
#include "MECore/Resource/Mesh/Mesh.h"
#include "MECore/Resource/Model/Model.h"
#include "MECore/Script/Bindings.h"

namespace ME {
    static void RegisterTexture(asIScriptEngine* engine) {
        int r;
        constexpr auto TYPE_NAME = "Texture";

        r = engine->RegisterObjectType(TYPE_NAME, sizeof(ITexture*), asOBJ_VALUE | asGetTypeTraits<ITexture*>());
        assert(r >= 0);

        r = engine->RegisterObjectBehaviour(TYPE_NAME, asBEHAVE_CONSTRUCT, "void f()",
            asFUNCTIONPR([](ITexture** tex) {
                *tex = nullptr;
            }, (ITexture**), void),
            asCALL_CDECL_OBJLAST);
        assert(r >= 0);

        r = engine->RegisterObjectBehaviour(TYPE_NAME, asBEHAVE_DESTRUCT, "void f()",
            asFUNCTIONPR([](ITexture** tex) {
                // cant really do anything
            }, (ITexture**), void),
            asCALL_CDECL_OBJLAST);
        assert(r >= 0);

        r = engine->RegisterObjectMethod(TYPE_NAME, "bool get_IsNull() const property",
            asFUNCTIONPR([](const ITexture** tex) {
                return *tex == nullptr;
            }, (const ITexture**), bool),
            asCALL_CDECL_OBJLAST);
        assert(r >= 0);
    }

    static void RegisterMaterial(asIScriptEngine* engine) {
        int r;
        constexpr auto TYPE_NAME = "Material";

        r = engine->RegisterObjectType(TYPE_NAME, sizeof(ME::Material), asOBJ_REF | asOBJ_NOCOUNT);
        assert(r >= 0);
    }

    static void RegisterMesh(asIScriptEngine* engine) {
        int r;
        constexpr auto TYPE_NAME = "Mesh";

        r = engine->RegisterObjectType(TYPE_NAME, sizeof(ME::Mesh), asOBJ_REF | asOBJ_NOCOUNT);
        assert(r >= 0);
    }

    static void RegisterModel(asIScriptEngine* engine) {
        int r;
        constexpr auto TYPE_NAME = "Model";

        r = engine->RegisterObjectType(TYPE_NAME, sizeof(ME::Model), asOBJ_REF | asOBJ_NOCOUNT);
        assert(r >= 0);
    }

    void RegisterResources(asIScriptEngine* engine) {
        RegisterTexture(engine);
        RegisterMaterial(engine);
        RegisterMesh(engine);
        RegisterModel(engine);
    }
}
