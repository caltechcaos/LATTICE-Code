#include <gtest/gtest.h>

#include "CircularBuffer.h"

TEST(CircularBufferTest, ManageItems) {
    lattice::CircularBuffer<int, 2> buffer;
    ASSERT_TRUE(buffer.Add(1));
    ASSERT_TRUE(buffer.Add(2));
    ASSERT_TRUE(buffer.Add(3));
    ASSERT_TRUE(buffer.Add(4));
    ASSERT_TRUE(buffer.Add(5));

    EXPECT_EQ(buffer.Get(0), 5);
    EXPECT_EQ(buffer.Get(1), 4);
    EXPECT_EQ(buffer[0], 5);
    EXPECT_EQ(buffer[1], 4);
}

TEST(CircularBufferTest, ManageItemsAlot) {
    lattice::CircularBuffer<int, 2> buffer;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 10; j++) {
            ASSERT_TRUE(buffer.Add(j + i * 10));
        }
        EXPECT_EQ(buffer.Get(0), i * 10 + 9);
        EXPECT_EQ(buffer.Get(1), i * 10 + 8);
        EXPECT_EQ(buffer[0], i * 10 + 9);
        EXPECT_EQ(buffer[1], i * 10 + 8);
    }
}

TEST(CircularBufferTest, StoreItems) {
    lattice::CircularBuffer<int, 5> buffer;
    for (int i = 0; i < 5; i++) {
        ASSERT_TRUE(buffer.Add(i));
    }

    for (int i = 4; i >= 0; i--) {
        EXPECT_EQ(buffer.Get(i), 4 - i);
        EXPECT_EQ(buffer.Get(i), 4 - i);
    }
}