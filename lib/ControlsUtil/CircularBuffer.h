#pragma once

#include <array>
#include <cstddef>

namespace lattice {
/**
 * Stores a fixed-length buffer which can be added to, removing
 * the oldest value from it
 * @tparam T The type of object to store in the buffer
 * @tparam L The length of the buffer
 */
template <typename T, size_t L>
class CircularBuffer {
   public:
    /**
     * Constructs an empty circular buffer
     */
    CircularBuffer() : mBuffer({}), mIndex(L - 1) {}

    /**
     * Adds an object to the start of the buffer, removing the oldest one
     * from the end
     * @param obj The object to add
     * @returns True on success, false otherwise
     */
    bool Add(T obj) {
        mIndex = (mIndex + 1) % L;
        mBuffer[mIndex] = obj;
        return true;
    }

    /**
     * Gets the object at the given index
     * @param ind The index, where 0 is the most recent object and
     *      L - 1 is the oldest
     * @return The object gotten
     */
    T& Get(size_t ind) {
        // Accounts for `ind` using 0 to mean "most recent", unlike hidden mIndex
        return mBuffer[(mIndex + L - ind) % L];
    }

    /**
     * Accessor Operator Overload
     * @param ind The index, where 0 is the most recent object and
     *      L - 1 is the oldest
     * @return The object gotten
     */
    T& operator[](size_t ind) {
        return Get(ind);
    }

   private:
    std::array<T, L> mBuffer;
    size_t mIndex;
};
}  // namespace lattice