//
// Created by dotryen on 12/19/25.
//

#pragma once

#include <memory>

#include "MECore/Service/IService.h"
#include "IRenderTarget.h"

namespace ME {
    class IRenderPipeline : public IService {
        public:
        // virtual void Render(RenderScene* scene, RenderCamera* camera);
        
        virtual std::shared_ptr<IRenderTarget> GetMainRenderTarget() = 0;
        virtual void SetMainRenderTarget(std::shared_ptr<IRenderTarget> renderTarget) = 0;
    };
}
