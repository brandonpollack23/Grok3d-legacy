#include "World.h"

#include "Entity/EntityHandle.h"
#include "Entity/EntityManager.h"

#include "Component/ComponentManager.h"

#include "Assertions.h"

using namespace Grok3d;
using namespace Grok3d::Assertions;
using namespace Grok3d::Components;
using namespace Grok3d::Entities;

GRK_World::GRK_World()
{
}

GRK_EntityHandle GRK_World::CreateEntity()
{
    GRK_EntityHandle ret(this, m_entityManager.CreateEntity());
    return ret;
}

template<class ComponentType>
GRK_Result GRK_World::AddComponent(GRK_Entity entity, ComponentType& newComponent)
{
    static_assert(hasComponentTypeAccessIndex(ComponentType), "GRK_World::AddComponent Method type param not base of GRK_Component");

    if (entity == 0)
    {
        return GRK_Result::EntityAlreadyDeleted;
    }

    GRK_Result result = GetComponentManager<ComponentType>()->AddComponent(entity, newComponent);

    if (result == GRK_Result::Ok)
    {
        //add it to bitmask
        m_entityManager.m_entityComponentsMap[entity] |= IndexToMask(ComponentType::GetComponentTypeAccessIndex());
    }

    return result;
}

template<class ComponentType>
GRK_Result GRK_World::RemoveComponent(GRK_Entity entity)
{
    static_assert(hasComponentTypeAccessIndex(ComponentType), "GRK_World::RemoveComponent Method type param not base of GRK_Component");

    if (entity == 0)
    {
        return GRK_Result::EntityAlreadyDeleted;
    }

     GRK_Result result = GetComponentManager<ComponentType>()->RemoveComponent(entity, newComponent);

     if (result == GRK_Result::Ok)
     {
        //remove it from bitmask
        m_entityManager.m_entityComponentsMap[entity] &= ~(IndexToMask(ComponentType::GetComponentTypeAccessIndex()))
     }

     return result;
}

template<class ComponentType>
ComponentType* GRK_World::GetComponent(Grok3d::Entities::GRK_Entity entity)
{
    static_assert(hasComponentTypeAccessIndex(ComponentType), "GRK_World::GetComponent Method type param not base of GRK_Component");

    if (
        entity == 0 ||
        (m_entityManager.m_entityComponentsMap[entity] & IndexToMask(ComponentType::GetComponentTypeAccessIndex()) == 0))
    {
        return nullptr;
    }

    return GetComponentManager<ComponentType>()->GetComponent(entity);
}

template<class ComponentType>
GRK_ComponentManager<ComponentType>* GRK_World::GetComponentManager()
{
    static_assert(hasComponentTypeAccessIndex(ComponentType), "GRK_World::GetComponentManager Method type param not base of GRK_Component");

    //lazy instantiation of component managers, if array - 1 is smaller than the index, we havent created this manager yet for 
    //the class
    //class index is made when the component is made, which it must have if we called this
    GRK_ComponentManager<ComponentType>* manager;

    if (m_componentManagers.size() - 1 > ComponentType::GetComponentTypeAccessIndex())
    {
        manager = new GetComponentManager<ComponentType>(this);
        m_componentManagers[ComponentType::GetComponentTypeAccessIndex()] = manager;
    }
    else
    {
        manager = m_componentManagers[ComponentType::GetComponentTypeAccessIndex()];
    }

    return manager;
}

GRK_ComponentBitMask GRK_World::GetEntityComponentsBitMask(GRK_Entity entity)
{
    if (entity != 0)
    {
        return m_entityManager.m_entityComponentsMap[entity];
    }
    else 
    {
        return 0;
    }
}

GRK_Result GRK_World::DeleteEntity(GRK_Entity entity)
{
    GRK_Result result = GRK_Result::Ok;

    //ComponentManager's deleting their components is handled by GarbageCollection

    //TODO 7 iterate through all systems delete entity

    m_entityManager.m_entityComponentsMap.erase(entity);

    return result;
}

std::vector<GRK_Entity>& GRK_World::GetDeletedUncleanedEntities()
{
    return m_entityManager.deletedUncleatedEntities;
}