# LATTICE Codebase
This is the codebase for Caltech's 2022 Big Idea Challenge Proposal LATTICE.

# Running Code

To support a more expansive set of files, the [PlatformIO](https://docs.platformio.org/en/latest/what-is-platformio.html) will be used. It supports several IDE's (e.g. having an [extension for VSCode](https://docs.platformio.org/en/latest/integration/ide/index.html#desktop-ides)) which makes installation simple.

To support the running of multiple files and unit testing, the setup is slightly less straightforward than pressing the build or deploy buttons. The Platform IO commandline will have to be used.

## Creating a new main file
To setup a new main file, go to `platformio.ini` and then copy the `[env:example]` environment and the line below it rename it to what the main file will be testing/running e.g. `[env:encodertest]`. The resulting code addition should be something like:

```
[env:envname]
extends = env:due
```

Then you can make a main file in the following naming format: `main-envname.cpp`. 

## Building Code
Run: `pio run -e envname`

## Uploading Code
Run: `pio run -t upload -e envname`

## Testing Code
Run: `pio test -e native`  
This requires gcc to be installed. Follow these [instructions](https://docs.platformio.org/en/latest/platforms/native.html) to get that setup. Note that we won't be able to support actual hardware unit tests without alot more work so only write unit tests for non-hardware dependent things like math functions. 
