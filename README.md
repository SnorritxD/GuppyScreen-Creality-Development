# GuppyScreen – Creality Development

Community development environment for adapting GuppyScreen to Creality 3D printers.

This repository is intended for developers, testers, researchers, and contributors who want to investigate, port, adapt, build, test, and improve GuppyScreen support for Creality printers and hardware platforms.

> **This is a development repository.**
>
> It is **not** a universal installer and it is **not** a ready-to-use release for every Creality printer.

---

## About This Repository

This repository provides a complete development environment for working on GuppyScreen with Creality hardware.

The goal is to provide the community with a common place where developers can:

- study the existing GuppyScreen implementation;
- build GuppyScreen from source;
- investigate Creality printer hardware;
- adapt GuppyScreen to different display systems;
- adapt touchscreen input;
- investigate framebuffer behavior;
- work with different SoCs and CPU architectures;
- test different LVGL versions and configurations;
- develop printer-specific changes;
- share patches and fixes;
- reproduce problems;
- document hardware discoveries;
- collaborate on support for additional Creality printers.

The repository intentionally contains the source code and development dependencies required to continue development.

Because of this, the repository is expected to be considerably larger than a normal end-user installation package.

---

## What Is GuppyScreen?

GuppyScreen is a lightweight touchscreen interface for Klipper/Moonraker-based 3D printers.

It uses **LVGL (Light and Versatile Graphics Library)** for the graphical user interface and communicates with the printer software environment to display and control printer functions.

GuppyScreen can be adapted to different hardware environments, but the hardware interface is not identical between Creality printer models.

Different models may use different:

- SoCs;
- CPU architectures;
- Linux kernels;
- framebuffer devices;
- framebuffer layouts;
- display controllers;
- touchscreen controllers;
- input event devices;
- kernel drivers;
- GUI services;
- system startup mechanisms;
- filesystem layouts;
- firmware versions.

Because of this:

> **Support for one Creality printer does not automatically mean support for another printer.**

---

## Purpose of This Repository

The primary purpose of this repository is to provide a common development base for bringing GuppyScreen to additional Creality printers and hardware platforms.

This repository is intended for research, development, experimentation, testing, and collaboration.

A developer working on another Creality printer may need to determine:

- which framebuffer is used by the display;
- which framebuffer format is used;
- the real display resolution;
- whether a virtual framebuffer height is used;
- which input event device belongs to the touchscreen;
- which touchscreen driver is loaded;
- which GUI process currently owns the display;
- which service starts the stock GUI;
- which CPU architecture is being used;
- which dynamic loader is available;
- which kernel drivers are involved;
- how the display behaves after suspend and resume.

These questions should be answered for the target printer before reliable support is developed.

---

## What You Can Do Here

This repository can be used for many different development tasks.

### Build GuppyScreen

Developers can build GuppyScreen from source using the included development environment.

This is useful for:

- testing source changes;
- testing LVGL changes;
- testing display changes;
- testing touchscreen changes;
- testing networking changes;
- testing UI changes;
- producing development binaries.

### Port GuppyScreen to Another Printer

A major purpose of this repository is supporting additional Creality printers.

A developer can investigate a new printer and create the required hardware-specific changes.

Examples include:

- framebuffer selection;
- touchscreen input selection;
- display initialization;
- framebuffer synchronization;
- framebuffer panning;
- input handling;
- architecture-specific build configuration;
- startup behavior;
- system integration.

### Investigate Printer Hardware

The repository can be used together with shell tools and a target printer to investigate the hardware environment.

Useful information includes:

- CPU and SoC information;
- Linux kernel information;
- CPU architecture;
- framebuffer devices;
- framebuffer resolution;
- framebuffer virtual resolution;
- bits per pixel;
- framebuffer stride;
- touchscreen devices;
- input event devices;
- touchscreen drivers;
- running GUI processes;
- startup services;
- filesystem layout.

The exact commands available depend on the firmware installed on the target printer.

### Develop Diagnostics

The repository can also be used to develop diagnostic tools that help determine how a particular Creality printer works.

Examples include:

- framebuffer diagnostics;
- display diagnostics;
- input diagnostics;
- hardware identification;
- runtime testing;
- compatibility testing.

---

## What This Repository Is Not

This repository is not:

- a universal GuppyScreen installer;
- a universal Creality firmware package;
- a guaranteed working build for every Creality printer;
- a replacement for the printer firmware;
- a replacement for the printer operating system;
- a guarantee that a binary built here will run on a particular printer;
- a reason to overwrite system files without investigation.

