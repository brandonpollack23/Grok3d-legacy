/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __GROK3d_TYPES__H
#define __GROK3d_TYPES__H

#include <type_traits>
#include <functional>

//configure Grok3d
#define INITIAL_ENTITY_ARRAY_SIZE 1024

//configure GLM
#define GLM_PRECISION_HIGHP_FLOAT

constexpr unsigned int IndexToMask(std::size_t index)
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
        CriticalError = 1 << 11,
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

    namespace Entities
    {
        typedef std::size_t GRK_Entity;
    }

    namespace Components
    {
        class GRK_Component;
        class GRK_TransformComponent;
        class GRK_GameLogicComponent;
        class GRK_GameBehaviourBase;

        typedef int GRK_ComponentBitMask;
    }

    namespace Systems
    {
        class GRK_SystemManager;
        class GRK_System;
        class GRK_GameLogicSystem;
    }

    template<class... ComponentTypes> class GRK_EntityComponentManager__;
    // This is where you add your components to the engine, by adding to this template argument list
    using GRK_EntityComponentManager = GRK_EntityComponentManager__<Components::GRK_TransformComponent, Components::GRK_GameLogicComponent>;

    namespace Components
    {
        template<class ComponentType, class ECM = GRK_EntityComponentManager> class GRK_ComponentHandle;
    }
    namespace Entities
    {
        template<class ECM = GRK_EntityComponentManager> class GRK_EntityHandle__;
        using GRK_EntityHandle = GRK_EntityHandle__<GRK_EntityComponentManager>;
    }
}

namespace std
{
    /*GRK_EntityHandle*/
    /*hash function for GRK_EntityHandle, just uses the entity hash*/
    template<class ECM> 
    struct hash<Grok3d::Entities::GRK_EntityHandle__<ECM>>
    {
        typedef Grok3d::Entities::GRK_EntityHandle__<ECM> argument_type;
        typedef size_t result_type;
        result_type operator()(argument_type const& e) const;
    };
}

#endif
