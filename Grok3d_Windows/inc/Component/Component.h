/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __COMPONENT__H
#define __COMPONENT__H

#include "../grok3d_types.h"

#include <string>

namespace Grok3d { namespace Components
{
    class GRK_Component
    {
    public:
        typedef unsigned int ComponentTypeAccessIndex;

        template<class ComponentType>
        static ComponentTypeAccessIndex GetComponentTypeAccessIndex();
    private:
        //TODO multithread lock th is
        static ComponentTypeAccessIndex _offsetCounter;
    };
} /*Components*/ } /*Grok3d*/

#endif
