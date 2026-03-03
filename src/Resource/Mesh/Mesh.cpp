//
// Created by ryen on 2/25/25.
//

#include <spdlog/spdlog.h>

#include "MECore/Resource/Mesh/Mesh.h"

#include "MECore/MECore.h"
#include "MECore/Render/IRenderInterface.h"

namespace ME {
    bool Mesh::ReallocateBuffer(nvrhi::BufferHandle& buffer, IMeshStream* stream) {
        if (buffer != nullptr) {
            if (buffer->getDesc().byteSize == stream->GetSize()) return true;
        }

        bool isIndex = stream->GetStreamType() == MeshStreamType::Index;

        nvrhi::BufferDesc desc;
        desc.byteSize = stream->GetSize();
        desc.structStride = 0;
        desc.canHaveUAVs = false;
        desc.isVertexBuffer = !isIndex;
        desc.isIndexBuffer = isIndex;
        desc.isDrawIndirectArgs = false;
        desc.isVolatile = false;
        desc.initialState = isIndex ? nvrhi::ResourceStates::IndexBuffer : nvrhi::ResourceStates::VertexBuffer;
        desc.keepInitialState = true;
        desc.debugName = stream->GetName().c_str();

        // TODO: add null checks
        buffer = MEInstance::Instance()->GetServiceProvider()->GetRenderService()->GetInterface()->GetDevice()->createBuffer(desc);
        return buffer != nullptr;
    }

    void Mesh::AddStream(IMeshStream* stream) {
        streams.push_back(std::unique_ptr<IMeshStream>(stream));
        gpuBuffers.insert({ stream, nullptr });
    }
    
    void Mesh::RemoveStream(IMeshStream* stream) {
        streams.erase(std::ranges::find_if(streams,
        [stream](const std::unique_ptr<IMeshStream>& ptr) {
            return ptr.get() == stream;
        }));
        gpuBuffers.erase(stream);
    }

    void Mesh::Upload() {
        auto service = MEInstance::Instance()->GetServiceProvider()->GetRenderService();
        if (service != nullptr) {
            wantsUpload = true;
        }
    }
    
    void Mesh::DoUpload(nvrhi::ICommandList* cmd) {
        for (const auto& stream : streams) {
            auto buffer = gpuBuffers[stream.get()];
            if (!ReallocateBuffer(buffer, stream.get())) {
                spdlog::error("Failed to allocate a new buffer for a mesh stream!");
            }
            
            cmd->writeBuffer(buffer, stream->GetData(), stream->GetSize());
            gpuBuffers[stream.get()] = buffer;
        }
        wantsUpload = false;
    }
}
