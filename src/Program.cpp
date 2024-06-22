#include "Program.h"
#include "Picking.h"



Program::Program(State& state) : m_state(state), m_render(state), m_terrain(10, 1.f){}
glm::vec4 pos(0.0f);
glm::mat4 rot(glm::rotate(glm::mat4(1.0), glm::pi<float>(), glm::vec3(1,0,0)));
float v = 0.05f, vr = 0.01f;

void Program::init() {
    m_terrain.loadTexture("IMG-0458.png", 200);
    m_render.createProgram("shaders/default.vert", "shaders/default.frag");
    m_render.setIndices(m_terrain.indices, m_terrain.indices_vec.size());
    //m_tinted.createProgram("shaders/default.vert", "shaders/tinted.frag");
    //m_tinted.setIndices(indices, 12);
    pos.z = -10;
}

void Program::update() {
    if (m_state.input.keyState['S']) {pos += v * rot * glm::vec4(0,0,1,0);}
    if (m_state.input.keyState['W']) {pos -= v * rot * glm::vec4(0,0,1,0);}
    
    if (m_state.input.keyState['D']) {pos += v * rot * glm::vec4(1,0,0,0);}
    if (m_state.input.keyState['A']) {pos -= v * rot * glm::vec4(1,0,0,0);}

    if (m_state.input.keyState['E']) {pos += v * rot * glm::vec4(0,1,0,0);}
    if (m_state.input.keyState['C']) {pos -= v * rot * glm::vec4(0,1,0,0);}

    if (m_state.input.keyState[VK_LEFT]) { rot = rot * glm::rotate(glm::mat4(1.0f), vr, glm::vec3(0,1,0)); }
    if (m_state.input.keyState[VK_RIGHT]) { rot = rot * glm::rotate(glm::mat4(1.0f), -vr, glm::vec3(0,1,0)); }

    if (m_state.input.keyState[VK_UP]) { rot = rot * glm::rotate(glm::mat4(1.0f), vr, glm::vec3(1,0,0)); }
    if (m_state.input.keyState[VK_DOWN]) { rot = rot * glm::rotate(glm::mat4(1.0f), -vr, glm::vec3(1,0,0)); }

    if (m_state.input.keyState[VK_OEM_COMMA]) { rot = rot * glm::rotate(glm::mat4(1.0f), vr, glm::vec3(0,0,1)); }
    if (m_state.input.keyState[VK_OEM_PERIOD]) { rot = rot * glm::rotate(glm::mat4(1.0f), -vr, glm::vec3(0,0,1)); }

    //picking
    if (m_state.input.keyState['P'])
    {   
        Vec3 camera_pos = {pos.x, pos.y, pos.z}; // lets unite Vec3 of "GraphicUtils.h" with "glm::vec" later
        Intersection intersection = Picking::pickAboo(camera_pos, m_state.input.mouseX, m_state.input.mouseY, m_state.window.width, m_state.window.height, m_terrain);
        Picking::printIntersection(intersection);
    }

}

void Program::draw() {
    m_render.setVertices(0,0);
    m_render.viewport(0, 0, m_state.window.width, m_state.window.height);
    m_render.setCamera(pos, rot, 45.0f);
    m_render.setVertices(m_terrain.vertices, m_terrain.vertices_vec.size() / 5);
    //glBindTexture(GL_TEXTURE_2D, terrain.texture);
    m_render.render(GL_LINES);
}


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