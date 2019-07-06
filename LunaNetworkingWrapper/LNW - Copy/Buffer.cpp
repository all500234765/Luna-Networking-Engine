#include "Buffer.h"

void LNW::Buffer::Clamp() {
    mOffset = std::min(std::max(mOffset, 0), MAX_BUFFER_SIZE - 1);
}

// Write data to buffer
size_t LNW::Buffer::operator<<(char* in) {
    int dataSize = sizeof(in);
    int writtenSize = 0;

    // 
    if( mState == RWState::Relative ) {
        // Copy only that we can write
        int Size = std::min(MAX_BUFFER_SIZE - 1, dataSize + mOffset);

        // Copy data
        memcpy(&mBuffer[mOffset], &in[0], Size);
        mOffset += Size;
        writtenSize = Size;
    } else if( mState == RWState::Begin ) {
        int Size = std::min(MAX_BUFFER_SIZE - 1, dataSize);

        // Copy data
        memcpy(&mBuffer[0], &in[0], Size);
        mOffset += Size;
        writtenSize = Size;
    } else if( mState == RWState::End ) {
        int Size = std::min(MAX_BUFFER_SIZE - 1, dataSize);

        // Copy data
        memcpy(&mBuffer[(MAX_BUFFER_SIZE - 1) - Size], &in[0], Size);
        writtenSize = Size;
    } else {
        DebugMessage("RW not yet supported by " __FUNCTION__);
    }

    // 
    return writtenSize;
}
