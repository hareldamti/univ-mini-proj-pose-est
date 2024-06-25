#pragma once
#include "State.h"

#define VERTEX_BUFFER_SIZE 1e5

#define CAMERA_FOV PI / 3

typedef struct Camera {
    glm::vec4 pos;
    glm::mat4 rot;
} Camera;

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
        i32 x, y, width, height;
        void createProgram(const std::string& vertexFile, const std::string& fragmentFile);
        void setIndices(const u32* indices, const u32 n);
        void setVertices(const f32* vertices, const i32 n);
        void setUniform(const std::string& name, const glm::mat4& value);
        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, const glm::vec4& value);
        void setCamera(const glm::vec3& pos, const glm::vec3& dir);
        void setCamera(const glm::vec3& pos, const glm::mat4& rot);
        void setTexture(u32 textureId);
        void viewport(int x, int y, int width, int height);
        void render(GLenum mode = GL_TRIANGLES, bool useElements = true);
        void renderPoints(std::vector<cv::Point3f> points);
};

u32 compileShader(const std::string& glsl_code, u32 shader_type);
u32 linkShaderProgram(u32 shader_id1, u32 shader_id2);