#include "CircularBuffer.h"

using namespace lattice;

template <typename T, size_t L>
CircularBuffer<T, L>::CircularBuffer() : mBuffer({}), mIndex(L - 1) { }

template <typename T, size_t L>
bool CircularBuffer<T, L>::Add(T obj) {
    mIndex = (mIndex + 1) % L;
    mBuffer[mIndex] = obj;
    return true;
}

template <typename T, size_t L>
T CircularBuffer<T, L>::Get(size_t ind) {
    // Accounts for `ind` using 0 to mean "most recent", unlike hidden mIndex
    return mBuffer[(mIndex - ind) % L];
}