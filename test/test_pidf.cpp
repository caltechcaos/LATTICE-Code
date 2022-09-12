#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <tuple>

#include "PIDF.h"
TEST(NumericalDiff, One) {
    lattice::PIDF pidf{0, 0, 0, 0};
    EXPECT_NEAR(pidf.CalcD(), 0, 0.00001);
    pidf.Run(0.1);
    EXPECT_NEAR(pidf.CalcD(), 7.5, 0.0001);
    pidf.Run(0.2);
    EXPECT_NEAR(pidf.CalcD(), 5.0, 0.0001);
    pidf.Run(0.3);
    EXPECT_NEAR(pidf.CalcD(), 5.0, 0.0001);
    pidf.Run(0.3);
    EXPECT_NEAR(pidf.CalcD(), -2.5, 0.0001);
    pidf.Run(0.3);
    EXPECT_NEAR(pidf.CalcD(), 0, 0.0001);
}

TEST(PIDOutput, One) {
    lattice::PIDF pidf{1, 1, 1, 0.5};
    EXPECT_NEAR(pidf.Run(0.0), 0.5, 0.00001);    // 0 + 0 - 0 + 0.5
    EXPECT_NEAR(pidf.Run(0.1), -7.102, 0.0001);  // (0-0.1) + ((0-0.1)*(0.02)) - 7.5 + 0.5
    EXPECT_NEAR(pidf.Run(0.2), -4.706, 0.0001);  // (0-0.2) + ((0-0.1)*(0.02) + ((0-0.2) * 0.02)) - 5.0 + 0.5
}

TEST(PIDFuncFeedforward, One) {
    auto ffFunc = [](double setpoint) { return std::copysign(5, setpoint); };
    lattice::PIDF pidf{0, 0, 0, ffFunc};
    EXPECT_NEAR(pidf.Run(0.0, 1), 5, 0.00001);
    EXPECT_NEAR(pidf.Run(0.0, -1), -5, 0.0001);
}

TEST(ErrorBounds, One) {
    lattice::PIDF pidf{0, 0, 0, 0};
    ASSERT_TRUE(pidf.AtTarget());
    pidf.Run(0.2);
    ASSERT_TRUE(!pidf.AtTarget());
    pidf.Run(0.1);
    ASSERT_TRUE(!pidf.AtTarget());
    pidf.Run(0.1);
    ASSERT_TRUE(!pidf.AtTarget());
    pidf.Run(0.1);
    ASSERT_TRUE(pidf.AtTarget());
}
