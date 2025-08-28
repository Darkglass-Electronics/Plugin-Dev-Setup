# Audio Plugin Development Documentation for Darkglass Anagram

This document contains information on how to build your audio plugins for the [Darkglass Anagram](https://www.darkglass.com/products/anagram/).

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

## Cross compilation

Because Anagram runs a custom Linux arm64 system, any system that does not match this architecture needs cross-compilation.  
Even those that do, the glibc and kernel version are very unlikely to match, so cross-compilation is pretty much always needed.

For this step we make use of [crosstool-ng](https://crosstool-ng.github.io/) running through [mod-plugin-builder](https://github.com/mod-audio/mod-plugin-builder/).  
These tools only run properly under a Linux system, so if you are not using Linux you will need to go through docker.

### Linux: Direct toolchain bootstrap

See [BUILDING-LINUX.md](BUILDING-LINUX.md) for instructions on how to build when using Linux.

### Other OSes: Setup toolchain through docker

See [BUILDING-DOCKER.md](BUILDING-DOCKER.md) for instructions on how to build when not using Linux, through the use of docker.
