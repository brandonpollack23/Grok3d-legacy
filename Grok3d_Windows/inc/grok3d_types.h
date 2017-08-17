/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __GROK3d_TYPES__H
#define __GROK3d_TYPES__H

/* This file forward declares all engine types so they can be used in other headers easily
* As such there are no header guards so:
* DO NOT INCLUDE HEADERS HERE */

//configure GLM
#define GLM_PRECISION_HIGHP_FLOAT

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

    class GRK_EntityComponentManager;

    namespace Entities
    {
        class GRK_EntityHandle;

        typedef int GRK_Entity;
    }

    namespace Components
    {
        template<class ComponentType> class GRK_ComponentHandle;

        class GRK_Component;
        class GRK_TransformComponent;
        class GRK_GameLogicComponent;

        typedef int GRK_ComponentBitMask;
    }
}

#endif
