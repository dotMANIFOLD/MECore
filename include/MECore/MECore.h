//
// Created by ryen on 12/29/24.
//

#pragma once

#include <spdlog/spdlog.h>

#include "Service/IServiceProvider.h"

// Platform defines derived from WickedEngine. Thank you turanszkij.
// WickedEngine is licensed under the MIT license.

#ifdef _WIN32
    #if WINAPI_FAMILY == WINAPI_FAMILY_GAMES
        #define PLATFORM_XBOX
    #else
        #define PLATFORM_WINDOWS
    #endif
#elif defined(__SCE__)
    #define PLATFORM_PS5
#else
    #define PLATFORM_LINUX
#endif

namespace ME {
    struct MEAppData {
        std::string name;
        std::string company;
        std::string version;
    };
    
    class MEInstance {
        private:
        static inline MEInstance* globalInstance;
        
        MEAppData appData;
        IServiceProvider* serviceProvider;
        
        public:
        static MEInstance* Instance() { return globalInstance; }
        
        MEInstance() = delete;
        
        /// Create an instance
        /// @param appData Info about this app.
        /// @param provider A service provider for different engine functions. The pointer is still owned by the program and is expected to be alive until the instance is destroyed.
        MEInstance(const MEAppData& appData, IServiceProvider* provider) {
            this->appData = appData;
            this->serviceProvider = provider;
        }
        ~MEInstance() = default;
        
        [[nodiscard]] const MEAppData& GetAppData() const { return appData; }
        [[nodiscard]] IServiceProvider* GetServiceProvider() const { return serviceProvider; }
        
        void Initialize() const { serviceProvider->Initialize(); }
        
        void MakeGlobal() { globalInstance = this; }
    };
}
