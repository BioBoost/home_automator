# Home Automation library for mBed

Library for a Home Automation system based on custom hardware board and an FRDM64F.

Check out the examples to see how to use the library.

## Requirements

You will need to enable C++11 compilation. This can be achieved by changing the three json configurations files in `code/mbed-os/tools/profiles`. Change the `cxx` section of the `GCC_ARM` config to the following:

```json
        "cxx": ["-std=gnu++11", "-fno-rtti", "-Wvla"],
```

## Logging

If you wish to enable logging you will need to activate it through the `mbed_lib.json` file found in this library. Just add the `ENABLE_SIMPLE_LOGGER` macro as shown below.

```json
{
    "name": "home_automator",
    "macros": ["ENABLE_SIMPLE_LOGGER"]
}
```

Make sure to setup a logger as the default logging mechanism throws all messages away.