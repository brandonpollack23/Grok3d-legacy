#include "Component/ComponentHandle.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

template<class ComponentType>
GRK_ComponentHandle<ComponentType>::GRK_ComponentHandle(GRK_ComponentManager<ComponentType>* manager, ComponentType* component, GRK_Entity owner) : 
    m_manager(manager), 
    m_component(component),
    m_owner(owner)
{
    static_assert(std::is_base_of<GRK_Component, ComponentType>::value, "The ComponentType passed to class GRK_ComponentHandle was not of base GRK_Component");
}

template<class ComponentType>
GRK_Entity GRK_ComponentHandle<ComponentType>::GetOwningEntity()
{
    return m_owner;
}

template<class ComponentType>
ComponentType* GRK_ComponentHandle<ComponentType>::operator->()
{
    return m_component;
}

template<class ComponentType>
GRK_Result GRK_ComponentHandle<ComponentType>::destroy()
{
    m_manager->RemoveComponent(m_entity);
}
