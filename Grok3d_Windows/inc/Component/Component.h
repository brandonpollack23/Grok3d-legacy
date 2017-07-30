#ifndef __COMPONENT__H
#define __COMPONENT__H

#include "../grok3d_types.h"

#include <string>

namespace Grok3d { namespace Components
{
    class GRK_Component
    {
    public:
        typedef unsigned int ComponentTypeAccessIndex;

        template<class ComponentType>
        static ComponentTypeAccessIndex GetComponentTypeAccessIndex();

        Grok3d::Entities::GRK_Entity GetContainingEntity();
        Grok3d::GRK_Result SetContainingEntity(Grok3d::Entities::GRK_Entity entity);

        std::string&  GetContainingEntityName();
    private:
        static ComponentTypeAccessIndex _offsetCounter;
        Grok3d::Entities::GRK_Entity m_entity;
        //a cache of the name of the gameobject this component is part of
        std::string name;
    };
} /*Components*/ } /*Grok3d*/

#endif
