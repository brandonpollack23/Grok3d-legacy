/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __ASSERTIONS__H
#define __ASSERTIONS__H

#include "templates/is_valid.h"

namespace Grok3d { namespace Assertions
{
    auto hasComponentTypeAccessIndex = Grok3d::Templates::is_valid([](auto&& x) -> decltype(x::GetComponentTypeAccessIndex()) { });
} /*Grok3d*/ } /*Assertions*/

#endif