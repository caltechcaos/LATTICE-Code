namespace lattice {
/**
 * Class to control the ESCON Motor Controllers intended to power the motion motors for the shuttle.
 */
class MotionMotor {
   public:
    /**
     * Constructor for the Motion Motor.
     *
     * @param enablePort The port on the arduino that is meant to send the enable signal.
     * @param pwmPort The port on the arduino that sends PWM output to the motor controller
     * @param analogInPort The port on the arduino that reads in the Analog Input information from the motor controller
     * @param maxRPM The maximum RPM to run on the motor (determines output scaling)
     * @param resolution The bit resolution of the PWM port
     */
    MotionMotor(int enablePort, int pwmPort, int analogInPort, double maxRPM = 6000, double resolution = 12, double deadband = 0.1);

    /**
     * Enables the motor to be controlled
     */
    void EnableMotor();

    /**
     * Disables the motor
     */
    void DisableMotor();

    /**
     * Commands the motor to a specified RPM
     */
    void SetVelocity(double rpm);

    /**
     * Gets the current motor velocity in RPM
     *
     * @return The motor velocity in RPM
     */
    double GetVelocity();

   private:
    const int kEnablePort;
    const int kPWMPort;
    const int kAnalogInputPort;
    const double kMaxRPM;
    const int kBitResolution;
    const int kDeadband;

    /**
     * Scales the RPM into a percent output that can be passed into the Motor Controller.
     *
     * @param rpm The target RPM
     * @return The percent output to be sent to the motor controller
     */
    double ScalePercentOutput(double rpm);

    /**
     * Scales the Analog Percent Input into the actual value.
     *
     * @param percentInput The percent analog input
     * @return The input scaled into the units we want (RPM)
     */
    double ScaleInput(double percentInput);
};
}  // namespace lattice