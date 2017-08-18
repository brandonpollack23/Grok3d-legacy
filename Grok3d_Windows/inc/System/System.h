#ifndef __SYSTEM__H
#define __SYSTEM__H

#include "grok3d_types.h"

namespace Grok3d { namespace Systems
{
    //this is one of the few cases where I am using dynamic dispatch
    //this is so that i can easily call the update function on them
    class GRK_System
    {
        virtual void Update(float dt) = 0;
        //inside update functions, just access wahtever system's offset like you would no problem it's static so it'll work
    };
} /*Systems*/ } /*Grok3d*/

#endif