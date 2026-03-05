//
// Created by ryen on 3/27/25.
//

#pragma once

#include "Resource.h"

namespace ME {
    // class Texture : public Resource {
    //     private:
    //     nvrhi::TextureHandle texture;
// 
    //     bool ReallocateTexture();
// 
    //     protected:
    //     virtual nvrhi::TextureDesc CreateDescriptor() = 0;
// 
    //     public:
    //     virtual nvrhi::TextureDimension GetDimension() const = 0;
    //     virtual nvrhi::Format GetFormat() const = 0;
    //     nvrhi::TextureHandle GetGPUTexture() const { return texture; }
// 
    //     virtual const void* GetData() const = 0;
// 
    //     virtual bool IsDirty() const = 0;
    //     virtual void ClearDirty() = 0;
// 
    //     bool UpdateTexture();
    //     void Upload();
    //     void AddUpload(nvrhi::ICommandList* cmd);
    // };
// 
    // class RawTexture : public Texture {
    //     private:
    //     nvrhi::Format format;
// 
    //     public:
    //     // This data is assumed to be controlled by the user and will not be freed by the texture.
    //     void* data;
    //     size_t size;
    //     bool isDirty;
// 
    //     explicit RawTexture(nvrhi::Format format) {
    //         this->format = format;
// 
    //         data = nullptr;
    //         size = 0;
    //         isDirty = false;
    //     }
// 
    //     nvrhi::Format GetFormat() const override { return format; }
// 
    //     const void* GetData() const override { return data; }
// 
    //     bool IsDirty() const override { return isDirty; }
    //     void ClearDirty() override { isDirty = false; }
    // };
// 
    // class Texture2D : public Texture {
    //     private:
    //     int width, height;
    //     nvrhi::Format format;
// 
    //     protected:
    //     nvrhi::TextureDesc CreateDescriptor() override {
    //         return nvrhi::TextureDesc().setWidth(width).setHeight(height);
    //     }
// 
    //     public:
    //     void* data;
    //     bool isDirty;
// 
    //     Texture2D(int width, int height, nvrhi::Format format) {
    //         this->width = width;
    //         this->height = height;
    //         this->format = format;
// 
    //         data = nullptr;
    //         isDirty = false;
    //     }
// 
    //     nvrhi::TextureDimension GetDimension() const override { return nvrhi::TextureDimension::Texture2D; }
    //     nvrhi::Format GetFormat() const override { return format; }
// 
    //     const void* GetData() const override { return data; }
// 
    //     bool IsDirty() const override { return isDirty; }
    //     void ClearDirty() override { isDirty = false; }
    // };
}
