//
// Created by ryen on 2/25/25.
//

#include <spdlog/spdlog.h>

#include "MECore/Resource/Mesh/Mesh.h"

#include "MECore/MECore.h"
#include "MECore/Render/IRenderInterface.h"

namespace ME {
    Mesh::ReallocResult Mesh::ReallocateBuffer(std::shared_ptr<IBuffer>& buffer, IMeshStream* stream) {
        auto device = MEInstance::Instance()->GetServiceProvider()->GetRenderInterface()->GetDevice();
        if (device == nullptr) {
            return ReallocResult::Error;
        }
        
        if (buffer != nullptr) {
            auto desc = buffer->GetDesc();
            if (desc.Size == stream->GetSize()) return ReallocResult::Nothing;
        }

        bool isIndex = stream->GetStreamType() == MeshStreamType::Index;

        BufferDesc desc;
        desc.Name = stream->GetName().c_str();
        desc.Usage = USAGE_DEFAULT;
        desc.BindFlags = isIndex ? BIND_INDEX_BUFFER : BIND_VERTEX_BUFFER;
        desc.Size = stream->GetSize();
        
        BufferData data;
        data.pData = stream->GetData();
        data.DataSize = stream->GetSize();
        
        auto ptr = buffer.get();
        device->CreateBuffer(desc, &data, &ptr);
        buffer = std::shared_ptr<IBuffer>(ptr);
        return buffer != nullptr ? ReallocResult::Success : ReallocResult::Error;
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
        auto interface = MEInstance::Instance()->GetServiceProvider()->GetRenderInterface();
        if (interface != nullptr) {
            wantsUpload = true;
        }
    }
    
    void Mesh::DoUpload(IDeviceContext* ctx) {
        for (const auto& stream : streams) {
            auto& buffer = gpuBuffers[stream.get()];
            auto result = ReallocateBuffer(buffer, stream.get());
            if (result == ReallocResult::Error) {
                spdlog::error("Failed to allocate a new buffer for a mesh stream!");
            } else if (result == ReallocResult::Nothing) {
                void* map;
                ctx->MapBuffer(buffer.get(), MAP_WRITE, MAP_FLAG_DISCARD, map);
                memcpy(map, stream->GetData(), stream->GetSize());
                ctx->UnmapBuffer(buffer.get(), MAP_WRITE);
            }
        }
        wantsUpload = false;
    }
}
