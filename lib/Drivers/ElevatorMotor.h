#include "CTREEncoder.h"
#include "Servo.h"

namespace lattice {
class ElevatorMotor {
   public:
    /**
     * Creates an elevator on the given pins
     * @param kElevatorPin The pin the elevator motor is plugged into
     * @param kEncoderFwdPin The pin the encoder's forward wire is plugged into
     * @param kEncoderBckPin The pin the encoder's reverse wire is plugged into
     */
    ElevatorMotor(const int kElevatorPin, const int kEncoderFwdPin, const int kEncoderBckPin);

    /**
     * Sets up for control of the elevator motor. Must be called before using it
     */
    void Setup();

    /**
     * Runs the elevator at the given power
     * @param power The power to run at, from -1 to 1, inclusive
     * @return True on successful motion, false otherwise
     */
    bool Run(double power);

    /**
     * Sets the brake to the given value, overriding any movement
     * @param brake True to stop, false to go
     * @return True on success, false otherwise
     */
    bool SetBrake(bool brake);

    /**
     * Rezeros the elevator's encoder at its current position
     */
    void Zero();

    /**
     * Returns the encoder's position
     * @return The encoder's position, in number of rotations
     */
    double GetPosition();

   private:
    CTREEncoder encoder;
    Servo motor;
    const int kPin;
    bool brake;
};
}  // namespace lattice