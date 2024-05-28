#include "Program.h"
#include <glad.h>

Program::Program(State state) : m_state(state) {}

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  
unsigned int VBO;
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

unsigned int shaderProgram;

unsigned int VAO;
unsigned int EBO;

glm::mat4 camera(glm::vec3 const& position, glm::vec3 const& rotation)
{
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), position);
    View = glm::rotate(View, rotation.x, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, rotation.y, glm::vec3(0.0f, -1.0f, 0.0f));
	View = glm::rotate(View, rotation.z, glm::vec3(0.0f, 1.0f, -1.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

void Program::init() {
	GLCall(glGenBuffers(1, &VBO)); 
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));  
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GLCall(glViewport(0, 0, 800, 600));
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLCall(glShaderSource(vertexShader, 1, &vertexShaderSource, NULL));
    GLCall(glCompileShader(vertexShader));
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOG_ERROR("VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLCall(glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL));
    GLCall(glCompileShader(fragmentShader));
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOG_ERROR("FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    shaderProgram = glCreateProgram();
    GLCall(glAttachShader(shaderProgram, vertexShader));
    GLCall(glAttachShader(shaderProgram, fragmentShader));
    GLCall(glLinkProgram(shaderProgram));
    glGetShaderiv(vertexShader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        LOG_ERROR("PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    GLCall(glUseProgram(shaderProgram));
    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader)); 

    GLCall(glGenBuffers(1, &EBO));
    GLCall(glGenVertexArrays(1, &VAO));

    GLCall(glBindVertexArray(VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

    GLCall(glEnableVertexAttribArray(0)); 
    
}

void Program::update() {
    vertices[2] += 1;
    vertices[5] += 1;
}

void Program::draw() {
    GLCall(glUseProgram(shaderProgram));
    GLCall(glBindVertexArray(VAO));
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    GLCall(glBindVertexArray(0));

    // Wireframe glPolygonMode(GL_FRONT_AND_BACK, GL_LINE ///// GL_FILL); 
}
