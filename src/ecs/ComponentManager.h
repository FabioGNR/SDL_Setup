#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "Entity.h"

template <typename Component>
class ComponentManager {
    using ComponentMap = std::unordered_map<EntityId, std::unique_ptr<Component>>;
    ComponentMap _map;

   public:
    template <typename... ComponentArgs>
    Component* add(Entity* entity, ComponentArgs&&... args) {
        auto component = std::make_unique<Component>(args...);
        auto ptr = component.get();

        const auto valid =
            _map.insert({entity->id, std::move(component)}).second;

        if (valid) {
            entity->addComponent<Component>();
            return ptr;
        } else {
            return nullptr;
        }
    }

    Component* get(const Entity* entity) const {
        return _map.at(entity->id).get();
    }
    Component* get(const EntityId entityId) const {
        return _map.at(entityId).get();
    }

    ComponentMap& getAll() { return _map; }
};
