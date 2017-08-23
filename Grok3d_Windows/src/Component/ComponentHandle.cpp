/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "Component/ComponentHandle.h"
#include "Component/TransformComponent.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

template<class ComponentType>
GRK_Entity GRK_ComponentHandle<ComponentType>::GetOwningEntity()
{
    return m_owner;
}

template<class ComponentType>
GRK_Result GRK_ComponentHandle<ComponentType>::destroy()
{
    return m_manager->RemoveComponent(m_entity);
}

//template specialization for GRK_TransformComponent, every entity MUST have this it cannot be destroyed
template<>
GRK_Result GRK_ComponentHandle<GRK_TransformComponent>::destroy()
{
    //TODO 20 log that this cant happen
    return GRK_Result::Ok;
}