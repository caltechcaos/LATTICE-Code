#pragma once

namespace lattice {
static constexpr double kArmLength = 0.25;
static constexpr double kLeaderPulleyX = 0.2;
static constexpr double kLeaderPulleyY = 0.015;
static constexpr double kArmX = 0.3542;
static constexpr double kArmY = 0.11;
static constexpr double kRPulley = 0.02;
/**
 * Calculates the angle needed to maintain a specified takeup for two arms holding the same angle.
 *
 * @param mintakeup The minimum takeup needed
 * @param Larm The arm length
 * @param dap The horizontal distance between the arm pivot and the passive pulley
 * @param dpdp The horizontal distance between the passive pulley and the drive pulley
 * @return The angle each arm needs to be to get the specified takeup (in radians)
 */
double DoubleArmTakeupAngles(double takeup, double armLength = kArmLength, double leaderPulleyX = kLeaderPulleyX, double leaderPulleyY = kLeaderPulleyY, double armX = kArmX, double armY = kArmY, double rPulley = kRPulley);

/**
 * Calculates the angle a single arm needs to be at to maintain a specified takeup.
 *
 * @param mintakeup The minimum takeup needed
 * @param Larm The arm length
 * @param dap The horizontal distance between the arm pivot and the passive pulley
 * @param dpdp The horizontal distance between the passive pulley and the drive pulley
 * @return The angle one arm needs to be to get the specified takeup (in radians)
 */
double SingleArmTakeupAngle(double takeup, double armLength = kArmLength, double leaderPulleyX = kLeaderPulleyX, double leaderPulleyY = kLeaderPulleyY, double armX = kArmX, double armY = kArmY, double rPulley = kRPulley);

double SingleSideTakeup(double armAngle, double armLength = kArmLength, double leaderPulleyX = kLeaderPulleyX, double leaderPulleyY = kLeaderPulleyY, double armX = kArmX, double armY = kArmY, double rPulley = kRPulley);

/**
 * Calculates the takeup resulting from two arm angles
 *
 * @param leftArmAngle The left arm angle in degrees
 * @param rightArmAngle The right arm angle in degrees
 * @param Larm The arm length
 * @param dap The horizontal distance between the arm pivot and the passive pulley
 * @param dpdp The horizontal distance between the passive pulley and the drive pulley
 * @return The takeup resulting from the angles
 */
double GetTakeup(double leftArmAngle, double rightArmAngle, double armLength = kArmLength, double leaderPulleyX = kLeaderPulleyX, double leaderPulleyY = kLeaderPulleyY, double armX = kArmX, double armY = kArmY, double rPulley = kRPulley);

/**
 * Calculates the corresponding angle to maintain a takeup given one arm angle
 *
 * @param Larm The arm length
 * @param dap The horizontal distance between the arm pivot and the passive pulley
 * @param dpdp The horizontal distance between the passive pulley and the drive pulley
 * @param otherAngle The angle of the other arm in degrees
 * @return The angle (in degrees) the other arm needs to be at to maintain the specified takeup
 */
double GetCorrespondingAngle(double otherAngle, double takeup, double armLength = kArmLength, double leaderPulleyX = kLeaderPulleyX, double leaderPulleyY = kLeaderPulleyY, double armX = kArmX, double armY = kArmY, double rPulley = kRPulley);
}  // namespace lattice
