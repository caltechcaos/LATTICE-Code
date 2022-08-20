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
     */
    CTREEncoder(int encoderForwardPort, int encoderBackwardPort);

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
    double GetEncoderRotation();

    /**
     * Sets the encoder position to zero
     */
    void ResetEncoderPosition();

   private:
    Encoder mEncoder;
    const double kCPR = 4096.0;  // counts per rotation
};
}  // namespace lattice
