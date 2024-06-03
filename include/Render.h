#pragma once
#include "State.h"

#define VERTEX_BUFFER_SIZE 1e5

class Render {
    private:
        State& m_state;
        u32 VBO;
        u32 VAO;
        u32 EBO;
        i32 nVertices;
        u32 shaderProgram;
        
    public:
        Render(State& state);   
        void createProgram(const std::string& vertexFile, const std::string& fragmentFile);
        void setIndices(const u32* indices, const u32 n);
        void setVertices(const f32* vertices, const i32 n);
        void setUniform(const std::string& name, const glm::mat4& value);
        void setUniform(const std::string& name, float value);
        void setCamera(const glm::vec3& pos, const glm::vec3& rot, f32 fov);
        void render();
};

u32 compileShader(const std::string& glsl_code, u32 shader_type);
u32 linkShaderProgram(u32 shader_id1, u32 shader_id2);