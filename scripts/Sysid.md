# System Identification Routines

To determine feedforward values for our motors, we will be utilizing the [FRC System identification tool](https://docs.wpilib.org/en/stable/docs/software/pathplanning/system-identification/introduction.html).

## Procedure
The tool performs 4 tests: quasistatic forward, quasistatic backward, dynamic forward, dynamic backward. The quasistatic tests ramp the voltage throughout the entirety of the test (helps fit a line to the velocity) while the dynamic tests set a constant voltage (helps fit a line to the acceleration). The `main-sysidouttest.cpp` provides a structure that allows you to run all of these tests from the commandline. 

Do note that we need access to the data being printed out into the Serial Monitor which we can do by typing the following command into a PlatformIO commandline:
```pio device monitor -e sysidouttest > loggingtest.txt```
From this console you can press a key and it sends it as input. Here are the defined button bindings:
- `s`: Start a test
- `e`: End a test
- `q`: Set to quasistatic test
- `d`: Set to dynamic test mode
- `f`: Set to forward
- `b`: Set to backward

## Getting Data From Our End
We must run the `process-sysid.py` script (`python scripts/process-sysid.py`) to get the raw data collected into the [json spec](https://github.com/wpilibsuite/sysid/blob/main/docs/data-collection.md#non-drivetrain-mechanisms) that sysid reads.

### Parameter Setup
- `filename`: The file we're reading from
- `outfile`: The output json file
- `rotational_conversion_factor`: Conversion factor between the units that the position measurement is in to rotations (e.g. encoder counts -> rotations)
- `rotation_to_measurement_factor`: Conversion factor between rotations and the unit the system is intended to be in (e.g. rotations -> distance traveled, or rotations -> degrees)
- `test_type`: Either `Simple` for regular motor characterization or `Elevator` for elevator motor characterization.
- `unit`: The name of the unit of the system (e.g. Rotations), although this doesn't really affect the analysis this is just for reference.

### Technical Notes
This workflow assumes that we can read position data and command voltage. However, we sill need velocity data and so the python script numerically differentiates the position values using a [5 point stencil](https://en.wikipedia.org/wiki/Five-point_stencil) and then applies a [median filter](https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.medfilt.html) with window size of 5 to smoothen the data.