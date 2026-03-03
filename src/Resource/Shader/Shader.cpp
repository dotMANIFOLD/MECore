//
// Created by ryen on 3/1/25.
//

#include "MECore/Resource/Shader/Shader.h"

namespace ME {
    void Shader::SetGlobalBlockProperty(const std::string& name, const void* data, int dataSize) {
        globalPropertiesHelper.SetBlockProperty(globalProperties, name, data, dataSize);
    }
}
