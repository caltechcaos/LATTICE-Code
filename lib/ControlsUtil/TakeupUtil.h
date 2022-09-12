#pragma once

namespace lattice {
/**
 * Calculates the angle needed to maintain a specified takeup for two arms holding the same angle.
 *
 * @param mintakeup The minimum takeup needed
 * @param Larm The arm length
 * @param dap The horizontal distance between the arm pivot and the passive pulley
 * @param dpdp The horizontal distance between the passive pulley and the drive pulley
 * @return The angle each arm needs to be to get the specified takeup (in radians)
 */
double DoubleArmTakeupAngles(double mintakeup, double Larm, double dap, double dpdp);

/**
 * Calculates the angle a single arm needs to be at to maintain a specified takeup.
 *
 * @param mintakeup The minimum takeup needed
 * @param Larm The arm length
 * @param dap The horizontal distance between the arm pivot and the passive pulley
 * @param dpdp The horizontal distance between the passive pulley and the drive pulley
 * @return The angle one arm needs to be to get the specified takeup (in radians)
 */
double SingleArmTakeupAngle(double mintakeup, double Larm, double dap, double dpdp);

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
double GetTakeup(double leftArmAngle, double rightArmAngle, double Larm, double dap, double dpdp);

/**
 * Calculates the corresponding angle to maintain a takeup given one arm angle
 *
 * @param Larm The arm length
 * @param dap The horizontal distance between the arm pivot and the passive pulley
 * @param dpdp The horizontal distance between the passive pulley and the drive pulley
 * @param otherAngle The angle of the other arm in degrees
 * @return The angle (in degrees) the other arm needs to be at to maintain the specified takeup
 */
double GetCorrespondingAngle(double mintakeup, double Larm, double dap, double dpdp, double otherAngle);

/**
 * Calculates the relative rates the arms need to move to maintain a specific takeup
 *
 * @param doubleArmAngle The arm angles needed for both arms to maintain a previously specified takeup
 * @param singleArmAngle The arm angle needed for a single arm to maintain a previously specified takeup
 * @return The rate at which the arms need to move to minimize a loss in takeup
 */
double GetRelativeArmRates(double doubleArmAngle, double singleArmAngle);
}  // namespace lattice
