namespace lattice {
/**
 * This is an enum to specify which type of I2C Port the lidarlite is plugged into
 * kDefault: SDA/SCL
 * kOne: SDA1/SCL1
 */
enum WireType {
    kDefault,
    kOne
};
}  // namespace lattice