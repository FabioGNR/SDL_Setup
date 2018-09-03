#pragma once
#include <cstdint>

using ComponentId = uint64_t;

namespace {
    ComponentId getNextId() {
        static ComponentId id = 0;
        return id++;
    }

    template<typename Component>
    ComponentId getNextComponentFamilyId() {
        static ComponentId familyId = getNextId();
        return familyId++;
    }
}

struct IComponent {
};

template<typename Component>
struct BaseComponent : public IComponent {
    static ComponentId familyId() {
        static ComponentId id = ::getNextComponentFamilyId<Component>();
        return id;
    }
};
