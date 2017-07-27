/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d.h"
#include "Entity/EntityManager.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

GRK_EntityManager::GRK_EntityManager()
{
    //TODO 10 INITIAL_ENTITY_ARRAY_SIZE is a init CVAR just like in ComponentManager
    m_entityComponentsMap = std::unordered_map<GRK_Entity, GRK_ComponentBitMask>(INITIAL_ENTITY_ARRAY_SIZE);
    deletedUncleatedEntities = std::vector<GRK_Entity>(INITIAL_ENTITY_ARRAY_SIZE/4);
}

GRK_Entity GRK_EntityManager::CreateEntity()
{
    //I could do a check here to see if we overflowed to 0 but that's just inconceivable that we'd have that many (2^32) entities    
    GRK_Entity id = m_NextEntityId++;
    m_entityComponentsMap[id] = 0;

    return id;
}