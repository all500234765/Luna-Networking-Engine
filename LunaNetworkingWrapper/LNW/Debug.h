#pragma once

#include <comdef.h>

namespace LNW {
    const char* GetFileName(const char *path);

#ifndef __FILENAME__
#define __FILENAME__ LNW::GetFileName(__FILE__)
#endif

    void DebugError(int id, const char* fn=__FILENAME__, int line=__LINE__);

    void DebugMessage(const char* msg);
}


