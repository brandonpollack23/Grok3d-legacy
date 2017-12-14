/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file */

#include "Component/RenderComponent.h"

#include <vector>

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::Utilities::ShaderManager;

GRK_RenderComponent::GRK_RenderComponent(
        float* vertexes,
        std::size_t vertexCount,
        GRK_OpenGLPrimitive primitive,
        GRK_ShaderProgramID shaderProgram) :
    m_vertexes(vertexes),
    m_VBOOffset(0),
    m_vertexCount(vertexCount),
    m_primitive(primitive),
    m_shaderProgram(shaderProgram)
{
    //First bind Vertex Array Object, then bind and set vertex buffers, then configure vertex attributes

    //Set up OGL VertexArrayObject and bind it to context
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create OGL buffer to store vertex data
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Bind current VBO to context Array Buffer
    glBufferData(GL_ARRAY_BUFFER, m_vertexCount, m_vertexes, GL_STATIC_DRAW); // Copy it over

    //Set up vertex attributes for the 0 vertex
    // Configure 0 vertex attribute
    // size is 3
    // they are floats
    // do not normalize 
    // stride is 3 floats between each vertex
    // offset of buffer where vertex data is is 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    //we can unbind from array_buffer since the correct buffer is now stored in the VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //unbind VAO just be safe and not change it somewhere later
    glBindVertexArray(0);
}
