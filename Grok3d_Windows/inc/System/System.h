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

        //Update runs update internal dynlink function and then cleanup that all systems should run
        GRK_Result Update(double dt);
        virtual GRK_Result UpdateInternal(double dt) = 0;

        //inside this functions, just access wahtever system's offset like you would no problem it's static so it'll work
        Grok3d::GRK_Result UpdateSystemEntities(const Grok3d::Entities::GRK_EntityHandle& entity);

        Grok3d::GRK_Result UnregisterEntity(const Grok3d::Entities::GRK_EntityHandle& entity);

        // Seperate function that completes unregistration since we cant modify a iterable while it's being iterated through
        Grok3d::GRK_Result CompleteUnregisterEntities();

    protected:
        virtual Grok3d::Components::GRK_ComponentBitMask GetComponentsBitMask() const = 0;

    protected:
        std::unordered_set<Grok3d::Entities::GRK_EntityHandle> m_trackedEntities;
        std::vector<Grok3d::Entities::GRK_EntityHandle> m_entitiesToUnregister;
    };
} /*Systems*/ } /*Grok3d*/

#endif
