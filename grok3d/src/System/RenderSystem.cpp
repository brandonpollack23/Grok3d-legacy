/* Copyright (c) 2017 Brandon Pollack
 * * Contact @ grok3d@gmail.com
 * * This file is available under the MIT license included in the project
 * */

/** @file*/

#include <iostream>
#include <cstdlib>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "grok3d_types.h"
#include "EntityComponentManager.h"
#include "System/RenderSystem.h"

using namespace Grok3d;
using namespace Grok3d::Systems;
using namespace Grok3d::Components;

GRK_RenderSystem::GRK_RenderSystem() :
    m_isInitialized(false)
{
}

auto GRK_RenderSystem::Initialize(GRK_EntityComponentManager* ecm) -> GRK_Result
{
    m_renderComponents = ecm->GetComponentStore<GRK_RenderComponent>();

    //set up error callback
    glfwSetErrorCallback(
            [](int error, const char* description)
            {
                std::cerr << "GLFW error 0x" << std::hex << error << " occured\n";
                std::cerr << description << std::endl;
                std::exit(-1);
            });

    //set up GLFW
    //TODO read a config file for resolution, fullscreen, game name, etc and 
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize glfw" << std::endl;
        std::exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //GLFW window creation
    //second to last param is NULL, this is for monitor, we need this when we do fullscreen
    m_window = glfwCreateWindow(800, 600, "Grok3d Game", NULL, NULL);
    if(m_window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        std::exit(-1); 
    }

    glfwMakeContextCurrent(m_window);

    //Initialize GLAD (OGL function loader)
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to Initialize GLAD" << std::endl;
        std::exit(-1);
    }

    //set up the viewport on the OGL context window
    //this is what is used when transforming from normalized device coordinates to actual offset
    glViewport(0, 0, 800, 600);
    //set up the callback to adjust viewport on window resize
    glfwSetFramebufferSizeCallback(m_window, 
            [](GLFWwindow*, int width, int height)
            { 
                glViewport(0, 0, width, height);
            });

    m_isInitialized = true;

    return GRK_Result::Ok;
}

auto GRK_RenderSystem::Render() const -> GRK_Result
{
    //TODO check if initialized
    //TODO move glfw poll events and process input on user input system

    if(!glfwWindowShouldClose(m_window))
    {
        ProcessInput();

        //Render Commands
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(auto& renderComponent : *m_renderComponents)
        {
            //specify shader program
            glUseProgram(renderComponent.GetShaderProgram());

            //Bind VAO (rules for how this vertex shader data is formatted)
            glBindVertexArray(renderComponent.GetVAO());

            //draw
            glDrawArrays(
                    renderComponent.GetPrimitive(),
                    renderComponent.GetVBOOffsett(),
                    renderComponent.GetVertexCount());
        }

        glfwSwapBuffers(m_window);
        glfwPollEvents();

        return GRK_Result::Ok;
    }
    else
    {
        glfwTerminate();
        return GRK_Result::RenderingTerminated;
    }
}

auto GRK_RenderSystem::ProcessInput() const -> void
{
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, true);
    }
}
