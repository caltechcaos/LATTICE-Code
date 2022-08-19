#pragma once

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
    CircularBuffer();

    /**
     * Adds an object to the start of the buffer, removing the oldest one
     * from the end
     * @param obj The object to add
     * @returns True on success, false otherwise
     */
    bool Add(T obj);

    /**
     * Gets the object at the given index
     * @param ind The index, where 0 is the most recent object and
     *      L - 1 is the oldest
     * @return The object gotten
     */
    T Get(size_t ind);

   private:
    std::array<T, L> mBuffer;
    size_t mIndex;
};
}  // namespace lattice