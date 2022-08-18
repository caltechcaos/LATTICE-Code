#include <Arduino.h>
#include <Encoder.h>

namespace lattice{
class CTREEncoder {
    public:
        CTREEncoder(int encoderForwardPort, int encoderBackwardPort);
        long GetRawEncoderPosition();
        double GetEncoderRotation();
        void ResetEncoderPosition();
    private:
        Encoder m_encoder;
        const double kCPR = 4096.0;
};
}
