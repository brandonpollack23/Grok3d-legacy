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

        virtual void Update(float dt) = 0;

        //inside this functions, just access wahtever system's offset like you would no problem it's static so it'll work
        Grok3d::GRK_Result UpdateSystemEntities(Grok3d::GRK_Entity entity, Grok3d::Component::GRK_ComponentBitMask newBitMask);

    private:
        virtual Grok3d::Component::GRK_ComponentBitMask GetComponentsBitMask() = 0;

    private:
        std::unordered_set<Grok3d::Entities::GRK_Entity> m_trackedEntities;
    };
} /*Systems*/ } /*Grok3d*/

#endif
