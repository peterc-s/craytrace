# craytrace
A very rudimentary raytracer written in C

Follows [RayTracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) up until section 10.

# Usage
## Requirements
- GCC
- GNU Make

This was only tested on a GNU/Linux machine, it may still work on a Windows/Mac/BSD system, but it hasn't been tested.

## Optional Requirements
- GDB - for debugging.
- timg + a kitty graphics compatible terminal emulator.

## Building
The Makefile supports a few different commands:
```bash
make # will just compile the release binary
make release # ^

make run # will compile and run the release binary
make release-run # ^

make debug # will compile the debug binary
make debug-rum # will compile and run the debug binary

make all # will compile release and debug binary

make debugger # will compile debug binary and open GDB with it

make render # will make the release binary, and render the output image to `image.ppm`, even if the code hasn't change.
make image # will make the release binary, and render the output image to `image.ppm` if it didn't already exist or if the release binary has changed, then view it with `timg`.

make clean # will remove the build directory, the output binaries, and the output image.

make install # will install to /usr/bin/c-raytrace
make uninstall # will uninstall from /usr/bin/c-raytrace
```

The Makefile accepts some configuration too, which can be set from the command line like this:
```bash
make all CC="gcc" # set the C compiler
make all CFLAGS="-flags" # don't use base compiler flags, set your own
make all CFLAGS_EX="-flags" # use base compiler flags but add extras
make all CFLAGS_DEBUG="-flags" # don't use base debug flags, set your own
make all CFLAGS_RELEASE="-flags" # don't use base release flags, set your own

make all SRC="/path/to/src" # set source directory
make all BUILDDIR="/path/to/build" # set build directory
make install INSTALLDIR="/path/to/install" # set install directory (WARNING: this requires sudo, and unless you update the INSTALLDIR in the Makefile itself, `make uninstall` will default to `sudo rm /usr/bin/c-raytrace`)
make uninstall INSTALLDIR="/path/to/install" # uninstall from specific directory

make all NAME="c-raytrace" # set output binary name, debug binary will default to `$(NAME)-debug`

make debugger DEBUGGER="gdb" # set the debugger to use

make image VIEWER="timg" VIEWER_FLAGS="-U -C" # set the program to view the output image with, including flags passed to it.
```
But, I'd recommend setting these options in the Makefile itself for ease of use.

# TODO
If I ever revisit this, it needs a full re-write. The early design decisions I made meant that creating materials would be messy and difficult so:
- [ ] Rewrite with considerations for adding materials.
- [ ] Consider whether dynamic memory allocation for object arrays is necessary, setting a sensible static cap may be better in terms of performance?
