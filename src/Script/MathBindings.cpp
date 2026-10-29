//
// Created by dotryen on 1/3/26.
//

#include <assert.h>

#include "MECore/Math/Types.h"
#include "MECore/Math/Transform.h"
#include "MECore/Script/Bindings.h"

namespace ME {
    template<class T>
    constexpr asQWORD GetPODFlags() {
        return asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<T>() | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_MORE_CONSTRUCTORS;
    }
    
    #define REGISTER_AXIS(type, axis, axisUpper) \
        r = engine->RegisterObjectMethod(#type, "float get_" #axis "() property", asMETHOD(type, Get##axisUpper), asCALL_THISCALL); \
        assert(r >= 0); \
        r = engine->RegisterObjectMethod(#type, "void set_" #axis "(float v) property", asMETHOD(type, Set##axisUpper), asCALL_THISCALL); \
        assert(r >= 0);

    static void RegisterVector2(asIScriptEngine* engine) {
        int r = engine->RegisterObjectType("Vector2", sizeof(Vector2), GetPODFlags<Vector2>());
        assert(r >= 0);

        r = engine->RegisterObjectProperty("Vector2", "float x", 0);
        assert(r >= 0);
        r = engine->RegisterObjectProperty("Vector2", "float y", 4);
        assert(r >= 0);

        r = engine->RegisterObjectBehaviour("Vector2",
            asBEHAVE_CONSTRUCT,
            "void f(float, float)",
            asFUNCTIONPR([](float x, float y, Vector2* self) {
                self->mF32[0] = x;
                self->mF32[1] = y;
            }, (float, float, Vector2*), void),
            asCALL_CDECL_OBJLAST);
        assert(r >= 0);
    }

    static void RegisterVector3(asIScriptEngine* engine) {
        int r = engine->RegisterObjectType("Vector3", sizeof(Vector3), GetPODFlags<Vector3>());
        // Cant do this because of W
        // Jolt vectors are actually arrays under the hood. So there is no field to get the offset of.
        // engine->RegisterObjectProperty("Vector3", "float x", 0); 
        // engine->RegisterObjectProperty("Vector3", "float y", 4);
        // engine->RegisterObjectProperty("Vector3", "float z", 8);
        
        REGISTER_AXIS(Vector3, x, X);
        REGISTER_AXIS(Vector3, y, Y);
        REGISTER_AXIS(Vector3, z, Z);
        
        r = engine->RegisterObjectBehaviour("Vector3",
            asBEHAVE_CONSTRUCT, 
            "void f(float, float, float)", 
            asFUNCTIONPR([](float x, float y, float z, Vector3* self) {
                new(self) Vector3(x, y, z);
            }, (float, float, float, Vector3*), void),
            asCALL_CDECL_OBJLAST);
        assert(r >= 0);
    }

    static void RegisterVector4(asIScriptEngine* engine) {
        int r = engine->RegisterObjectType("Vector4", sizeof(Vector4), GetPODFlags<Vector4>());
        
        REGISTER_AXIS(Vector4, x, X);
        REGISTER_AXIS(Vector4, y, Y);
        REGISTER_AXIS(Vector4, z, Z);
        REGISTER_AXIS(Vector4, w, W);
        
        r = engine->RegisterObjectBehaviour("Vector4",
            asBEHAVE_CONSTRUCT,
            "void f(float, float, float, float)",
            asFUNCTIONPR([](float x, float y, float z, float w, Vector4* self) {
                new(self) Vector4(x, y, z, w);
            }, (float, float, float, float, Vector4*), void),
            asCALL_CDECL_OBJLAST);
        assert(r >= 0);
    }

    static void RegisterQuaternion(asIScriptEngine* engine) {
        int r = engine->RegisterObjectType("Quaternion", sizeof(Quaternion), GetPODFlags<Quaternion>());
        
        REGISTER_AXIS(Quaternion, x, X);
        REGISTER_AXIS(Quaternion, y, Y);
        REGISTER_AXIS(Quaternion, z, Z);
        REGISTER_AXIS(Quaternion, w, W);
        
        r = engine->RegisterObjectBehaviour("Quaternion",
            asBEHAVE_CONSTRUCT,
            "void f(float, float, float, float)",
            asFUNCTIONPR([](float x, float y, float z, float w, Quaternion* self) {
                new(self) Quaternion(x, y, z, w);
            }, (float, float, float, float, Quaternion*), void),
            asCALL_CDECL_OBJLAST);
        assert(r >= 0);
    }

    static void RegisterTransform(asIScriptEngine* engine) {
        int r = engine->RegisterObjectType("Transform", sizeof(Transform), GetPODFlags<Transform>());
        assert(r >= 0);
        r = engine->RegisterObjectProperty("Transform", "Vector3 position", offsetof(Transform, position));
        assert(r >= 0);
        r = engine->RegisterObjectProperty("Transform", "Quaternion rotation", offsetof(Transform, rotation));
        assert(r >= 0);
        r = engine->RegisterObjectProperty("Transform", "Vector3 scale", offsetof(Transform, scale));
        assert(r >= 0);
    }

    static void RegisterMatrix(asIScriptEngine* engine) {
        int r;

        constexpr auto TYPE_NAME = "Matrix4x4";

        r = engine->RegisterObjectType(TYPE_NAME, sizeof(Matrix4x4), GetPODFlags<Matrix4x4>());
        assert(r >= 0);

        r = engine->RegisterObjectMethod(TYPE_NAME, "Vector4 get_opIndex(uint idx) const property",
            asMETHOD(Matrix4x4, GetColumn4), asCALL_THISCALL);
        assert(r >= 0);
        r = engine->RegisterObjectMethod(TYPE_NAME, "void set_opIndex(uint idx, Vector4) property",
            asMETHOD(Matrix4x4, SetColumn4), asCALL_THISCALL);
        assert(r >= 0);

        r = engine->RegisterObjectMethod(TYPE_NAME, "Vector3 GetTranslation() const",
            asMETHOD(Matrix4x4, GetTranslation), asCALL_THISCALL);
        assert(r >= 0);

        r = engine->RegisterObjectMethod(TYPE_NAME, "Quaternion GetRotation() const",
            asMETHOD(Matrix4x4, GetQuaternion), asCALL_THISCALL);
        assert(r >= 0);
    }

    #undef REGISTER_AXIS
    
    void RegisterMath(asIScriptEngine* engine) {
        RegisterVector2(engine);
        RegisterVector3(engine);
        RegisterVector4(engine);
        RegisterQuaternion(engine);
        RegisterTransform(engine);
        RegisterMatrix(engine);
    }
}
