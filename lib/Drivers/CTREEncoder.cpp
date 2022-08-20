#include "CTREEncoder.h"

using namespace lattice;

CTREEncoder::CTREEncoder(int encoderForwardPort, int encoderBackwardPort)
    : mEncoder(encoderForwardPort, encoderBackwardPort) {}

long CTREEncoder::GetRawEncoderPosition() {
    return mEncoder.read();
}

double CTREEncoder::GetEncoderRotation() {
    return mEncoder.read() / kCPR;
}

void CTREEncoder::ResetEncoderPosition() {
    mEncoder.write(0);
}