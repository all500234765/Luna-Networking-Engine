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
        template<typename T>
        size_t WriteBuffer(T* in, size_t size) {
            int dataSize = size;
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
                DebugMessage("This RW state not supported yet by " __FUNCTION__);
            }

            // 
            return writtenSize;
        }

        size_t WriteBuffer(PacketID id, char* data, size_t size);
        char* ReadBuffer(size_t offset, size_t size);

        // 
        void Clamp();

        // Returns written size
        // TODO: Get size
        template<typename T>
        size_t operator << (T* in) {
            int dataSize = 0;
            int writtenSize = 0;

            // 
            if( mState == RWState::Relative ) {
                // Copy only that we can write
                int Size = std::min(MAX_BUFFER_SIZE - 1, dataSize + mOffset);

                // Copy data
                memcpy(&mBuffer[mOffset], in, Size);
                mOffset += Size;
                writtenSize = Size;
            } else if( mState == RWState::Begin ) {
                int Size = std::min(MAX_BUFFER_SIZE - 1, dataSize);

                // Copy data
                memcpy(&mBuffer[0], in, Size);
                mOffset += Size;
                writtenSize = Size;
            } else if( mState == RWState::End ) {
                int Size = std::min(MAX_BUFFER_SIZE - 1, dataSize);

                // Copy data
                memcpy(&mBuffer[(MAX_BUFFER_SIZE - 1) - Size], in, Size);
                writtenSize = Size;
            } else {
                DebugMessage("RW not yet supported by " __FUNCTION__);
            }

            // 
            return writtenSize;
        }

        template<typename T>
        size_t operator >> (T* out);

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
