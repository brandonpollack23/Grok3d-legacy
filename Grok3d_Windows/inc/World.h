/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __WORLD__H

#define __WORLD__H

#include "grok3d_types.h"

#include "Entity/EntityManager.h"

#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"

#include <vector>

namespace Grok3d 
{
    class GRK_World
    {
    //make EntityHandle a friend so it can access the component creation methods, but nothing outside the engine can
    //this ensures the bitmask of components will always be up to date
    friend class Entities::GRK_EntityHandle;

    public:
        GRK_World();

        Grok3d::Entities::GRK_EntityHandle CreateEntity();

    private:
        Grok3d::GRK_Result DeleteEntity(Grok3d::Entities::GRK_Entity entity);

        template<class ComponentType>
        Grok3d::Components::GRK_ComponentManager<ComponentType>* GetComponentManager();

        //these component functions can only be called from GRK_EntityHandle or GRK_World!!!
        template<class ComponentType>
        Grok3d::GRK_Result AddComponent(Grok3d::Entities::GRK_Entity entity, ComponentType& newComponent);

        template<class ComponentType>
        ComponentType* GetComponent(Grok3d::Entities::GRK_Entity entity);

        template<class ComponentType>
        Grok3d::GRK_Result RemoveComponent(Grok3d::Entities::GRK_Entity entity);


    private:
        Grok3d::Entities::GRK_EntityManager m_entityManager;

        Grok3d::Components::GRK_ComponentManager<Grok3d::Components::GRK_TransformComponent> m_translationComponentManager;
        //TODO inf pointer to each componentmanager, do a static array of them and use their defines, populate it in constructor and see source line 50

        std::vector<Grok3d::Components::GRK_ComponentManagerBase*> m_componentManagers;
    };
} /*Grok3d*/

#endif