/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
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

        GRK_Result Destroy()
        {
            RETURN_FAILURE_IF_ENTITY_DESTROYED(
                GRK_Result::NoSuchEntity,
                return m_manager->DeleteEntity(m_entity);
            m_entity = 0;);
        }

        bool inline IsDestroyed() const
        {
            return m_entity == 0;
        }


        template<class ComponentType>
        Grok3d::GRK_Result AddComponent(ComponentType&& component)
        {
            RETURN_FAILURE_IF_ENTITY_DESTROYED(
                Grok3d::GRK_Result::Ok,
                return m_manager->template AddComponent<ComponentType>(m_entity, std::move(component)););
        }

        template<class ComponentType>
        Grok3d::GRK_Result RemoveComponent()
        {
            RETURN_FAILURE_IF_ENTITY_DESTROYED(
                GRK_Result::NoSuchEntity,
                return m_manager->template RemoveComponent<ComponentType>(m_entity););
        }

        template<class ComponentType>
        Grok3d::Components::GRK_ComponentHandle<ComponentType> GetComponent() const
        {
            return m_manager->template GetComponent<ComponentType>(m_entity);
        }

        bool HasComponents(const Grok3d::Components::GRK_ComponentBitMask componentBits) const
        {
            RETURN_FAILURE_IF_ENTITY_DESTROYED(
                false,
                Components::GRK_ComponentBitMask components = m_manager->GetEntityComponentsBitMask(m_entity);
            return ((components & componentBits) == componentBits));
        }

        bool operator==(const Grok3d::Entities::GRK_EntityHandle& rhs) const
        {
            return this->m_entity == rhs.m_entity;
        }

        template<class EntityComponentManager> friend bool operator==(const int entity, const Grok3d::Entities::GRK_EntityHandle__<EntityComponentManager>& handle);
        template<class EntityComponentManager> friend bool operator==(const Grok3d::Entities::GRK_EntityHandle__<EntityComponentManager>& handle, const int entity);

    private:
        friend ::std::hash<GRK_EntityHandle__<ECM>>;

        Grok3d::Entities::GRK_Entity m_entity;
        ECM* const m_manager;
    };
} /*Entities*/ } /*Grok3d*/

template<class ECM>
typename std::hash<Grok3d::Entities::GRK_EntityHandle__<ECM>>::result_type
    std::hash<Grok3d::Entities::GRK_EntityHandle__<ECM>>::operator()(typename std::hash<Grok3d::Entities::GRK_EntityHandle__<ECM>>::argument_type const& e) const
{
    return hash<size_t>{}(e.m_entity);
}

template<class ECM>
bool operator==(const int entity, const Grok3d::Entities::GRK_EntityHandle__<ECM>& handle)
{
    return entity == handle.m_entity;
}

template<class ECM>
bool operator==(const Grok3d::Entities::GRK_EntityHandle__<ECM>& handle, const int entity)
{
    return entity == handle.m_entity;
}

#endif
