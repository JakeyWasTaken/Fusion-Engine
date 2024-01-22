#pragma once
#include "FusionCore.h"

unsigned int ActiveShaderProg;

namespace Fusion
{
	class Shader
	{
	public:
        Shader() {}

        Shader(const char* vertexShaderName, const char* fragShaderName)
        {
            // Allows us to reload shaders
            m_vertexShaderName = vertexShaderName;
            m_fragShaderName = fragShaderName;

            ShaderFactory::CreateShader(vertexShaderName, m_vertShader, ShaderType::Vertex);
            ShaderFactory::CreateShader(fragShaderName, m_fragShader, ShaderType::Fragment);
            ShaderFactory::CreateShaderProg(m_ProgId, m_vertShader, m_fragShader);
        }

        ~Shader()
        {
            glDeleteProgram(m_ProgId);
        }

        void reloadShader()
        {
            bool wasActive = ActiveShaderProg == m_ProgId;

            unsigned int tempProgId;

            ShaderFactory::CreateShader(m_vertexShaderName, m_vertShader, ShaderType::Vertex);
            ShaderFactory::CreateShader(m_fragShaderName, m_fragShader, ShaderType::Fragment);
            bool Success = ShaderFactory::CreateShaderProg(tempProgId, m_vertShader, m_fragShader);

            if (Success)
            {
                glDeleteProgram(m_ProgId);
                m_ProgId = tempProgId;
                if (wasActive)
                    use();
            }
            else
            {
                printf("Failed to reload shaders");
            }
            
        }

        void use()
        {
            ActiveShaderProg = m_ProgId;
            glUseProgram(m_ProgId);
        }

        unsigned int getProgId()
        {
            return m_ProgId;
        }

        const char* getVertPath() const { return m_vertexShaderName; }
        const char* getFragPath() const { return m_fragShaderName; }

        void setBool(const std::string& name, bool value) const
        {
            glUniform1i(glGetUniformLocation(m_ProgId, name.c_str()), (int)value);
        }

        void setInt(const std::string& name, int value) const
        {
            glUniform1i(glGetUniformLocation(m_ProgId, name.c_str()), value);
        }

        void setFloat(const std::string& name, float value) const
        {
            glUniform1f(glGetUniformLocation(m_ProgId, name.c_str()), value);
        }

        // ------------------------------------------------------------------------
        void setVec2(const std::string& name, const glm::vec2& value) const
        {
            glUniform2fv(glGetUniformLocation(m_ProgId, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string& name, float x, float y) const
        {
            glUniform2f(glGetUniformLocation(m_ProgId, name.c_str()), x, y);
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string& name, const glm::vec3& value) const
        {
            glUniform3fv(glGetUniformLocation(m_ProgId, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string& name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(m_ProgId, name.c_str()), x, y, z);
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string& name, const glm::vec4& value) const
        {
            glUniform4fv(glGetUniformLocation(m_ProgId, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string& name, float x, float y, float z, float w) const
        {
            glUniform4f(glGetUniformLocation(m_ProgId, name.c_str()), x, y, z, w);
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string& name, const glm::mat2& mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(m_ProgId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string& name, const glm::mat3& mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(m_ProgId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string& name, const glm::mat4& mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(m_ProgId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
	private:
        const char* m_vertexShaderName = nullptr;
        const char* m_fragShaderName = nullptr;
		unsigned int m_ProgId = NULL;
		unsigned int m_vertShader = NULL;
		unsigned int m_fragShader = NULL;
	};
}