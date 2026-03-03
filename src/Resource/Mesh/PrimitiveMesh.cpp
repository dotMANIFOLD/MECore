//
// Created by dotryen on 12/19/25.
//

#include <memory>

#include "MECore/Resource/Mesh/Mesh.h"
#include "MECore/Resource/Mesh/PositionMeshStream.h"
#include "MECore/Resource/Mesh/IndexMeshStream.h"
#include "MECore/Resource/Mesh/PrimitiveMesh.h"

namespace ME {
    namespace PrimitiveMesh {
        static std::shared_ptr<Mesh> cubeMesh = nullptr;
        static std::shared_ptr<Mesh> planeMesh = nullptr;
        
        std::shared_ptr<Mesh> Cube() {
            if (!cubeMesh) {
                auto posStream = new PositionMeshStream();
                posStream->data = {
                    { -1, -1, -1 },
                    { 1, -1, -1 },
                    { 1, 1, -1 },
                    { -1, 1, -1 },
                    { -1, -1, 1 },
                    { 1, -1, 1 },
                    { 1, 1, 1 },
                    { -1, 1, 1 },
                };
                posStream->isDirty = true;
            
                auto indexStream = new IndexMeshStream();
                indexStream->data = {
                    0, 1, 3, 3, 1, 2,
                    1, 5, 2, 2, 5, 6,
                    5, 4, 6, 6, 4, 7,
                    4, 0, 7, 7, 0, 3,
                    3, 2, 7, 7, 2, 6,
                    4, 5, 0, 0, 5, 1
                };
                indexStream->isDirty = true;
            
                auto mesh = std::make_shared<Mesh>();
                mesh->AddStream(posStream);
                mesh->AddStream(indexStream);
                
                cubeMesh = mesh;
            }
            return cubeMesh;
        }
        
        std::shared_ptr<Mesh> Plane() {
            if (!planeMesh) {
                auto posStream = new PositionMeshStream();
                posStream->data = {
                    { -1, 0, -1 },
                    { 1, 0, -1 },
                    { -1, 0, 1 },
                    { 1, 0, 1 },
                };
                posStream->isDirty = true;
                
                auto indexStream = new IndexMeshStream();
                indexStream->data = {
                    0, 1, 2, 2, 1, 3
                };
                indexStream->isDirty = true;
                
                auto mesh = std::make_shared<Mesh>();
                mesh->AddStream(posStream);
                mesh->AddStream(indexStream);
                
                planeMesh = mesh;
            }
            return planeMesh;
        }
    }
}
