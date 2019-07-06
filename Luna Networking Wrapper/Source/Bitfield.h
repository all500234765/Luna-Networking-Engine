#pragma once

#include <memory>

template<size_t _Count>
struct BitField {
private:
    bool Bit[_Count];

public:
    BitField() { memset(&Bit[0], 0, _Count); }
    BitField(const BitField& i) { memcpy(&Bit[0], &i.Bit[0], i.Size()); }
    BitField(const BitField<_Count>& i) { memcpy(&Bit[0], &i.Bit[0], _Ciunt); }

    // Get ptr to bit at 'index'
    bool& operator[](size_t index) { return &Bit[index]; }

    // Assign new value to bit
    bool operator=(bool& bit) {
        Bit[index] = bit;
        return bit;
    }

    // Return number of bits
    inline size_t Size() { return _Count; }
};
