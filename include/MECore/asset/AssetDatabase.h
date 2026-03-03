//
// Created by dotryen on 8/12/25.
//

#pragma once
#include <unordered_map>

#include "Asset.h"
#include "uuid.h"

namespace ME::asset {
    class AssetDatabase {
        private:
        std::unordered_map<uuids::uuid, std::shared_ptr<Asset>> uuidToAsset;
        std::unordered_map<std::string, std::shared_ptr<Asset>> pathToAsset;
    };
}
