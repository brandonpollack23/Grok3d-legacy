/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/** @file
 * Definition for render componenent*/

#ifndef __RENDERCOMPONENT__H
#define __RENDERCOMPONENT__H

#include "grok3d_types.h"
#include "utilities/ShaderManager.h"

#include "glad/glad.h"
#include "glm/fwd.hpp"

#include <vector>
#include <memory>

//TODO have unitied "model store" or "file store" so that vertexes can be shared as shared_ptr
namespace Grok3d::Components
{
    enum class GRK_OpenGLPrimitive : GLenum
    {
        GL_Triangles = GL_TRIANGLES
    };

    class GRK_RenderComponent
    {
        public:
            GRK_RenderComponent(
                    float* vertexes,
                    std::size_t vertexCount,
                    GRK_OpenGLPrimitive primitive,
                    Grok3d::Utilities::ShaderManager::GRK_ShaderProgramID shaderProgram) noexcept;

            auto GetVAO()           const { return m_VAO;                            }
            auto GetVBOOffsett()    const { return m_VBOOffset;                      }
            auto GetVertexCount()   const { return m_vertexCount;                    }
            auto GetPrimitive()     const { return static_cast<GLenum>(m_primitive); }
            auto GetShaderProgram() const { return m_shaderProgram;                  }

        private:
            float* m_vertexes;

            GRK_VertexArrayObject m_VAO;

            GRK_VertexBufferObject m_VBO;
            std::size_t m_VBOOffset;
            std::size_t m_vertexCount;

            GRK_OpenGLPrimitive m_primitive;

            Grok3d::Utilities::ShaderManager::GRK_ShaderProgramID m_shaderProgram;
    };
}

#endif
