//
// Created by ryen on 2/20/25.
//

#pragma once

#include <vector>
#include <memory>
#include <nvrhi/nvrhi.h>

#include "IMeshStream.h"
#include "MECore/Math/Types.h"
#include "MECore/Resource/Resource.h"

namespace ME {
    /// A single mesh.
    class Mesh : public Resource, public std::enable_shared_from_this<Mesh> {
        private:
        std::vector<std::unique_ptr<IMeshStream>> streams;
        
        bool wantsUpload;
        std::unordered_map<IMeshStream*, nvrhi::BufferHandle> gpuBuffers;

        bool ReallocateBuffer(nvrhi::BufferHandle& buffer, IMeshStream* stream);
    
        public:
        using Pointer = std::shared_ptr<Mesh>;
        using WeakPointer = std::weak_ptr<Mesh>;

        Mesh() = default;
        
        const std::vector<std::unique_ptr<IMeshStream>>& GetAllStreams() const { return streams; }
        IMeshStream* GetStreamBySemantic(const std::string& semantic) const {
            for (const auto& stream : streams) {
                if (stream->GetSemantic() == semantic) return stream.get();
            }
            return nullptr;
        }
        nvrhi::BufferHandle GetBufferBySemantic(const std::string& semantic) const {
            for (const auto& stream : streams) {
                if (stream->GetSemantic() == semantic) return gpuBuffers.at(stream.get());
            }
            return nullptr;
        }

        // COMMON STREAMS AND BUFFERS
        IMeshStream* GetPositionStream() const {
            return GetStreamBySemantic("POSITION");
        }
        nvrhi::BufferHandle GetPositionBuffer() const {
            return GetBufferBySemantic("POSITION");
        }

        IMeshStream* GetIndexStream() const {
            return GetStreamBySemantic("INDEX");
        }
        nvrhi::BufferHandle GetIndexBuffer() const {
            return GetBufferBySemantic("INDEX");
        }

        /// Adds a stream to this mesh.
        /// 
        /// The mesh assumes ownership over the stream.
        void AddStream(IMeshStream* stream);
        void RemoveStream(IMeshStream* stream);
        
        /// Triggers a flag to upload the mesh to the GPU.
        /// 
        /// The mesh should be uploaded before this frame is rendered.
        /// (GPU buffers are handled by the RenderPipeline, this cannot be guaranteed)
        void Upload();
        bool WantsUpload() const { return wantsUpload; }
        
        /// Function to upload all mesh streams to the GPU.
        void DoUpload(nvrhi::ICommandList* cmd);
    };
}
