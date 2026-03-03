//
// Created by ryen on 3/3/25.
//

#pragma once

#include "Asset.h"
#include "MECore/resource/Shader.h"

namespace ME::asset {
    class Shader : public Asset {
        public:
        virtual std::shared_ptr<Resource::Shader> CreateShader() const = 0;
    };
}
