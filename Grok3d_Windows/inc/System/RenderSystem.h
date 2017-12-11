/* Copyright (c) 2017 Brandon Pollack
 * * Contact @ grok3d@gmail.com
 * * This file is available under the MIT license included in the project
 * */

/** @file*/

#ifndef __RENDERSYSTEM__H
#define __RENDERSYSTEM__H

#include "GLFW/glfw3.h"

#include "grok3d_types.h"
#include "Component/RenderComponent.h"

namespace Grok3d::Systems
{
    /**
     * @brief Opengl rendering system which initializes the window and draws
     *
     * @details this system uses glad and glfw to create an opengl window and draw all the
     * GRK_RenderComponents
     * In the future this will be able to be constructed with an enum class which selects rendering
     * backend, OGL, DX, etc*/
    class GRK_RenderSystem
    {
        public:
            GRK_RenderSystem() noexcept;

            auto Render() const -> Grok3d::GRK_Result;

            auto Initialize(GRK_EntityComponentManager* ecm) -> GRK_Result;

        private:
            bool m_isInitialized;
            const std::vector<Grok3d::Components::GRK_RenderComponent>* m_renderComponents;
            GLFWwindow* m_window;
    };
}

#endif
