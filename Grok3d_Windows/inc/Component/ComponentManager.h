/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __COMPONENTMANAGER__H
#define __COMPONENTMANAGER__H

#include <unordered_map>
#include <vector>
#include <functional>

#include "../grok3d_types.h"

namespace Grok3d { namespace Components 
{
    class GRK_ComponentManagerBase
    {
    };

    template<class ComponentType>
    class GRK_ComponentManager : public GRK_ComponentManagerBase
    {
    public:
        GRK_ComponentManager();
        //implicit destructor is fine

        Grok3d::GRK_Result AddComponent(Grok3d::Entities::GRK_Entity entity, ComponentType& component);

        ComponentType* GetComponent(Grok3d::Entities::GRK_Entity entity);

        Grok3d::GRK_Result RemoveComponent(Grok3d::Entities::GRK_Entity entity);

        Grok3d::GRK_Result ForEach(std::function<GRK_Result(ComponentType component)> lambda);
    private:
        typedef int ComponentInstance;

        std::unordered_map<Grok3d::Entities::GRK_Entity, ComponentInstance>  m_entityMap;
        std::vector<ComponentType> m_components;
    };
} /*Components*/ } /*Grok3d*/

#endif
