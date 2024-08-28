#include "utils.h"

std::string readFile(const std::string& file_path) {
    std::ifstream stream(file_path);
    std::string data((std::istreambuf_iterator<char>(stream)),
                 std::istreambuf_iterator<char>());
    return data;
}

void _glClearError() {
    while(glGetError() != GL_NO_ERROR);
}

bool _glCatchError(const char* func_name, const char* file, int line) {
    std::map<int, const std::string>GL_ENUM_ERRORS = {
        {500, "GL_INVALID_ENUM"},
        {501, "GL_INVALID_VALUE"},
        {502, "GL_INVALID_OPERATION"},
        {503, "GL_STACK_OVERFLOW"},
        {504, "GL_STACK_UNDERFLOW"},
        {505, "GL_OUT_OF_MEMORY"},
        {506, "GL_INVALID_FRAMEBUFFER_OPERATION"},
        {507, "GL_CONTEXT_LOST"},
        {8031, "GL_TABLE_TOO_LARGE"},
    };
    bool r = true;
    unsigned int error;
    while((error = glGetError()) != GL_NO_ERROR) {
        LOG_ERROR("%s %s: %d - (GL %x)", func_name, file, line, error);
        r = false;
    }
    if (!r) ERROR_EXIT("");
    return r;
}

cv::Point2f Point2f(glm::vec2& point) {
    return cv::Point2f(point.x, point.y);
}
cv::Point3f Point3f(glm::vec3& point) {
    return cv::Point3f(point.x, point.y, point.z);
}

glm::mat4 Mat4(cv::Mat& mat) {
    glm::mat4 m(1.0f);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[j][i] = mat.at<double>(i, j);
    return m;
}

glm::vec4 Vec4(cv::Mat& tvec) {
    return glm::vec4(tvec.at<double>(0), tvec.at<double>(1), tvec.at<double>(2), 1);
}

glm::vec4 Vec4(cv::Point3f& point) {
    return glm::vec4(point.x, point.y, point.z, 1);
}

std::string format(glm::mat4 m) {
    return std::format( 
        "{}\t{}\t{}\t{}\n{}\t{}\t{}\t{}\n{}\t{}\t{}\t{}\n{}\t{}\t{}\t{}",
        m[0][0], m[1][0], m[2][0], m[3][0],
        m[0][1], m[1][1], m[2][1], m[3][1],
        m[0][2], m[1][2], m[2][2], m[3][2],
        m[0][3], m[1][3], m[2][3], m[3][3]); 
}

std::string format(glm::vec4 v) {
    return std::format( 
        "{}\n{}\n{}\n{}",
        v[0], v[1], v[2], v[3]); 
}

Params::Params(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) ERROR_EXIT("Config file %s not found\n", filename.c_str());

    std::string line;
    while (std::getline(file, line)) {
        std::string name;
        f32 value;
        std::istringstream stream(line);
        stream >> name >> value;
        params[name] = value;
        if (stream.fail()) ERROR_EXIT("Config file invalid\n");
    }
}

f32 Params::get(std::string name) {
    if (!params.contains(name)) ERROR_EXIT("Parameter %s not requested but not found\n", name.c_str());
    return params[name];
}