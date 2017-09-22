#ifndef __SYSTEM__H
#define __SYSTEM__H

#include "grok3d_types.h"
#include <unordered_set>

namespace Grok3d { namespace Systems
{
    //this is one of the few cases where I am using dynamic dispatch
    //this is so that i can easily call the update function on them
    class GRK_System
    {
    public:
        GRK_System();
        virtual GRK_Result Update(double dt) = 0;

        //inside this functions, just access wahtever system's offset like you would no problem it's static so it'll work
        Grok3d::GRK_Result UpdateSystemEntities(Grok3d::Entities::GRK_EntityHandle& entity);

        Grok3d::GRK_Result UnregisterEntity(Grok3d::Entities::GRK_EntityHandle& entity);

    protected:
        virtual Grok3d::Components::GRK_ComponentBitMask GetComponentsBitMask() = 0;

    protected:
        std::unordered_set<Grok3d::Entities::GRK_EntityHandle> m_trackedEntities;
    };
} /*Systems*/ } /*Grok3d*/

#endif
