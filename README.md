# Home Automation library for mBed

Library for a Home Automation system based on custom hardware board and an FRDM64F.

Check out the examples to see how to use the library.

## Requirements

You will need to enable C++11 compilation. This can be achieved by changing the three json configurations files in `code/mbed-os/tools/profiles`. Change the `cxx` section of the `GCC_ARM` config to the following:

```json
        "cxx": ["-std=gnu++11", "-fno-rtti", "-Wvla"],
```

## Logging

If you wish to enable logging you will need to activate it through the `mbed_lib.json` file found in this library. Just add the `DO_SIMPLE_LOG` macro as shown below.

```json
{
    "name": "home_automator",
    "macros": ["DO_SIMPLE_LOG"]
}
```

In that case you will also need to add [Simple Logger](https://github.com/BioBoost/simple_logger) as a library:

```shell
mbed add https://github.com/BioBoost/simple_logger
```

Make sure to setup a logger as the default logging mechanism throws all messages away.