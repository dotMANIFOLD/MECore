//
// Created by dotryen on 10/27/26.
//

#pragma once

#include <daScript/daScript.h>
#include <daScript/ast/ast.h>

#include "Annotations/Math.h"

class Module_MECore : public Module {
    public:
    Module_MECore() : Module("MECore") {
        ModuleLibrary lib(this);
        addAnnotation(new Vector3Annotation(lib));
    }
};

REGISTER_MODULE(Module_MECore);
