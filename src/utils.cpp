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
