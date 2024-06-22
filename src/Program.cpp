#include "Program.h"
#include "Picking.h"



Program::Program(State& state) :
    m_state(state),
    m_terrain_renderer(state),
    m_lines_renderer(state),
    m_points_renderer(state),
    m_terrain(10, 0){}



float v = 0.05f, vr = 0.01f;
std::vector<f32> pickingPoints;
std::vector<u32> pickingIndices;

void Program::init() {
    m_terrain.loadTexture("IMG-0458.png", 500);
    m_terrain_renderer.createProgram("shaders/default.vert", "shaders/texture.frag");
    m_terrain_renderer.setIndices(m_terrain.indices, m_terrain.indices_vec.size());
    m_terrain_renderer.setVertices(m_terrain.vertices, m_terrain.vertices_vec.size() / 5);
    m_lines_renderer.createProgram("shaders/default.vert", "shaders/default.frag");
    m_points_renderer.createProgram("shaders/default.vert", "shaders/default.frag");
    
    cameraPos = glm::vec4(0.0f);
    cameraRot = glm::rotate(glm::mat4(1.0), glm::pi<float>(), glm::vec3(1,0,0));
    cameraPos.z = -10;
}


void Program::update() {
    if (m_state.input.keyState['S']) {cameraPos += v * cameraRot * glm::vec4(0,0,1,0);}
    if (m_state.input.keyState['W']) {cameraPos -= v * cameraRot * glm::vec4(0,0,1,0);}
    
    if (m_state.input.keyState['D']) {cameraPos += v * cameraRot * glm::vec4(1,0,0,0);}
    if (m_state.input.keyState['A']) {cameraPos -= v * cameraRot * glm::vec4(1,0,0,0);}

    if (m_state.input.keyState['E']) {cameraPos += v * cameraRot * glm::vec4(0,1,0,0);}
    if (m_state.input.keyState['C']) {cameraPos -= v * cameraRot * glm::vec4(0,1,0,0);}

    if (m_state.input.keyState[VK_LEFT]) { cameraRot = cameraRot * glm::rotate(glm::mat4(1.0f), vr, glm::vec3(0,1,0)); }
    if (m_state.input.keyState[VK_RIGHT]) { cameraRot = cameraRot * glm::rotate(glm::mat4(1.0f), -vr, glm::vec3(0,1,0)); }

    if (m_state.input.keyState[VK_UP]) { cameraRot = cameraRot * glm::rotate(glm::mat4(1.0f), vr, glm::vec3(1,0,0)); }
    if (m_state.input.keyState[VK_DOWN]) { cameraRot = cameraRot * glm::rotate(glm::mat4(1.0f), -vr, glm::vec3(1,0,0)); }

    if (m_state.input.keyState[VK_OEM_COMMA]) { cameraRot = cameraRot * glm::rotate(glm::mat4(1.0f), vr, glm::vec3(0,0,1)); }
    if (m_state.input.keyState[VK_OEM_PERIOD]) { cameraRot = cameraRot * glm::rotate(glm::mat4(1.0f), -vr, glm::vec3(0,0,1)); }

    //picking
    if (m_state.isMousePressed())
    {   
        Intersection intersection = Picking::cast(cameraPos, cameraRot, m_terrain_renderer, m_state, m_terrain);
        if (intersection.hit) {
            pickingPoints.push_back(intersection.point.x);
            pickingPoints.push_back(intersection.point.y);
            pickingPoints.push_back(intersection.point.z);
            pickingPoints.push_back(rand()/RAND_MAX);
            pickingPoints.push_back(rand()/RAND_MAX);
            pickingIndices.push_back(pickingPoints.size());
        }
        Picking::printIntersection(intersection);
    }
}

void Program::draw() {
    m_terrain_renderer.viewport(0, 0, m_state.window.width, m_state.window.height);
    m_terrain_renderer.setCamera(cameraPos, cameraRot);
    m_terrain_renderer.setTexture(m_terrain.texture);
    m_terrain_renderer.render(GL_LINES);

    m_points_renderer.setIndices(pickingIndices.data(), pickingIndices.size());
    m_points_renderer.setVertices(pickingPoints.data(), pickingPoints.size());
    m_points_renderer.viewport(0, 0, m_state.window.width, m_state.window.height);
    m_points_renderer.setCamera(cameraPos, cameraRot);
    m_points_renderer.setTexture(m_terrain.texture);
    glPointSize(10);
    m_points_renderer.render(GL_POINTS);
}


// GLCall(glDisable(GL_DEPTH_TEST));
// GLCall(glEnable(GL_BLEND));
// GLCall(glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA));
// m_tinted.setVertices(vertices, 8);
// m_tinted.viewport(400, 0, 400, 600);
// m_tinted.setCamera(cameraPos, rot, 45.0f);
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