/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file*/

#include "utilities/ShaderManager.h"

#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace Grok3d;

namespace Grok3d::Utilities::ShaderManager
{
    auto GRK_LoadShader(const char* const shaderSource, ShaderType type) -> GRK_ShaderID
    {
        auto id = glCreateShader(static_cast<GLenum>(type));

        glShaderSource(id, 1, &shaderSource, nullptr);
        glCompileShader(id);

        //check if shader compilation is successfull, handle if not
        std::array<char, 512> infoLog;
        auto success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(id, 512, NULL, infoLog.data());
            std::cerr << "Error occurred, failed to load shader\n\t" << infoLog.data() << std::endl;

            glDeleteShader(id); // Don't leak the shader.

            std::exit(-1); //TODO maybe don't exit
        }

        return id;
    }

    auto GRK_LoadShaderFile(const char* const shaderFile, ShaderType type) -> GRK_ShaderID
    {
        //open the file
        auto fs = std::ifstream(shaderFile);

        //get file size
        fs.seekg(0, std::ios::end);
        auto size = fs.tellg();

        //TODO check for a max file size?

        //create an appropriately sized string
        std::string shaderSource;
        shaderSource.reserve(size);

        //seek to the beginning of the file
        fs.seekg(0);

        fs.read(&shaderSource[0], size);

        return GRK_LoadShader(shaderSource.c_str(), type);
    }

    auto GRK_UnloadShader(const GRK_ShaderID id) -> GRK_Result
    {
        glDeleteShader(id);

        auto error = glGetError();
        if(error == GL_INVALID_VALUE)
        {
            std::cerr << "Attempted to delete shader which was never created" << std::endl;
            return GRK_Result::OpenGLErrorOccurred;
        }
        else
        {
            return GRK_Result::Ok;
        }
    }

    auto GRK_CreateShaderProgram(std::initializer_list<GRK_ShaderID> shaders) -> GRK_ShaderProgramID
    {
        auto id = GRK_ShaderProgramID{glCreateProgram()};

        for(auto shader : shaders)
        {
            glAttachShader(id, shader);
            auto status = glGetError();
            if(status)
            {
                std::cerr << "Error attaching shaders to shader program, invalid input?" << std::endl;

                glDeleteProgram(id);

                std::exit(-1);
            }
        }

        glLinkProgram(id);
        auto status = glGetError();
        if(status)
        {
            std::cerr << "Error linking shader program, invalid input?" << std::endl;

            glDeleteProgram(id);

            std::exit(-1);
        }

        std::array<char, 512> infoLog;
        glGetProgramiv(id, GL_LINK_STATUS, reinterpret_cast<GLint*>(&status));
        if(!status)
        {
            glGetProgramInfoLog(id, 512, NULL, infoLog.data());

            std::cerr << "Error occurred failed to link program\n\t" << infoLog.data() << std::endl;

            glDeleteProgram(id);
        }

        return id;
    }

    auto GRK_UnloadShaderProgram(GRK_ShaderProgramID id) -> GRK_Result
    {
        glDeleteProgram(id);

        auto error = glGetError();
        if(error == GL_INVALID_VALUE)
        {
            std::cerr << "Attempted to delete program which was never created" << std::endl;
            return GRK_Result::OpenGLErrorOccurred;
        }
        else
        {
            return GRK_Result::Ok;
        }
    }
}
