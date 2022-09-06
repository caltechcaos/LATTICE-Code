#include <gtest/gtest.h>

#include "FeedforwardUtil.h"

TEST(SimpleFeedForward, One) {
    EXPECT_NEAR(lattice::GetSimpleFeedforward(0.5, 0.3, 0.2, 0.01, 0.5), 0.603, 0.0001);
}

TEST(SimpleFeedForward, Two) {
    EXPECT_NEAR(lattice::GetSimpleFeedforward(0.5, 0.3, 0.2, -0.1, 0.5), -0.43, 0.0001);
}

TEST(SimpleFeedForward, Three) {
    EXPECT_NEAR(lattice::GetSimpleFeedforward(0.5, 0.3, 0.2, -0.1, -0.5), -0.63, 0.0001);
}

TEST(ElevatorFeedForward, One) {
    EXPECT_NEAR(lattice::GetElevatorFeedforward(0.5, 0.3, 0.2, 0.25, 0.01, 0.5), 0.853, 0.0001);
}

TEST(ElevatorFeedForward, Two) {
    EXPECT_NEAR(lattice::GetElevatorFeedforward(0.5, 0.3, 0.2, 0.25, -0.1, 0.5), -0.18, 0.0001);
}

TEST(ElevatorFeedForward, Three) {
    EXPECT_NEAR(lattice::GetElevatorFeedforward(0.5, 0.3, 0.2, 0.25, -0.1, -0.5), -0.38, 0.0001);
}
