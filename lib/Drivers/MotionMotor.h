namespace lattice {
class MotionMotor {
   public:
    MotionMotor(int enablePort, int pwmPort, int analogInPort, double maxRPM = 6000, double resolution = 12);
    void EnableMotor();
    void DisableMotor();
    void SetVelocity(double rpm);
    double GetVelocity();

   private:
    const int kBitResolution;
    const int kEnablePort;
    const int kPWMPort;
    const int kAnalogInputPort;
    const double kMaxRPM;
    double ScalePercentOutput(double rpm);
    double ScaleInput(double percentInput);
};
}  // namespace lattice