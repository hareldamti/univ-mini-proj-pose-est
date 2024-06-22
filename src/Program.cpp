#include "Program.h"

Program::Program(State& state) : m_state(state), m_render(state), m_tinted(state){}

float vertices[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top left 
    0.0f,  0.0f, 0.5f, 1.0f, 1.0f, // top right
     0.5f, -0.0f, -0.5f, 0.0f, 1.0f, // bottom right
    -0.5f, -0.0f, -0.5f, 0.0f, 0.0f, // bottom left
    -0.0f,  0.0f, 0.5f, 1.0f, 0.0f // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3,    // second triangle
    4, 5, 7,   // first triangle
    5, 6, 7    // second triangle
    
};  

glm::vec4 pos(0.0f);
glm::vec3 rot(0.0f);
glm::mat4 dir;
float v = 0.05f, vr = 0.01f;

void Program::init() {
    m_render.createProgram("shaders/default.vert", "shaders/default.frag");
    m_render.setIndices(indices, 12);
    m_tinted.createProgram("shaders/default.vert", "shaders/tinted.frag");
    m_tinted.setIndices(indices, 12);
    pos.z = -.5;
    //glEnable(GL_BLEND);
    
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

void Program::update() {
    dir = glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(0,1,0));
    dir = glm::rotate(dir, rot.y, glm::vec3(1,0,0));


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
    m_render.setVertices(vertices, 8);
    m_render.viewport(0, 0, 800, 600);
    m_render.setCamera(pos, rot, 45.0f);
    m_render.render();
    
    // GLCall(glDisable(GL_DEPTH_TEST));
    // GLCall(glEnable(GL_BLEND));
    // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA));
    // m_tinted.setVertices(vertices, 8);
    // m_tinted.viewport(400, 0, 400, 600);
    // m_tinted.setCamera(pos, rot, 45.0f);
    // m_tinted.setUniform("tintColor", glm::vec4(0.f,0.f,1.f,1.f));
    // m_tinted.render();
    // m_tinted.setUniform("tintColor", glm::vec4(1.f,0.f,0.f,1.f));
    // m_tinted.render();
    // m_tinted.setUniform("tintColor", glm::vec4(0.f,1.f,0.f,1.f));
    // m_tinted.render();
    // GLCall(glEnable(GL_DEPTH_TEST));
    // GLCall(glDisable(GL_BLEND));
    // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); ///// GL_FILL); 
}
