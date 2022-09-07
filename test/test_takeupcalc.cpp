#include <gtest/gtest.h>

#include "TakeupUtil.h"

TEST(GetTakeup, One) { ASSERT_NEAR(lattice::GetTakeup(40.269020828, 40.269020828, 0.25, 0.2, 0.1542), 0.475, 0.0001); }
TEST(GetTakeup, Two) { ASSERT_NEAR(lattice::GetTakeup(28.965594854, 28.965594854, 0.25, 0.2, 0.1542), 0.3, 0.0001); }
TEST(GetTakeup, Three) { ASSERT_NEAR(lattice::GetTakeup(75.05861708, 0, 0.25, 0.2, 0.1542), 0.5, 0.0001); }
TEST(GetTakeup, Four) { ASSERT_NEAR(lattice::GetTakeup(90.07985159, 0.0, 0.25, 0.2, 0.1542), 0.6, 0.0001); }
TEST(GetTakeup, Five) { ASSERT_NEAR(lattice::GetTakeup(0.0, 90.07985159, 0.25, 0.2, 0.1542), 0.6, 0.0001); }

TEST(DoubleArmTakeupTest, One) { ASSERT_NEAR(lattice::DoubleArmTakeupAngles(0.475, 0.25, 0.2, 0.1542), 40.269020828, 0.01); }
TEST(DoubleArmTakeupTest, Two) { ASSERT_NEAR(lattice::DoubleArmTakeupAngles(0.3, 0.25, 0.2, 0.1542), 28.965594854, 0.01); }
TEST(DoubleArmTakeupTest, Three) { ASSERT_NEAR(lattice::DoubleArmTakeupAngles(0.2, 0.125, 0.1, 0.5), 42.608235618, 0.01); }
TEST(DoubleArmTakeupTest, Four) { ASSERT_NEAR(lattice::DoubleArmTakeupAngles(0.98, 0.25, 0.2, 0.1542), 73.6365358, 0.01); }
TEST(DoubleArmTakeupTest, Five) { ASSERT_NEAR(lattice::DoubleArmTakeupAngles(0.6, 0.25, 0.2, 0.1542), 48.258229732, 0.01); }

TEST(SingleArmTakeupTest, One) { ASSERT_NEAR(lattice::SingleArmTakeupAngle(0.475, 0.25, 0.2, 0.1542), 71.52575931, 0.01); }
TEST(SingleArmTakeupTest, Two) { ASSERT_NEAR(lattice::SingleArmTakeupAngle(0.3, 0.25, 0.2, 0.1542), 48.258229732, 0.01); }
TEST(SingleArmTakeupTest, Three) { ASSERT_NEAR(lattice::SingleArmTakeupAngle(0.2, 0.125, 0.1, 0.5), 69.95642791, 0.01); }
TEST(SingleArmTakeupTest, Four) { ASSERT_NEAR(lattice::SingleArmTakeupAngle(0.5, 0.25, 0.2, 0.1542), 75.05861708, 0.01); }
TEST(SingleArmTakeupTest, Five) { ASSERT_NEAR(lattice::SingleArmTakeupAngle(0.6, 0.25, 0.2, 0.1542), 90.07985159, 0.01); }

TEST(CorrespondingAngleTest, One) { ASSERT_NEAR(lattice::GetCorrespondingAngle(0.475, 0.25, 0.2, 0.1542, 29.965693), 50.496489358, 0.01); }
TEST(CorrespondingAngleTest, Two) { ASSERT_NEAR(lattice::GetCorrespondingAngle(0.3, 0.25, 0.2, 0.1542, 0.0), 48.258229732, 0.0001); }
TEST(CorrespondingAngleTest, Three) { ASSERT_NEAR(lattice::GetCorrespondingAngle(0.2, 0.125, 0.1, 0.5, 42.60800644), 42.60800644, 0.01); }
TEST(CorrespondingAngleTest, Four) { ASSERT_NEAR(lattice::GetCorrespondingAngle(0.5, 0.25, 0.2, 0.1542, 0.175021417679), 75.05747, 0.01); }
TEST(CorrespondingAngleTest, Five) { ASSERT_NEAR(lattice::GetCorrespondingAngle(0.6, 0.25, 0.2, 0.1542, 57.2958), 39.346959848, 0.01); }