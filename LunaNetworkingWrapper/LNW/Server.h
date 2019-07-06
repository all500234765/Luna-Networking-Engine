#pragma once

#include "Buffer.h"

namespace LNW {
    class Server {
    private:
        Buffer In, Out;

    public:
        inline const Buffer& GetInBuffer();

    };

}