Do not assume that a binary working on one printer can simply be copied to another printer.

---

## Repository Structure

The repository contains the development source and supporting components.

- `README.md` — project overview and development guidance.
- `DEVELOPMENT.md` — development environment and build information.
- `Makefile` — project build system.
- `VERSION` — project version information.
- `src/` — main GuppyScreen application source.
- `lvgl/` — LVGL source.
- `lv_drivers/` — LVGL hardware drivers.
- `lv_touch_calibration/` — touchscreen calibration support.
- `libhv/` — networking and system-related library.
- `spdlog/` — logging library.
- `wpa_supplicant/` — wireless/networking development component.
- `assets/` — graphical assets.
- `themes/` — UI themes.
- `patches/` — development and hardware-specific patches.
- `fbinfo` — framebuffer diagnostic utility.
- `fbinfo.c` — framebuffer diagnostic utility source.
- `guppyscreen` — known-good reference binary.
- `build/` — normal development build output.
- `lv_conf.h` — LVGL configuration.
- `lv_drv_conf.h` — LVGL driver configuration.

The exact contents may evolve as development continues.

---

## Development Philosophy

The most important rule for Creality development is:

> **Do not assume that different printers use the same hardware configuration.**

Even when two printers appear to use the same display size or similar firmware, their internal implementation may differ.

Always investigate the target hardware first.

For example, one printer may use one framebuffer and touchscreen event device while another printer may use completely different devices.

A hardcoded configuration that works on one printer may completely fail on another printer.

Hardware-specific configuration should therefore be identified, tested, and documented rather than blindly copied.

---

## Printer Support

Support should be considered **printer-specific unless proven otherwise**.

A new printer should not be marked as supported simply because:

- GuppyScreen starts;
- the screen lights up;
- the UI appears;
- touch appears to work.

Proper support should also consider:

- display stability;
- framebuffer behavior;
- touch reliability;
- startup;
- shutdown;
- reboot;
- application restart;
- sleep and wake;
- printer communication;
- firmware compatibility;
- resource usage;
- long-running operation.

---

## Hardware Investigation

Before attempting to port GuppyScreen to a new printer, create a hardware profile.

At minimum, document:

- printer model;
- firmware version;
- SoC;
- CPU architecture;
- endianess;
- ABI;
- Linux kernel version;
- framebuffer device;
- framebuffer driver;
- display resolution;
- virtual framebuffer resolution;
- bits per pixel;
- stride;
- touchscreen device;
- touchscreen driver;
- stock GUI process;
- stock GUI service;
- dynamic loader.

Do not assume that `/dev/fb0` is the display.

Do not assume that `/dev/input/event0` is the touchscreen.

Both must be verified on the target printer.

---

## Framebuffer and Display Development

Framebuffer behavior is one of the most important areas when porting GuppyScreen to Creality hardware.

Do not assume that the framebuffer is simply a linear display surface.

Some devices may use:

- double buffering;
- virtual framebuffer heights;
- framebuffer panning;
- multiple layers;
- hardware composition;
- custom framebuffer drivers;
- display synchronization;
- vendor-specific IOCTL behavior.

For example, a framebuffer may report a physical display resolution of 800 × 480 while using a virtual framebuffer resolution of 800 × 960.

This can indicate that multiple framebuffer pages are available.

Changes involving framebuffer operations such as:

- `FBIOGET_VSCREENINFO`;
- `FBIOPUT_VSCREENINFO`;
- `FBIOPAN_DISPLAY`;
- `yoffset`;
- `virtual_yres`;
- `stride`;
- `bits_per_pixel`;

must be tested carefully on the target hardware.

---

## Touchscreen Development

Touchscreen handling is equally hardware-specific.

Different Creality printers may use different:

- touchscreen controllers;
- Linux input drivers;
- `/dev/input/eventX` devices;
- coordinate systems;
- calibration requirements;
- suspend/resume behavior.

A touchscreen working during normal operation does not automatically mean that it is correct after:

- reboot;
- application restart;
- sleep;
- wake;
- firmware restart.

These scenarios should be tested when adding support.

---

## LVGL Development

GuppyScreen uses LVGL as its graphical framework.

This repository provides the development environment around the LVGL version currently used by the source tree.

When changing LVGL versions, do not assume that a newer version is a drop-in replacement.

Major LVGL changes can affect:

- object APIs;
- display APIs;
- input APIs;
- rendering;
- drivers;
- event handling;
- timers;
- memory management;
- framebuffer integration.

If testing another LVGL version, document:

