#pragma once

#include <bitset>
#include <cstdint>
#include <type_traits>

#include "Component.h"

using EntityId = uint64_t;

namespace {
EntityId getNextEntityId() {
    static EntityId id = 0;
    return id++;
}
}  // namespace

struct Entity {
    const EntityId id = ::getNextEntityId();
    std::bitset<32> signature;

    template <typename Component>
    bool hasComponent() {
        static_assert(std::is_base_of<IComponent, Component>::value,
                      "Component must be inherited from BaseComponent");
        return signature[Component::familyId()];
    }

    template <typename FirstComponent, typename SecondComponent,
              typename... Rest>
    bool hasComponent() {
        return (hasComponent<FirstComponent>() &&
                hasComponent<SecondComponent, Rest...>());
    }

    template <typename Component>
    void addComponent() {
        static_assert(std::is_base_of<IComponent, Component>::value,
                      "Component must be inherited from BaseComponent");
        signature.set(Component::familyId());
    }
};
