/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __GROK3d_TYPES__H
#define __GROK3d_TYPES__H

/* This file forward declares all engine types so they can be used in other headers easily
* As such there are no header guards so:
* DO NOT INCLUDE HEADERS HERE */

constexpr unsigned int IndexToMask(unsigned int index)
{
    return 1 << index;
}

//Forward declarations for headers
namespace Grok3d
{
    enum class GRK_Result
    {
        Ok = 0,
        NoSuchElement,
        NoSpaceRemaining,
        NoSuchEntity,
        EntityAlreadyDeleted,
        EntityAlreadyHasComponent,
        NoSuchComponentManager,
        ComponentAlreadyAdded,
        NoSuchComponentIdentity,
        MustUseComponentHandleToDeleteBehaviour,
    };

    class GRK_World;

    namespace Entities
    {
        class GRK_EntityHandle;

        typedef int GRK_Entity;

        class GRK_EntityManager;
    }

    namespace Components
    {
        class GRK_Component;
        template<class ComponentType> class GRK_ComponentHandle;
        // DOCUMENTATION IMPORTANT : every component must implement a public field for componentManager pointer,
        // this makes templates not fail and it's easy to get the right manager in world etc
        class GRK_ComponentManagerBase;
        template<class ComponentType> class GRK_ComponentManager;

        class GRK_TransformComponent;

        typedef int GRK_ComponentBitMask;
    }
}

#endif
