#include "CTREEncoder.h"

using namespace lattice;

CTREEncoder::CTREEncoder(int encoderForwardPort, int encoderBackwardPort, double gearing)
    : mEncoder(encoderForwardPort, encoderBackwardPort), kGearing(gearing) {}

long CTREEncoder::GetRawEncoderPosition() {
    return mEncoder.read();
}

double CTREEncoder::GetPosition() {
    return mEncoder.read() / kCPR / kGearing;
}

void CTREEncoder::ResetEncoderPosition() {
    mEncoder.write(0);
}