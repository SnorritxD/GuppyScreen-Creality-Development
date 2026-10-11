# 🐧 GuppyScreen Creality Development

A community-driven research and development repository for adapting,
building, testing, and investigating **GuppyScreen** on Creality 3D
printers.

This project focuses on evidence-based compatibility research: hardware,
firmware, Linux environments, display systems, touch input, CPU
architectures, toolchains, and runtime behaviour.

## Project Goal

The goal is not to assume compatibility between printers, but to
measure, test, document, and understand the differences required to make
GuppyScreen work on different Creality platforms.

A working solution on one printer does not automatically support
another.

## Development Workflow

Development happens on a development PC.

The printer is the target device for investigation and testing.

    Development PC
     |
     +-- Source changes
     +-- Build
     +-- Analysis
     +-- Test package
     |
     v
    Creality Printer
     |
     +-- Hardware testing
     +-- Firmware testing
     +-- Diagnostics

Do not use the printer as the normal development environment.

## Development Philosophy

This project follows an evidence-first workflow:

    Inspect
     ↓
    Measure
     ↓
    Understand
     ↓
    Create hypothesis
     ↓
    Test
     ↓
    Document
     ↓
    Modify

Changes should be based on information from the real target device.

## Current Baseline

The current baseline uses the original GuppyScreen architecture:

-   LVGL 8.3
-   Original GuppyScreen source structure
-   Hardware compatibility before LVGL migration

## Research Areas

### Hardware

-   SoC identification
-   CPU architecture
-   Endianness
-   ABI
-   Instruction set
-   Memory layout

### Linux

-   Kernel versions
-   Device nodes
-   Dynamic loader
-   Libraries
-   Filesystem layout

### Display

-   Framebuffer devices
-   Resolution
-   Virtual framebuffer layout
-   Panning behaviour
-   Display services

### Touchscreen

-   Input devices
-   Event nodes
-   Coordinates
-   Multitouch behaviour

### Firmware

-   GUI applications
-   System binaries
-   Startup services
-   Firmware differences

## Investigation Commands

CPU information:

``` bash
uname -a
uname -m
cat /proc/cpuinfo
```

Framebuffer:

``` bash
ls -l /dev/fb*
```

Input:

``` bash
ls -l /dev/input/
```

Executable analysis:

``` bash
file <binary>
readelf -h <binary>
readelf -l <binary>
readelf -d <binary>
```

## Compatibility Rules

A printer is not supported because it:

-   has the same brand
-   has the same screen size
-   has a similar CPU
-   uses similar firmware

Compatibility must be verified through testing.

## Cross Compilation

The target toolchain must match:

-   CPU architecture
-   Endianness
-   ABI
-   Instruction set
-   Floating point ABI
-   libc
-   Dynamic loader

A valid ELF file does not automatically mean it can run on the printer.

## Framebuffer Investigation

Do not assume:

    /dev/fb0

is always correct.

The correct framebuffer must be identified on the target device.

Investigate:

-   framebuffer device
-   visible resolution
-   virtual resolution
-   bits per pixel
-   stride
-   panning behaviour

## Touchscreen Investigation

Do not assume:

    /dev/input/event0

is always correct.

Identify the real touchscreen device from the printer.

## Safety

Embedded testing can cause:

-   frozen displays
-   broken GUI
-   lost input
-   unstable firmware behaviour

Always:

1.  Keep backups
2.  Change one thing at a time
3.  Keep recovery access
4.  Record firmware versions
5.  Document results

## Current Research Platform

### Creality K1C 2025/2026

The K1C is one research platform for this project.

Investigated topics include:

-   Ingenic X2600 / XBurst II
-   MIPS executable compatibility
-   ABI compatibility
-   Dynamic loader compatibility
-   Framebuffer behaviour
-   Touchscreen input
-   Sleep/wake behaviour
-   Firmware differences
-   LVGL integration

The K1C is a research platform, not the definition of the entire
project.

## Development Records

Each printer should document:

    Printer:
    Hardware revision:
    Firmware:
    SoC:
    CPU architecture:
    ABI:
    Kernel:
    Framebuffer:
    Touchscreen:
    Toolchain:
    Known issues:
    Test results:

## Contributing

Useful contributions include:

-   Hardware research
-   Firmware comparisons
-   Build fixes
-   Toolchain information
-   Compatibility testing
-   Documentation
-   Reproducible test results

## Final Rule

# Measure first. Change second. Document everything.

Do not guess when the printer can provide the answer.

## Credits

Based on the original GuppyScreen project, LVGL, open-source embedded
Linux tools, and community research.

## License

See the individual project components for their applicable licenses.
