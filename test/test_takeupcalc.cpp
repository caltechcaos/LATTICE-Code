#include <gtest/gtest.h>

#include "TakeupUtil.h"

TEST(GetTakeup, One) { ASSERT_NEAR(lattice::GetTakeup(40.0, 40.0), 0.150697, 0.0001); }
TEST(GetTakeup, Two) { ASSERT_NEAR(lattice::GetTakeup(29.9, 29.9), 0.0, 0.0001); }
TEST(GetTakeup, Three) { ASSERT_NEAR(lattice::GetTakeup(75.0, 50.0), 0.524229, 0.0001); }
TEST(GetTakeup, Four) { ASSERT_NEAR(lattice::GetTakeup(45.0, 0.0), 0.114709, 0.0001); }
TEST(GetTakeup, Five) { ASSERT_NEAR(lattice::GetTakeup(0.0, 45.0), 0.114709, 0.0001); }
TEST(GetTakeup, Six) { ASSERT_NEAR(lattice::GetTakeup(105.0, 35.0), 0.643994, 0.0001); }

TEST(DoubleArmTakeupTest, One) {
    double takeup = 0.475;
    double angle = lattice::DoubleArmTakeupAngles(takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, angle), takeup, 0.01);
}
TEST(DoubleArmTakeupTest, Two) {
    double takeup = 0.6;
    double angle = lattice::DoubleArmTakeupAngles(takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, angle), takeup, 0.01);
}
TEST(DoubleArmTakeupTest, Three) {
    double takeup = 0.2;
    double angle = lattice::DoubleArmTakeupAngles(takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, angle), takeup, 0.01);
}
TEST(DoubleArmTakeupTest, Four) {
    double takeup = 0.150;
    double angle = lattice::DoubleArmTakeupAngles(takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, angle), takeup, 0.01);
}
TEST(DoubleArmTakeupTest, Five) {
    double takeup = 0.8;
    double angle = lattice::DoubleArmTakeupAngles(takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, angle), takeup, 0.01);
}

TEST(SingleArmTakeupTest, One) {
    double takeup = 0.475;
    double angle = lattice::SingleArmTakeupAngle(takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, 0.0), takeup, 0.01);
}
TEST(SingleArmTakeupTest, Two) {
    double takeup = 0.6;
    double angle = lattice::SingleArmTakeupAngle(takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, 0.0), takeup, 0.01);
}
TEST(SingleArmTakeupTest, Three) {
    double takeup = 0.2;
    double angle = lattice::SingleArmTakeupAngle(takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, 0.0), takeup, 0.01);
}
TEST(SingleArmTakeupTest, Four) {
    double takeup = 0.15;
    double angle = lattice::SingleArmTakeupAngle(takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, 0.0), takeup, 0.01);
}

TEST(CorrespondingAngleTest, One) {
    double takeup = 0.475;
    double otherAngle = 40.0;
    double angle = lattice::GetCorrespondingAngle(otherAngle, takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, otherAngle), takeup, 0.01);
}
TEST(CorrespondingAngleTest, Two) {
    double takeup = 0.6;
    double otherAngle = 86.0;
    double angle = lattice::GetCorrespondingAngle(otherAngle, takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, otherAngle), takeup, 0.01);
}
TEST(CorrespondingAngleTest, Three) {
    double takeup = 0.2;
    double otherAngle = 0.0;
    double angle = lattice::GetCorrespondingAngle(otherAngle, takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, otherAngle), takeup, 0.01);
}
TEST(CorrespondingAngleTest, Four) {
    double takeup = 0.475;
    double otherAngle = 0.0;
    double angle = lattice::GetCorrespondingAngle(otherAngle, takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, otherAngle), takeup, 0.01);
}
TEST(CorrespondingAngleTest, Five) {
    double takeup = 0.6;
    double otherAngle = 0.0;
    double angle = lattice::GetCorrespondingAngle(otherAngle, takeup);
    ASSERT_NEAR(lattice::GetTakeup(angle, otherAngle), takeup, 0.01);
}

TEST(CorrespondingAngleTest, Six) {
    double takeup = 0.6;
    double otherAngle = lattice::SingleArmTakeupAngle(takeup);
    double angle = lattice::GetCorrespondingAngle(otherAngle, takeup);
    ASSERT_NEAR(angle, 0.0, 0.01);
}