/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __ENTITYMANAGER__H

#define __ENTITYMANAGER__H

#include <unordered_map>
#include <vector>

#include "../grok3d.h"

namespace Grok3d { namespace Entities 
{
    class GRK_EntityManager
    {
    friend class Grok3d::GRK_World;
    public:
        Grok3d::Entities::GRK_Entity CreateEntity();
    private:
        GRK_EntityManager();
        //values of 0 are destroyed entities
        Grok3d::Entities::GRK_Entity m_NextEntityId = 1;

        //world updates this, it is a friend class, but i thought it should be defined where it makes sense and is easy to find
        std::unordered_map<Grok3d::Entities::GRK_Entity, Grok3d::Components::GRK_ComponentBitMask> m_entityComponentsMap;

        std::vector<Grok3d::Entities::GRK_Entity> deletedUncleatedEntities;
    };
} /*Entities*/ } /*Grok3d*/ 

#endif