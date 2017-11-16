/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file*/ 

#ifndef __ENTITYHANDLER__H
#define __ENTITYHANDLER__H

#include <type_traits>
#include <functional>

#include "grok3d_types.h"

#include "Component/ComponentHandle.h"

#define RETURN_FAILURE_IF_ENTITY_DESTROYED(error, statements) \
    do {\
        if (IsDestroyed()) \
        { \
            return error; \
        } \
        else \
        { \
            statements; \
        } \
    } while(0)

namespace Grok3d { namespace Entities 
{
    /**
     * @brief the handle to an entity for convenient interaction with @link
     * Grok3d::GRK_EntityComponentManager__ GRK_EntityComponentManager__ @endlink
     *
     * @tparam ECM The type of the GRK_EntityComponentManager__ who created the handle, this can
     * change depending on what components are in the template arguments list for it
     *
     * @details
     * This essentially is a bundle of the @link Grok3d::Entities::GRK_Entity 
     * GRK_Entity @endlink and pointer to the owning GRK_EntityComponentManager__.
     * all calls are forwarded to the owning GRK_EntityComponentManager__*/
    template<class ECM>
    class GRK_EntityHandle__
    {
    public:
        GRK_EntityHandle__(ECM* entityComponentManager, GRK_Entity entity) :
            m_entity(entity),
            m_manager(entityComponentManager)
        {
        }

        operator GRK_Entity() const
        {
            return m_entity;
        }

        /**Destroys the entity and all it's attached components, setting the internal
         * @link Grok3d::Entities::GRK_Entity GRK_Entity @endlink to 0*/ 
        auto Destroy() -> GRK_Result
        {
            RETURN_FAILURE_IF_ENTITY_DESTROYED(
                GRK_Result::NoSuchEntity,
                return m_manager->DeleteEntity(m_entity);
                m_entity = 0;);
        }

        /**Checks if the entity is destroyed (it has an ID of 0 if it is destroyed)*/
        auto inline IsDestroyed() const -> bool
        {
            return m_entity == 0;
        }

        /**Adds component to the entity by moving it into the 
         * @link Grok3d::GRK_EntityComponentManager__ GRK_EntityComponentManager__ @endlink
         * 
         * @tparam ComponentTypes the type of component to add, should be deduced by the compiler*/
        template<class ComponentType>
        auto AddComponent(ComponentType&& component) -> Grok3d::GRK_Result
        {
            RETURN_FAILURE_IF_ENTITY_DESTROYED(
                Grok3d::GRK_Result::Ok,
                return m_manager->template AddComponent<ComponentType>(m_entity, std::move(component)););
        }

        /**Adds component to the entity by specifying the type,
         * @link Grok3d::GRK_EntityComponentManager__ GRK_EntityComponentManager__ @endlink
         *
         * @tparam ComponentType The type of component to remove, must be specified
         *
         * @returns returns @link Grok3d::GRK_Result::NoSuchEntity NoSuchEntity @endlink
         * if entity does not exist anymore or @link Grok3d::GRK_Result::NoSuchElement NoSuchElement
         * @endlink if it doesn't have the necessary component*/
        template<class ComponentType>
        auto RemoveComponent() -> Grok3d::GRK_Result
        {
            RETURN_FAILURE_IF_ENTITY_DESTROYED(
                GRK_Result::NoSuchEntity,
                return m_manager->template RemoveComponent<ComponentType>(m_entity););
        }

        /**Gets the specified componenet by type
         *
         * @tparam ComponentType The type of componet you are trying to get, must be specified
         *
         * @returns returns @link Grok3d::GRK_Result::NoSuchEntity NoSuchEntity @endlink
         * if entity does not exist anymore or @link Grok3d::GRK_Result::NoSuchElement NoSuchElement
         * @endlink if it doesn't have the necessary component*/
        template<class ComponentType>
        auto GetComponent() const -> Grok3d::Components::GRK_ComponentHandle<ComponentType>
        {
            return m_manager->template GetComponent<ComponentType>(m_entity);
        }

        /**Check if this entity has the specified components
         *
         * @param componentBits a bitmask consisting of the components constructed by OR'ing
         * Grok3d::IndexToMask on @link Grok3d::GRK_EntityComponentManager__::GetComponentTypeAccessIndex
         * GetComponentTypeAccessIndex @endlink of the necessary component types*/
        auto HasComponents(const Grok3d::Components::GRK_ComponentBitMask componentBits) const -> bool
        {
            RETURN_FAILURE_IF_ENTITY_DESTROYED(
                false,
                Components::GRK_ComponentBitMask components = m_manager->GetEntityComponentsBitMask(m_entity);
                return ((components & componentBits) == componentBits));
        }

        auto operator==(const Grok3d::Entities::GRK_EntityHandle& rhs) const -> bool
        {
            return this->m_entity == rhs.m_entity;
        }

        template<class EntityComponentManager> friend auto operator==(const int entity, const Grok3d::Entities::GRK_EntityHandle__<EntityComponentManager>& handle) -> bool;
        template<class EntityComponentManager> friend auto operator==(const Grok3d::Entities::GRK_EntityHandle__<EntityComponentManager>& handle, const int entity) -> bool;

    private:
        friend ::std::hash<GRK_EntityHandle__<ECM>>;

        Grok3d::Entities::GRK_Entity m_entity; ///< The entiy ID this is the handle for
        ECM* const m_manager;                  ///< the manager who created this handle
                                               ///< (@link Grok3d::GRK_EntityComponentManager__ GRK_EntityComponentManager__ @endlink)
    };
} /*Entities*/ } /*Grok3d*/

/**A hash algorithm for @link Grok3d::Entities::GRK_EntityHandle__ GRK_EntityHandle__ @endlink
 *
 * @details
 * This just forwards the hash to size_t's implementation on the internal entity ID*/
template<class ECM>
typename std::hash<Grok3d::Entities::GRK_EntityHandle__<ECM>>::result_type
std::hash<Grok3d::Entities::GRK_EntityHandle__<ECM>>::operator()(typename std::hash<Grok3d::Entities::GRK_EntityHandle__<ECM>>::argument_type const& e) const
{
    return hash<size_t>{}(e.m_entity);
}

template<class ECM>
auto operator==(const int entity, const Grok3d::Entities::GRK_EntityHandle__<ECM>& handle) -> bool
{
    return entity == handle.m_entity;
}

template<class ECM>
auto operator==(const Grok3d::Entities::GRK_EntityHandle__<ECM>& handle, const int entity) -> bool
{
    return entity == handle.m_entity;
}

#endif
