#include "CTREEncoder.h"

using namespace lattice;

CTREEncoder::CTREEncoder(int encoderForwardPort, int encoderBackwardPort): m_encoder(encoderForwardPort, encoderBackwardPort){
}

long CTREEncoder::GetRawEncoderPosition() {
    return m_encoder.read();
}

double CTREEncoder::GetEncoderRotation() {
    return m_encoder.read() / kCPR;
}

void CTREEncoder::ResetEncoderPosition() {
    m_encoder.write(0);
}