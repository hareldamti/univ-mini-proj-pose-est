#include "utils.h"

void _glClearError() {
    while(glGetError() != GL_NO_ERROR);
}

bool _glCatchError(const char* func_name, const char* file, int line) {
    bool r = true;
    unsigned int error;
    while((error = glGetError()) != GL_NO_ERROR) {
        LOG_ERROR("%s %s: %d - (GL %d)", func_name, file, line, error);
        r = false;
    }
    if (!r) ERROR_EXIT("");
    return r;
}
