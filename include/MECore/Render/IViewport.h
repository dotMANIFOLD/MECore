//
// Created by ryen on 2/25/25.
//

#pragma once

namespace ME {
    class IViewport {
        public:
        virtual ~IViewport() = default;

        virtual void GetSize(int* width, int* height) const = 0;
        float GetAspect() const {
            int width, height;
            GetSize(&width, &height);
            return static_cast<float>(width) / static_cast<float>(height);
        }
    };
}