1. which version was tested;
2. which source changes were required;
3. which drivers were changed;
4. whether the application built;
5. whether it started;
6. whether display output worked;
7. whether touch worked;
8. whether long-term operation worked.

---

## Building GuppyScreen

Clone the repository with:

    git clone --recursive https://github.com/SnorritxD/GuppyScreen-Creality-Development.git
    cd GuppyScreen-Creality-Development

If the repository was cloned without `--recursive`, initialize the submodules with:

    git submodule update --init --recursive

The exact build requirements and development environment are documented in `DEVELOPMENT.md`.

Always read `DEVELOPMENT.md` before changing the build environment.

The normal development build is performed using the project's `Makefile`.

---

## Build Environment

The project may require a cross-compilation toolchain when building for Creality printer hardware.

The correct compiler depends on the architecture of the target printer.

For example, the Creality K1C 2025/2026 development environment uses a MIPS little-endian target.

Do not select a compiler based only on the word "MIPS".

The following properties matter:

- architecture;
- endianess;
- ABI;
- CPU instruction set;
- floating-point ABI;
- dynamic loader;
- libc compatibility.

A binary can successfully compile while still being completely incompatible with the target printer.

---

## Testing on a Printer

A development binary should be tested on the actual target hardware.

Before testing:

1. Identify the printer model.
2. Record the firmware version.
3. Record the CPU architecture.
4. Record the framebuffer configuration.
5. Record the touchscreen configuration.
6. Record the stock GUI process and service.
7. Make a backup of anything that may be changed.
8. Keep a known-good recovery method available.

Never assume that a development binary is safe simply because it builds successfully.

---

## Local and Simulator Development

Local development and simulation can be used for UI and application development where possible.

This is useful for:

- UI changes;
- layout changes;
- themes;
- application logic;
- debugging source code.

However, local simulation does not reproduce every hardware-specific behavior.

It may not reproduce:

- Creality framebuffer drivers;
- hardware framebuffer synchronization;
- touchscreen drivers;
- vendor-specific display layers;
- suspend/resume behavior;
- target CPU architecture;
- target Linux kernel behavior.

Hardware testing remains necessary for hardware-specific changes.

---

## Configuration

Runtime configuration such as `guppyconfig.json` may be generated or maintained outside the source tree depending on the target installation.

Do not commit:

- personal runtime configuration;
- credentials;
- passwords;
- API keys;
- private network information;
- machine-specific secrets.

Development configuration and production runtime configuration should remain separate.

---

## Important Safety Notes

This repository is intended for development.

When working directly on a Creality printer:

### Always Keep a Recovery Path

Before modifying system files:

- make backups;
- record the original state;
- understand how to restore it;
- avoid deleting original firmware files;
- avoid overwriting files unnecessarily.

### Never Blindly Disable the Stock GUI

A printer may depend on vendor startup services for:

- display initialization;
- touchscreen initialization;
- hardware services;
- printer control;
- system integration.

If a stock service must be disabled during testing, document exactly what was changed and how to restore it.

### Never Assume a Binary Is Compatible

A binary compiled for one architecture or ABI may not run on another.

### Do Not Start With Destructive Changes

Start with:

- diagnostics;
- read-only inspection;
- backups;
- isolated tests.

---

## Known-Good Reference Binary

The root-level `guppyscreen` file is retained as a known-good development reference.

It is **not normal build output**.

Normal development output belongs under `build/`.

The reference binary exists so developers can:

- compare behavior;
- compare file hashes;
- verify whether a new build differs;
- test regressions;
- preserve a known working point.

Do not replace the reference binary merely because a new build was created.

If the reference binary is intentionally replaced, document why.

---

## Working With a New Creality Printer

When starting support for a new printer, do not immediately start changing GuppyScreen source code.

First create a hardware profile.

Document:

- Printer:
- Firmware:
- SoC:
- Architecture:
- Kernel:
- Framebuffer:
- Framebuffer driver:
- Resolution:
- Virtual resolution:
- Bits per pixel:
- Stride:
- Touch device:
- Touch driver:
- Stock GUI:
- GUI service:
- Dynamic loader:

Then investigate the existing system.

---

## Recommended Development Workflow

### Step 1 — Identify the Hardware

Collect CPU, kernel, framebuffer, input, and GUI information.

### Step 2 — Preserve the Original System

Create backups before changing anything.

### Step 3 — Determine Display Ownership

Find out which process currently uses the framebuffer.

### Step 4 — Determine Touchscreen Ownership

Identify the correct `/dev/input/eventX` device and driver.

