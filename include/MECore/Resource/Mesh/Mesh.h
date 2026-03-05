//
// Created by ryen on 2/20/25.
//

#pragma once

#include <vector>
#include <memory>
#include <Buffer.h>

#include "IMeshStream.h"
#include "MECore/Math/Types.h"
#include "MECore/Resource/Resource.h"

using namespace Diligent;

namespace ME {
    /// A single mesh.
    class Mesh : public Resource, public std::enable_shared_from_this<Mesh> {
        private:
        enum class ReallocResult {
            Nothing, Success, Error
        };
        
        std::vector<std::unique_ptr<IMeshStream>> streams;
        
        bool wantsUpload;
        std::unordered_map<IMeshStream*, std::shared_ptr<IBuffer>> gpuBuffers;

        ReallocResult ReallocateBuffer(std::shared_ptr<IBuffer>& buffer, IMeshStream* stream);
    
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
        std::shared_ptr<IBuffer> GetBufferBySemantic(const std::string& semantic) const {
            for (const auto& stream : streams) {
                if (stream->GetSemantic() == semantic) return gpuBuffers.at(stream.get());
            }
            return nullptr;
        }

        // COMMON STREAMS AND BUFFERS
        IMeshStream* GetPositionStream() const {
            return GetStreamBySemantic("POSITION");
        }
        std::shared_ptr<IBuffer> GetPositionBuffer() const {
            return GetBufferBySemantic("POSITION");
        }

        IMeshStream* GetIndexStream() const {
            return GetStreamBySemantic("INDEX");
        }
        std::shared_ptr<IBuffer> GetIndexBuffer() const {
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
        void DoUpload(IDeviceContext* ctx);
    };
}
