/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "Entity/EntityHandle.h"

#include "EntityComponentManager.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

GRK_EntityHandle::GRK_EntityHandle(GRK_EntityComponentManager* entityComponentManager, GRK_Entity entity) : 
    m_manager(entityComponentManager),
    m_entity(entity)
{
}

GRK_EntityHandle::operator GRK_Entity() const
{
    return m_entity;
}

GRK_Result GRK_EntityHandle::Destroy()
{
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
    GRK_Result::NoSuchEntity,
    return m_manager->DeleteEntity(m_entity);
    m_entity = 0;);
}

bool inline GRK_EntityHandle::IsDestroyed() const
{
    return m_entity == 0;
}

bool GRK_EntityHandle::HasComponents(const GRK_ComponentBitMask componentBits) const
{
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
        false,
        GRK_ComponentBitMask components = m_manager->GetEntityComponentsBitMask(m_entity);
        return ((components & componentBits) == componentBits));
}

bool GRK_EntityHandle::operator==(const Grok3d::Entities::GRK_EntityHandle& rhs) const
{
    return this->m_entity == rhs.m_entity;
}

typename std::hash<GRK_EntityHandle>::result_type std::hash<GRK_EntityHandle>::operator()(std::hash<GRK_EntityHandle>::argument_type const& e) const
{
    return hash<size_t>{}(e.m_entity);
}

bool Grok3d::Entities::operator==(const int entity, const GRK_EntityHandle& handle)
{
    return entity == handle.m_entity;
}

bool Grok3d::Entities::operator==(const GRK_EntityHandle& handle, const int entity)
{
    return entity == handle.m_entity;
}
