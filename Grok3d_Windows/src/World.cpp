#include "World.h"

#include "Entity/EntityHandle.h"
#include "Entity/EntityManager.h"

#include "Component/ComponentManager.h"

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::Entities;

GRK_World::GRK_World()
{
    m_componentManagers.push_back(&m_translationComponentManager);
}

GRK_EntityHandle GRK_World::CreateEntity()
{
    GRK_EntityHandle ret(this, m_entityManager.CreateEntity());
    return ret;
}

template<class ComponentType>
GRK_Result GRK_World::AddComponent(GRK_Entity entity, ComponentType& newComponent)
{
    return GetComponentManager<ComponentType>()->AddComponent(entity, newComponent);
    return GRK_OK;
}

template<class ComponentType>
GRK_Result GRK_World::RemoveComponent(GRK_Entity entity)
{
    return GetComponentManager<ComponentType>()->RemoveComponent(entity, newComponent);
    return GRK_OK;
}

template<class ComponentType>
ComponentType* GRK_World::GetComponent(Grok3d::Entities::GRK_Entity entity)
{
    return GetComponentManager<ComponentType>()->GetComponent(entity);
}

template<class ComponentType>
GRK_ComponentManager<ComponentType>* GRK_World::GetComponentManager()
{
    //TODO 0
}

GRK_Result GRK_World::DeleteEntity(GRK_Entity entity)
{
    //TODO inf each manager delete the entity and see header line 50
    m_translationComponentManager.RemoveComponent(entity);

    return GRK_OK;
}