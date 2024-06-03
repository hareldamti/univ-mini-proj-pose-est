#include "Render.h"

u32 compileShader(const std::string& glsl_code, u32 shader_type) {
    u32 shader = glCreateShader(shader_type);
    const char* src = glsl_code.c_str();
    glShaderSource(shader, 1, &src, NULL);
    int success;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char msg[512];
        glGetShaderInfoLog(shader, 512, NULL, msg);
        ERROR_EXIT("Error compiling GLenum(%x) shader. %s\n", shader_type, msg);
    }
    return shader;
}

u32 linkShaderProgram(u32 shader_id1, u32 shader_id2) {
    int success;
    char log[512];

    u32 program = glCreateProgram();
    glAttachShader(program, shader_id1);
    if (shader_id2) glAttachShader(program, shader_id2);
   
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, log);
        ERROR_EXIT("Error linking shader. %s\n", log);
    }
    glDeleteShader(shader_id1);
    if (shader_id1) glDeleteShader(shader_id2);
    return program;
}

Render::Render(State& state) : m_state(state) {
}

void Render::createProgram(const std::string& vertexFile, const std::string& fragmentFile) {
    GLCall(glGenBuffers(1, &VBO)); 
    GLCall(glGenBuffers(1, &EBO));
    GLCall(glGenVertexArrays(1, &VAO));
    std::string vertexSource = readFile(vertexFile);
    std::string fragmentSource = readFile(fragmentFile);
    u32 vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    u32 fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    shaderProgram = linkShaderProgram(vertexShader, fragmentShader);
    GLCall(glBindVertexArray(VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    glBufferData(GL_ARRAY_BUFFER, 5 * VERTEX_BUFFER_SIZE * sizeof(f32), (void*)0, GL_DYNAMIC_DRAW);
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0)); 
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(1));

    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader)); 
    setUniform("camera", glm::mat4(1.0f));
}

void Render::setIndices(const u32* indices, const u32 n) {
    nVertices = (i32)n;
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * n, indices, GL_STATIC_DRAW));
}

void Render::setVertices(const f32* vertices, const i32 n) {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO)); 
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(f32) * n * 5, vertices));
}
    
void Render::setUniform(const std::string& name, const glm::mat4& value) {
    GLCall(glUseProgram(shaderProgram));
    u32 location;
    GLSet(location, glGetUniformLocation(shaderProgram, name.c_str()));
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

void Render::setUniform(const std::string& name, float value) {
    GLCall(glUseProgram(shaderProgram));
    u32 location;
    GLSet(location, glGetUniformLocation(shaderProgram, name.c_str()));
    GLCall(glUniform1f(location, value));
}

void Render::setCamera(const glm::vec3& pos, const glm::vec3& rot, f32 fov) {
    glm::mat4 cam = glm::mat4(1.0f);
    cam = glm::rotate(cam, rot.z, glm::vec3(0.0f,0.0f,1.0f));
    cam = glm::rotate(cam, -rot.y, glm::vec3(1.0f,0.0f,0.0f));
    cam = glm::rotate(cam, -rot.x, glm::vec3(0.0f,1.0f,0.0f));
    cam = glm::translate(cam, pos);
    glm::mat4 proj = glm::perspective(
        fov,
        (float) m_state.window.width / m_state.window.height,
        0.1f, 100.0f
        ) * cam;
    
    setUniform("camera", proj);
}

void Render::render() {
    GLCall(glViewport(0, 0, 800, 600));
    GLCall(glUseProgram(shaderProgram));
    GLCall(glBindVertexArray(VAO));
    GLCall(glDrawElements(GL_TRIANGLES, nVertices, GL_UNSIGNED_INT, 0));
    GLCall(glBindVertexArray(0));
}