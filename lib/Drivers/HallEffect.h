#pragma once

namespace lattice {
class HallEffect {
   public:
    HallEffect(int signalPort);
    void Setup();
    bool Get();

   private:
    const int kPort;
};
}  // namespace lattice