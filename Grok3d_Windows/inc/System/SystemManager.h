#ifndef __SYSTEMMANAGER__H
#define __SYSTEMMANAGER__H

#include "grok3d_types.h"

#include <array>

namespace Grok3d { namespace Systems
{
    class GRK_SystemManager
    {
        //TODO
    public:
            GRK_SystemManager();
            Grok3d::GRK_Result Initialize(Grok3d::EntityComponentManager* ecManager);
            
            //thie function does a check to see if all the bits in it's bit mask 
            //are in it's bit mask
            Grok3d::GRK_Result UpdateSystemEntities(
                    Grok3d::GRK_Entity entity,
                    Grok3d::Component::GRK_ComponentBitMask newBitMask);

        Grok3d::GRK_Entity UpdateSystems(float dt);

    private:
            std::array<GRK_System, 1> m_systems;
    };
} /*Systems*/ } /*Grok3d*/

#endif
