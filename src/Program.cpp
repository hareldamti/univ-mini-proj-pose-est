#include "Program.h"

Program::Program(State& state) : m_state(state), m_render(state){}

float vertices[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

glm::vec4 pos(0.0f);
glm::vec3 rot(0.0f);
glm::mat4 dir;
float v = 0.05f, vr = 0.01f;

void Program::init() {
    m_render.createProgram("shaders/default.vert", "shaders/default.frag");
    m_render.setIndices(indices, 6);
    pos.z = -.5;
}

void Program::update() {
    dir = glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(1,0,0));
    dir = glm::rotate(dir, rot.x, glm::vec3(0,1,0));

    if (m_state.input.keyState['W']) {pos += v * dir * glm::vec4(0,0,1,0);}
    if (m_state.input.keyState['S']) {pos -= v * dir * glm::vec4(0,0,1,0);}
    
    if (m_state.input.keyState[VK_LEFT]) {rot.x += vr;}
    if (m_state.input.keyState[VK_RIGHT]) {rot.x -= vr;}

    if (m_state.input.keyState[VK_UP]) {rot.y +=  vr;}
    if (m_state.input.keyState[VK_DOWN]) {rot.y -= vr;}

    if (m_state.input.keyState[VK_OEM_COMMA]) {rot.z +=  vr;}
    if (m_state.input.keyState[VK_OEM_PERIOD]) {rot.z -= vr;}

}

void Program::draw() {
    m_render.setVertices(vertices, 4);
    m_render.setCamera(pos, rot, 45.0f);
    m_render.render();

    // Wireframe: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE ///// GL_FILL); 
}
