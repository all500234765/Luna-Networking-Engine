#pragma once

// TODO: Custom size
#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 4096
#endif

#include <algorithm>
#include "Debug.h"

#undef min
#undef max

namespace LNW {
    typedef char PacketID;

    enum RWState : char {
        // 
        Begin,
        End,

        // 
        Relative, 
        RelativeReplace
    };

    // Fixed Buffer
    class Buffer {
    private:
        char*   mBuffer;
        RWState mState;
        int     mOffset; // In range [0; MAX_BUFFER_SIZE - 1];

    public:
        Buffer(): mState(RWState::Relative), mOffset(0) { mBuffer = new char[MAX_BUFFER_SIZE]; };
        ~Buffer() { delete mBuffer; };

        // 
        inline void    SetState(RWState state) { mState = state; }
        inline RWState GetState() { return mState; }

        // R/W
        void  WriteBuffer(PacketID id, char* data, size_t size);
        char* ReadBuffer(size_t offset, size_t size);

        // 
        void Clamp();

        // Returns written size
        size_t operator << (char* in);
        size_t operator >> (char* out);

        inline void Seek(RWState state, int off) {
            switch( state ) {
                case RWState::Begin: mOffset = off; break;
                case RWState::End  : mOffset = (MAX_BUFFER_SIZE - 1) - off; break;

                case RWState::Relative:
                case RWState::RelativeReplace:
                    mOffset = off;
                    break;
            }
        }

        inline void Seek(int off) { mOffset = off; }
        inline int Tell() { return mOffset; }

        inline const char* Data() { return mBuffer; }
        inline char* DataPtr() { return &mBuffer[0]; }
        inline size_t Size() { return MAX_BUFFER_SIZE; }
    };
}
