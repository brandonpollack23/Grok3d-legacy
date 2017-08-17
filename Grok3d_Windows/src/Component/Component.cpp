/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "Component/Component.h"

//this definition needed to be in a CPP file so that it wasn't redefined all over the place (even though I have ifndef blockers...idk why this happens
//it happens at work too and you can put a _declspec_(any) or something like that in the declaration but this is an MSVC only thing and I wanna be cross platform
Grok3d::Components::GRK_Component::ComponentTypeAccessIndex Grok3d::Components::GRK_Component::_offsetCounter;
