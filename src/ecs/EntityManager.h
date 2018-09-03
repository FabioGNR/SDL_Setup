#pragma once

#include <memory>
#include <vector>

#include "Entity.h"

class EntityManager {
   public:
    std::vector<std::unique_ptr<Entity>> _entities;
    Entity* create() {
        auto entity = std::make_unique<Entity>();
        auto* ptr = entity.get();

        _entities.push_back(std::move(entity));

        return ptr;
    }
};
