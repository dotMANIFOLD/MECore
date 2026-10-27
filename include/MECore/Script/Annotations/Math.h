//
// Created by dotryen on 10/27/26.
//

#pragma once
#include "daScript/ast/ast_handle.h"
#include "MECore/Math/Transform.h"
#include "MECore/Math/Types.h"

using namespace das;
using namespace ME;

#define MAKE_VALUE_CAST(type) \
    template<> \
    struct cast<type> { \
        static type to(vec4f v) { union { type custom; vec4f native; } u; u.native = v; return u.custom; } \
        static vec4f from(type v) { union { type custom; vec4f native; } u; u.custom = v; return u.native; }\
    };

#define MAKE_VALUE_ANNOTATION(type) \
    struct type##Annotation : ManagedValueAnnotation<type> { \
        type##Annotation(ModuleLibrary& lib) : ManagedValueAnnotation(lib, #type) { } \
    };

#define ANNOTATE(type) \
    MAKE_CAST(type) \
    MAKE_ANNOTATION(type) \

template<>
struct cast<Vector3> {
    static Vector3 to(vec4f v) { union { Vector3 vec; vec4f native; } u; u.native = v; return u.vec; }
    static vec4f from(Vector3 v) { union { Vector3 vec; vec4f native; } u; u.vec = v; return u.native; }
};

struct Vector3Annotation : ManagedStructureAnnotation<Vector3> {
    Vector3Annotation(ModuleLibrary& lib) : ManagedStructureAnnotation("Vector3", lib) {
        addProperty<DAS_BIND_MANAGED_PROP(GetX)>("x");
        addProperty<DAS_BIND_MANAGED_PROP(GetY)>("y");
        addProperty<DAS_BIND_MANAGED_PROP(GetZ)>("z");
        addProperty<DAS_BIND_MANAGED_PROP(Length)>("length");
    }
};

struct TransformAnnotation : ManagedStructureAnnotation<Transform> {
    TransformAnnotation(ModuleLibrary& lib) : ManagedStructureAnnotation("Transform", lib) {
        addField<DAS_BIND_MANAGED_FIELD(position)>("position", "position");
        addField<DAS_BIND_MANAGED_FIELD(rotation)>("rotation", "rotation");
        addField<DAS_BIND_MANAGED_FIELD(scale)>("scale", "scale");
    }
};

MAKE_TYPE_FACTORY(Vector3, Vector3)
MAKE_TYPE_FACTORY(Vector4, Vector4)
MAKE_TYPE_FACTORY(Quaternion, Quaternion)
MAKE_TYPE_FACTORY(Transform, Transform)

#undef ANNOTATE
#undef MAKE_VALUE_CAST
#undef MAKE_VALUE_ANNOTATION
