/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __ENTITYHANDLER__H

#define __ENTITYHANDLER__H

#include "grok3d_types.h"

namespace Grok3d { namespace Entities 
{
    class GRK_EntityHandle
    {
    template<class T>
    friend class Grok3d::Components::GRK_ComponentHandle;
    public:
        GRK_EntityHandle(Grok3d::GRK_World* world, Grok3d::Entities::GRK_Entity entity);

        Grok3d::GRK_Result Destroy();
        bool inline IsDestroyed();

        template<class ComponentType>
        Grok3d::GRK_Result AddComponent(ComponentType& component);

        template<class ComponentType>
        Grok3d::GRK_Result RemoveComponent();

        template<class ComponentType>
        ComponentType* GetComponent();

        // STRETCH 1: change this to just take in template of which component
        bool inline HasComponents(int componentBits);

    private:
        Grok3d::Entities::GRK_Entity m_entity;
        Grok3d::GRK_World* m_world;
    };
} /*Entities*/ } /*Grok3d*/

#endif
