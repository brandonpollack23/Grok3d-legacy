#ifndef __COMPONENT__H
#define __COMPONENT__H

#include "../grok3d_types.h"

namespace Grok3d { namespace Components
{
    class GRK_Component
    {
    public:
        typedef unsigned int ComponentTypeAccessIndex;

        template<class ComponentType>
        static ComponentTypeAccessIndex GetComponentTypeAccessIndex();
    private:
        static ComponentTypeAccessIndex _offsetCounter;
    };
} /*Components*/ } /*Grok3d*/

#endif
