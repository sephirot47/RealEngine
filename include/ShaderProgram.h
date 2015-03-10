#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#define GL_GLEXT_PROTOTYPES

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "glm/glm.hpp"
#include "Texture.h"
#include "Shader.h"
#include "Debug.h"

class ShaderProgram
{
private:
    int textureCount;
    GLuint object;

public:

    ShaderProgram();
    ~ShaderProgram();

    void AttachShader(const Shader &shader) const;
    void Link() const;
    void Use() const;
    void UnUse() const;

    void AttachTexture(const Texture &texture, const std::string& name) const;
    void DetachTexture(const Texture &texture, const std::string& name) const;

    GLint GetUniformLocation(const std::string& name) const;
    void SetUniform(const std::string& name, float value) const;
    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, const glm::vec2& value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;
    void SetUniform(const std::string& name, const glm::vec4& value) const;
    void SetUniform(const std::string& name, const glm::mat4& value, GLboolean normalized = GL_FALSE) const;

    GLuint GetObject() const { return object; }
};

#endif // SHADERPROGRAM_H
