#include "Debug.h"

void LNW::DebugError(int id, const char* fn, int line) {
    _com_error error(id);

    printf("[LNW]: Error in %s[%i]: %s", fn, line, error.ErrorMessage());
    printf("\n");
}

void LNW::DebugMessage(const char* msg) {
    printf("[LNW]: %s", msg);
    printf("\n");
}

const char* LNW::GetFileName(const char* path) {
    const char *name = NULL, *tmp = NULL;
    if( path && *path ) {
        name = strrchr(path, '/');
        tmp = strrchr(path, '\\');
        if( tmp ) {
            return name && name > tmp ? name + 1 : tmp + 1;
        }
    }
    return name ? name + 1 : path;
}
