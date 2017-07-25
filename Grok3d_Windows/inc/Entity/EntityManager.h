/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __ENTITYMANAGER__H

#define __ENTITYMANAGER__H

#include "../grok3d.h"

namespace Grok3d { namespace Entities 
{
    class GRK_EntityManager
    {
    public:
        Grok3d::Entities::GRK_Entity CreateEntity();
    private:
        //values of 0 are destroyed entities
        Grok3d::Entities::GRK_Entity m_NextEntityId = 1;
    };
} /*Entities*/ } /*Grok3d*/ 

#endif