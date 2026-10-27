//
// Created by dotryen on 1/3/26.
//

#include "MECore/Math/Types.h"
#include "MECore/Math/Transform.h"
#include "MECore/Math/Bindings.h"

namespace ME {
    template<class T>
    constexpr asQWORD GetPODFlags() {
        return asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<T>() | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_MORE_CONSTRUCTORS;
    }
    
    #define REGISTER_AXIS(type, axis, axisUpper) \
        engine->RegisterObjectMethod(#type, "float get_" #axis "() property", asMETHOD(type, Get##axisUpper), asCALL_THISCALL); \
        engine->RegisterObjectMethod(#type, "void set_" #axis "() property", asMETHOD(type, Set##axisUpper), asCALL_THISCALL)
    
    void RegisterVector2(asIScriptEngine* engine) {
        engine->RegisterObjectType("Vector2", sizeof(Vector2), GetPODFlags<Vector2>());
    }
    
    void RegisterVector3(asIScriptEngine* engine) {
        engine->RegisterObjectType("Vector3", sizeof(Vector3), GetPODFlags<Vector3>());
        // Cant do this because of W
        // Jolt vectors are actually arrays under the hood. So there is no field to get the offset of.
        // engine->RegisterObjectProperty("Vector3", "float x", 0); 
        // engine->RegisterObjectProperty("Vector3", "float y", 4);
        // engine->RegisterObjectProperty("Vector3", "float z", 8);
        
        REGISTER_AXIS(Vector3, x, X);
        REGISTER_AXIS(Vector3, y, Y);
        REGISTER_AXIS(Vector3, z, Z);
        
        engine->RegisterObjectBehaviour("Vector3", 
            asBEHAVE_CONSTRUCT, 
            "void f(float, float, float)", 
            asFUNCTIONPR([](float x, float y, float z, Vector3* self) {
                new(self) Vector3(x, y, z);
            }, (float, float, float, Vector3*), void),
            asCALL_CDECL_OBJLAST);
    }

    void RegisterVector4(asIScriptEngine* engine) {
        engine->RegisterObjectType("Vector4", sizeof(Vector4), GetPODFlags<Vector4>());
        
        REGISTER_AXIS(Vector4, x, X);
        REGISTER_AXIS(Vector4, y, Y);
        REGISTER_AXIS(Vector4, z, Z);
        REGISTER_AXIS(Vector4, w, W);
        
        engine->RegisterObjectBehaviour("Vector4",
            asBEHAVE_CONSTRUCT,
            "void f(float, float, float, float)",
            asFUNCTIONPR([](float x, float y, float z, float w, Vector4* self) {
                new(self) Vector4(x, y, z, w);
            }, (float, float, float, float, Vector4*), void),
            asCALL_CDECL_OBJLAST);
    }
    
    void RegisterQuaternion(asIScriptEngine* engine) {
        engine->RegisterObjectType("Quaternion", sizeof(Quaternion), GetPODFlags<Quaternion>());
        
        REGISTER_AXIS(Quaternion, x, X);
        REGISTER_AXIS(Quaternion, y, Y);
        REGISTER_AXIS(Quaternion, z, Z);
        REGISTER_AXIS(Quaternion, w, W);
        
        engine->RegisterObjectBehaviour("Quaternion",
            asBEHAVE_CONSTRUCT,
            "void f(float, float, float, float)",
            asFUNCTIONPR([](float x, float y, float z, float w, Quaternion* self) {
                new(self) Quaternion(x, y, z, w);
            }, (float, float, float, float, Quaternion*), void),
            asCALL_CDECL_OBJLAST);
    }
    
    void RegisterTransform(asIScriptEngine* engine) {
        engine->RegisterObjectType("Transform", sizeof(Transform), GetPODFlags<Transform>());
        engine->RegisterObjectProperty("Transform", "Vector3 position", offsetof(Transform, position));
        engine->RegisterObjectProperty("Transform", "Quaternion rotation", offsetof(Transform, rotation));
        engine->RegisterObjectProperty("Transform", "Vector3 scale", offsetof(Transform, scale));
    }
    
    void RegisterMathBindings(asIScriptEngine* engine) {
        RegisterVector2(engine);
        RegisterVector3(engine);
        RegisterVector4(engine);
        RegisterQuaternion(engine);
        RegisterTransform(engine);
    }
}
