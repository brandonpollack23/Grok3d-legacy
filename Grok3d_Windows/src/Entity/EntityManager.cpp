/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d.h"

using namespace Grok3d::Entities;

GRK_Entity GRK_EntityManager::CreateEntity()
{
    //I could do a check here to see if we overflowed to 0 but that's just inconceivable that we'd have that many (2^32) entities    
    return m_NextEntityId++;
}