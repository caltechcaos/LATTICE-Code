#include <gtest/gtest.h>

#include "MathUtil.h"

TEST(GetTakeup, One) { ASSERT_NEAR(lattice::GetTakeup(0.702827, 0.702827, 0.25, 0.2, 0.1542), 0.475, 0.0001); }
TEST(GetTakeup, Two) { ASSERT_NEAR(lattice::GetTakeup(0.505545, 0.505545, 0.25, 0.2, 0.1542), 0.3, 0.0001); }
TEST(GetTakeup, Three) { ASSERT_NEAR(lattice::GetTakeup(1.31002, 0, 0.25, 0.2, 0.1542), 0.5, 0.0001); }
TEST(GetTakeup, Four) { ASSERT_NEAR(lattice::GetTakeup(1.57219, 0.0, 0.25, 0.2, 0.1542), 0.6, 0.0001); }
TEST(GetTakeup, Five) { ASSERT_NEAR(lattice::GetTakeup(0.0, 1.57219, 0.25, 0.2, 0.1542), 0.6, 0.0001); }

TEST(DoubleArmTakeupTest, One) { ASSERT_NEAR(lattice::DoubleArmTakeupAngles(0.475, 0.25, 0.2, 0.1542), 0.702827, 0.0001); }
TEST(DoubleArmTakeupTest, Two) { ASSERT_NEAR(lattice::DoubleArmTakeupAngles(0.3, 0.25, 0.2, 0.1542), 0.505545, 0.0001); }
TEST(DoubleArmTakeupTest, Three) { ASSERT_NEAR(lattice::DoubleArmTakeupAngles(0.2, 0.125, 0.1, 0.5), 0.743654, 0.0001); }
TEST(DoubleArmTakeupTest, Four) { ASSERT_NEAR(lattice::DoubleArmTakeupAngles(0.98, 0.25, 0.2, 0.1542), 1.2852, 0.0001); }
TEST(DoubleArmTakeupTest, Five) { ASSERT_NEAR(lattice::DoubleArmTakeupAngles(0.6, 0.25, 0.2, 0.1542), 0.842265, 0.0001); }

TEST(SingleArmTakeupTest, One) { ASSERT_NEAR(lattice::SingleArmTakeupAngle(0.475, 0.25, 0.2, 0.1542), 1.24836, 0.0001); }
TEST(SingleArmTakeupTest, Two) { ASSERT_NEAR(lattice::SingleArmTakeupAngle(0.3, 0.25, 0.2, 0.1542), 0.842265, 0.0001); }
TEST(SingleArmTakeupTest, Three) { ASSERT_NEAR(lattice::SingleArmTakeupAngle(0.2, 0.125, 0.1, 0.5), 1.22097, 0.0001); }
TEST(SingleArmTakeupTest, Four) { ASSERT_NEAR(lattice::SingleArmTakeupAngle(0.5, 0.25, 0.2, 0.1542), 1.31002, 0.0001); }
TEST(SingleArmTakeupTest, Five) { ASSERT_NEAR(lattice::SingleArmTakeupAngle(0.6, 0.25, 0.2, 0.1542), 1.57219, 0.0001); }