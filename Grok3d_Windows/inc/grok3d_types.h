/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file
 * @brief The mother file of type definitions
 * @details
 * this file contains all of the typedefs, forward type declarations, enums, general helper
 * functions (having to do with types) and operators for defined types
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

/** The namespace for the project*/
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
    inline auto operator | (GRK_Result lhs, GRK_Result rhs) -> GRK_Result

    {
        return (GRK_Result)(static_cast<T>(lhs) | static_cast<T>(rhs));
    }
    inline auto operator |= (GRK_Result& lhs, GRK_Result rhs) -> GRK_Result&
    {
        lhs = (GRK_Result)(static_cast<T>(lhs) | static_cast<T>(rhs));
        return lhs;
    }

    /** The namespace which contains all Entity related code*/
    namespace Entities
    {
        /**
         * @brief A unique ID that identifies an object in the game world
         */
        typedef std::size_t GRK_Entity;
    }

    /** The namespace which contains all Compoenents related code and Compoenents definitions*/
    namespace Components
    {
        class GRK_Component;
        class GRK_TransformComponent;
        class GRK_GameLogicComponent;
        class GRK_GameBehaviourBase;

        typedef int GRK_ComponentBitMask;
    }

    /** The namespace which contains all System related code and System definitions*/
    namespace Systems
    {
        class GRK_SystemManager;
        class GRK_System;
        class GRK_GameLogicSystem;
    }

    /**
     * @brief The specialized version of GRK_EntityComponentManager__ that is used throughout the
     * engine, it contains all of the types of components supported in a paramater pack
     * This is where you add your components to the engine, by adding to this template argument list
     *
     * @see GRK_EntityComponentManager__
     */
    template<class... ComponentTypes> class GRK_EntityComponentManager__;
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

/**std namespace, added a hash function*/
namespace std
{
    /*GRK_EntityHandle*/
    /*hash function for GRK_EntityHandle, just uses the entity hash*/
    template<class ECM> 
    struct hash<Grok3d::Entities::GRK_EntityHandle__<ECM>>
    {
        typedef Grok3d::Entities::GRK_EntityHandle__<ECM> argument_type;
        typedef size_t result_type;
        auto operator()(argument_type const& e) const -> result_type;
    };
}

#endif
