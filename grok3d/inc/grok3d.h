/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file convenience header that includes the entire engine*/

#ifndef __GROK3D__H
#define __GROK3D__H

#include "EntityComponentManager.h"

#include "Entity/EntityHandle.h"

#include "Component/ComponentHandle.h"
#include "Component/TransformComponent.h"
#include "Component/GameLogicComponent.h"
#include "Component/RenderComponent.h"

#include "System/System.h"
#include "System/SystemManager.h"
#include "System/GameLogicSystem.h"
#include "System/RenderSystem.h"

#include "utilities/ShaderManager.h"

#include "Engine.h"

#endif

