#include "Buffer.h"

void LNW::Buffer::Clamp() {
    mOffset = std::min(std::max(mOffset, 0), MAX_BUFFER_SIZE - 1);
}
