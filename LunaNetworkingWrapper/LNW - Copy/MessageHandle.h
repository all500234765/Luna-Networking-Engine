#pragma once

#include <winsock.h>

#include "Buffer.h"

namespace LNW {
    namespace MessageHandle {
        typedef void(*WelcomeMessage)(SOCKET socket);
        typedef void(*ServerMessageHandle)(SOCKET socket, const LNW::Buffer* in, LNW::Buffer& out);
        typedef void(*ClientMessageHandle)(SOCKET socket, const LNW::Buffer* in, LNW::Buffer& out);
    }

}
