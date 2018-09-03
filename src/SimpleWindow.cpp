//
// Created by Zain on 27/08/2018.
//

#include <SDL.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "SimpleWindow.h"

#include "ecs/Entity.h"
#include "ecs/EntityManager.h"

#include "ecs/Component.h"
#include "ecs/ComponentManager.h"

#include "ecs/System.h"

struct PosComponent : BaseComponent<PosComponent> {
    int x, y;
    PosComponent(int x, int y) : x(x), y(y) {}
};

struct PhyComponent : BaseComponent<PhyComponent> {
    int w, h;
    bool isStatic;
    PhyComponent(int w, int h, bool isStatic)
        : w(w), h(h), isStatic(isStatic) {}
};

struct VelComponent : BaseComponent<VelComponent> {
    float x, y;
    VelComponent(float x, float y) : x(x), y(y) {}
};

struct World {
    EntityManager entities;

    template <typename... Components>
    void onEntityWith(std::function<void(Entity* entity)>&& func) {
        for (auto& entity : entities._entities) {
            if (entity->hasComponent<Components...>()) {
                func(entity.get());
            }
        }
    }
};

class MovementSystem : public System {
    World& world;
    ComponentManager<PosComponent>& posManager;
    ComponentManager<VelComponent>& velManager;

   public:
    MovementSystem(World& world, ComponentManager<PosComponent>& pos,
                   ComponentManager<VelComponent>& vel)
        : world(world), posManager(pos), velManager(vel) {}

    void update(float dt) override {
        world.onEntityWith<PosComponent, VelComponent>([&](Entity* entity) {
            auto* pos = posManager.get(entity);
            auto* vel = velManager.get(entity);

            pos->x += vel->x;
            pos->y += vel->y;

            if (pos->x > 640 * 2 + 10) pos->x = 0;
            else if (pos->x < 0 - 10) pos->x = 0;
            if (pos->y > 480 * 2 + 10) pos->y = 0;
            else if (pos->y < 0 - 10 ) pos->y = 0;
        });
    }
};

class PhysicSystem : public System {
    World& world;
    ComponentManager<VelComponent>& velManager;
    ComponentManager<PhyComponent>& phyManager;
    ComponentManager<PosComponent>& posManager;

   public:
    PhysicSystem(World& world, ComponentManager<VelComponent>& vel,
                 ComponentManager<PhyComponent>& phy,
                 ComponentManager<PosComponent>& pos)
        : world(world), velManager(vel), phyManager(phy), posManager(pos) {}

    void update(float dt) override {
        const float maxVelocity = 10.f;
        world.onEntityWith<VelComponent, PhyComponent>([&](Entity* entity) {
            auto* phy = phyManager.get(entity);
            auto* pos = posManager.get(entity);
            auto* vel = velManager.get(entity);

            if (phy->isStatic) return;

            SDL_Rect thisRect = {pos->x, pos->y, phy->w, phy->h};

            float newVel = vel->y + 2.f;
            vel->y = newVel > maxVelocity ? maxVelocity : newVel;


            for (auto& other : phyManager.getAll()) {
                auto *otherPhy = phyManager.get(other.first);
                auto *otherPos = posManager.get(other.first);

                if (phy == otherPhy) continue;

                SDL_Rect otherRect = {otherPos->x, otherPos->y, otherPhy->w,
                                      otherPhy->h};
                if (SDL_HasIntersection(&thisRect, &otherRect)) {
                    vel->y = 0;
                }
            }
        });
    }
};

class RenderSystem : public System {
    World& world;
    ComponentManager<PosComponent>& posManager;
    ComponentManager<PhyComponent>& phyManager;
    SDL_Renderer& renderer;

   public:
    RenderSystem(World& world, ComponentManager<PosComponent>& pos,
                 ComponentManager<PhyComponent>& phy, SDL_Renderer& render)
        : world(world), posManager(pos), phyManager(phy), renderer(render) {}

    void update(float dt) override {
        SDL_RenderClear(&renderer);
        world.onEntityWith<PosComponent>([&](Entity* entity) {
            auto* pos = posManager.get(entity);
            auto* phy = phyManager.get(entity);

            SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255);

            SDL_Rect rect;
            rect.x = pos->x;
            rect.y = pos->y;
            rect.w = phy->w;
            rect.h = phy->h;

            SDL_RenderFillRect(&renderer, &rect);
            SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 0);
        });
        SDL_RenderPresent(&renderer);
    }
};

void SimpleWindow::run() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL Setup",  // Window title
                                          SDL_WINDOWPOS_CENTERED,  // X pos
                                          SDL_WINDOWPOS_CENTERED,  // Y Pos
                                          640 * 2, 480 * 2,  // Width, Height
                                          SDL_WINDOW_SHOWN   // Window Flag
    );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;

    World world;

    ComponentManager<PosComponent> posManager;
    ComponentManager<VelComponent> velManager;
    ComponentManager<PhyComponent> phyManager;

    MovementSystem movSystem(world, posManager, velManager);
    RenderSystem renSystem(world, posManager, phyManager, *renderer);
    PhysicSystem phySystem(world, velManager, phyManager, posManager);

    Entity* groundEntity = world.entities.create();
    posManager.add(groundEntity, 640/2, 620);
    phyManager.add(groundEntity, 640, 20, true);

    Entity* box = world.entities.create();
    posManager.add(box, 640/2, 0);
    phyManager.add(box, 50, 50, false);
    velManager.add(box, 0, 0);

    bool run = true;
    while (run) {
        auto start = SDL_GetTicks();
        float velx, vely;
        velx = vely = 0;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_a)
                    velx += -2.f;
                else if (event.key.keysym.sym == SDLK_d)
                    velx += 2.f;
                if (event.key.keysym.sym == SDLK_w)
                    vely += -2.f;
                else if (event.key.keysym.sym == SDLK_s)
                    vely += 2.f;
            }
            auto *pos = velManager.get(box->id);
            pos->x += velx;
            pos->y += vely;
        }

        phySystem.update(0.0f);
        movSystem.update(0.0f);
        renSystem.update(0.0f);

        if (1000.0 / 60 > SDL_GetTicks() - start)
            SDL_Delay(1000.0 / 60 - (SDL_GetTicks() - start));
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}
