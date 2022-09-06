#include <Arduino.h>
#include <Encoder.h>

namespace lattice {
/**
 * Class to interface with the CTRE Magnetic Encoder.
 */
class CTREEncoder {
   public:
    /**
     * Constructor for the CTRE Magnetic Encoder.
     *
     * @param encoderForwardPort The A channel of the CTRE Mag Encoder
     * @param encoderBackwardPort The B channel of the CTRE Mag Encoder
     * @param gearing The gearing of the motor. (50:1 -> 50, 50:2 -> 25, etc)
     */
    CTREEncoder(int encoderForwardPort, int encoderBackwardPort, double gearing = 1.0);

    /**
     * Gets the raw encoder position in counts
     *
     * @return The acculumated encoder counts (0-4096)
     */
    long GetRawEncoderPosition();

    /**
     * Gets the encoder position in rotational units
     *
     * @return The encoder position in rotations
     */
    double GetPosition();

    /**
     * Sets the encoder position to zero
     */
    void ResetEncoderPosition();

   private:
    Encoder mEncoder;
    const double kGearing;
    static constexpr double kCPR = 4096.0;  // counts per rotation
};
}  // namespace lattice