### Step 5 — Build Diagnostic Tools

Use tools such as `fbinfo` to verify the framebuffer before involving the complete GUI.

### Step 6 — Build GuppyScreen

Compile using the correct target toolchain.

### Step 7 — Test Display Output

Verify:

- correct framebuffer;
- correct resolution;
- correct pixel format;
- correct orientation;
- no corruption.

### Step 8 — Test Touch

Verify:

- coordinate mapping;
- calibration;
- gestures;
- repeated input;
- restart behavior.

### Step 9 — Test System Integration

Test:

- startup;
- application restart;
- reboot;
- sleep;
- wake;
- printer communication.

### Step 10 — Document Everything

Record the hardware configuration and all changes required.

This documentation is extremely valuable for the next contributor working on the same printer.

---

## Contributing

Contributions are welcome.

Useful contributions include:

- new printer support;
- display driver improvements;
- framebuffer fixes;
- touchscreen fixes;
- LVGL compatibility work;
- UI improvements;
- performance improvements;
- diagnostic tools;
- documentation;
- build improvements;
- reproducible bug reports;
- hardware research.

When contributing printer-specific work, clearly identify the printer model and firmware version involved.

---

## Pull Requests

A good pull request should explain:

### What changed?

Describe the actual change.

### Why was it necessary?

Explain the problem it solves.

### Which printer(s) does it affect?

Be explicit.

### Which firmware versions were tested?

Include them when known.

### How was it tested?

Describe the tests that were actually performed.

For example:

- Build: Pass
- Application startup: Pass
- Display: Pass
- Touch: Pass
- Reboot: Pass
- Sleep/wake: Pass
- Long-running test: Pass

If something was not tested, say so.

Do not claim compatibility that has not been tested.

---

## Reporting Problems

When reporting a problem, provide as much useful information as possible.

At minimum, provide:

- Printer model;
- firmware version;
- SoC;
- architecture;
- GuppyScreen version or commit;
- LVGL version;
- framebuffer;
- touch device;
- problem description;
- expected behavior;
- actual behavior;
- steps to reproduce.

Where possible, also include relevant diagnostics.

A photo or video can be extremely useful for display problems.

For framebuffer problems, include relevant framebuffer information.

For touchscreen problems, include the input-device information.

---

## Production Releases

This repository is intended for development.

Once support for a specific printer is considered stable, it can be packaged separately for end users.

A production repository or release should:

- target a clearly defined printer;
- document supported firmware;
- provide a tested installation method;
- provide rollback and recovery;
- avoid exposing unnecessary development files;
- avoid shipping development-only tools;
- clearly state hardware compatibility.

The development repository should remain the place where experimental and cross-printer development takes place.

---

## Relationship to Printer-Specific Repositories

Printer-specific production repositories should remain separate from this development repository.

The intended relationship is:

    GuppyScreen-Creality-Development
        |
        +-- research
        +-- development
        +-- experiments
        +-- patches
        +-- cross-printer support
                |
                +-- K1C
                +-- K1 Max
                +-- K2
                +-- K3
                +-- other printers

A printer-specific production repository can then contain only the files required to safely install and run the tested release for that printer.

This separation helps prevent experimental development changes from accidentally becoming production installation changes.

---

## Project Status

This repository is an active development environment.

Support for individual printers should be considered experimental until that printer has been properly investigated and tested.

The fact that GuppyScreen works on one Creality printer does **not** mean that it works on all Creality printers.

Hardware support should be added incrementally and documented clearly.

---

## Credits

This development repository builds on the GuppyScreen project and its open-source ecosystem.

Credit goes to the original GuppyScreen developers and contributors, as well as the developers of the libraries and components used by the project.

Additional credit belongs to community contributors who investigate Creality hardware, test builds, identify hardware differences, and contribute fixes and documentation.

---

## License

Please refer to the license files and upstream project licensing information included in this repository.

Individual third-party components may have their own licenses.

When modifying or redistributing components, respect the applicable licenses of:

- GuppyScreen;
- LVGL;
- LVGL drivers;
- libhv;
- spdlog;
- wpa_supplicant;
- other included or linked components.

---

## Final Note

The goal of this repository is not simply to make GuppyScreen run on one printer.

The goal is to make it easier for the community to understand how Creality hardware differs between printers, develop the required adaptations, test them properly, and eventually turn successful development work into reliable printer-specific releases.

If you are working on a new Creality printer:

**Investigate first.  
Document what you find.  
Build second.  
Test carefully.  
Share the results.**
