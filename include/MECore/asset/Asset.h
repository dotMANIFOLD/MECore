#pragma once

#include <uuid.h>

namespace ME::asset {
    class Asset {
        private:
        uuids::uuid uuid;
        std::string name;

        protected:
        bool loaded;
        
        public:
        Asset() = default;
        explicit Asset(uuids::uuid uuid) {
            this->uuid = uuid;
        }
        virtual ~Asset() = default;

        uuids::uuid GetUUID() const { return uuid; }

        std::string GetName() const { return name; }
        void SetName(const std::string& name) { this->name = name; }
    };
}
