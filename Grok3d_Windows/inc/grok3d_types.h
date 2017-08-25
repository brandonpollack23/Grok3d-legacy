/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __GROK3d_TYPES__H
#define __GROK3d_TYPES__H

#include <type_traits>

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
    class GRK_Engine;

    enum class GRK_Result : unsigned int
    {
        Ok = 0,
        NoSuchElement = 1,
        NoSpaceRemaining = 1 << 1,
        NoSuchEntity = 1 << 2,
        EntityAlreadyDeleted = 1 << 3,
        EntityAlreadyHasComponent = 1 << 4,
        NoSuchComponentManager = 1 << 5,
        ComponentAlreadyAdded = 1 << 6,
        NoSuchComponentIdentity = 1 << 7,
        MustUseComponentHandleToDeleteBehaviour = 1 << 8,
        ErrorAddingToSystem = 1 << 9,
        EngineFailureNoInitialState = 1 << 10,
    };

    using T = std::underlying_type_t<GRK_Result>;
    inline GRK_Result operator | (GRK_Result lhs, GRK_Result rhs)

    {
        return (GRK_Result)(static_cast<T>(lhs) | static_cast<T>(rhs));
    }
    inline GRK_Result& operator |= (GRK_Result& lhs, GRK_Result rhs)
    {
        lhs = (GRK_Result)(static_cast<T>(lhs) | static_cast<T>(rhs));
        return lhs;
    }

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

    namespace Systems
    {
        class GRK_SystemManager;
        class GRK_System;
        class GRK_GameLogicSystem;
    }
}

namespace std
{
    /*GRK_EntityHandle*/
    /*hash function for GRK_EntityHandle, just uses the entity hash*/
    template<> 
    struct hash<Grok3d::Entities::GRK_EntityHandle>
    {
        typedef Grok3d::Entities::GRK_EntityHandle argument_type;
        typedef size_t result_type;
        result_type operator()(argument_type const& e) const;
    };
}


#endif
