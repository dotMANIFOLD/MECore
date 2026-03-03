//
// Created by dotryen on 1/12/26.
//

#pragma once

namespace ME {
    class IValid {
        public:
        virtual ~IValid() = default;
        
        virtual bool IsValid() const = 0;
    };
}
