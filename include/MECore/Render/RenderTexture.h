//
// Created by ryen on 2/25/25.
//

#pragma once

#include "IRenderTarget.h"

namespace ME {
    class RenderTexture : public IRenderTarget {
        private:
        uint32_t width, height;
        nvrhi::Format colorFormat;
        nvrhi::Format depthFormat;

        nvrhi::TextureHandle colorTexture;
        nvrhi::TextureHandle depthTexture;

        nvrhi::FramebufferHandle framebuffer;

        void CreateResources();

        public:
        RenderTexture() = default;
        RenderTexture(uint32_t width, uint32_t height, nvrhi::Format colorFormat = nvrhi::Format::BGRA8_UNORM, nvrhi::Format depthFormat = nvrhi::Format::D16);
        ~RenderTexture() override;

        void GetSize(int* width, int* height) const override {
            *width = this->width;
            *height = this->height;
        }
        bool CanRender() const override {
            return framebuffer != nullptr;
        }
        nvrhi::FramebufferHandle GetFramebuffer() const override {
            return framebuffer;
        }
        nvrhi::ITexture* GetColorTexture() const {
            return colorTexture;
        }
        nvrhi::ITexture* GetDepthTexture() const {
            return depthTexture;
        }

        void SetSize(uint32_t width, uint32_t height);
    };
}
