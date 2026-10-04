# GuppyScreen Creality Development

A community development and research repository for adapting, building, testing, and investigating **GuppyScreen** on Creality 3D printers.

This repository is intended as a development environment for researching printer-specific hardware, firmware, display systems, touchscreen input, Linux environments, CPU architectures, toolchains, and GuppyScreen compatibility.

It is **not limited to one printer model**.

---

## Important: Development PC vs. Printer

This repository is developed on a **development PC**.

The repository should be cloned, modified, built, and committed on your development computer.

The printer is the **target device used for hardware investigation and testing**.

Do **not** clone this repository directly onto the printer as your normal development workflow.

Typical workflow:

```text
Development PC
      │
      ├── Clone repository
      ├── Modify source
      ├── Build
      ├── Inspect binaries
      └── Commit / push changes
             │
             ▼
       Transfer test build
             │
             ▼
       Creality printer
             │
             ├── Run test
             ├── Inspect hardware
             ├── Inspect firmware
             └── Collect diagnostic information
```

The exact build and deployment process may differ between printer models.

---

# Project Purpose

GuppyScreen is an alternative touchscreen interface originally developed for compatible 3D printers.

This repository exists to investigate what is required to make GuppyScreen work on additional Creality printer platforms.

Different Creality printers can use significantly different:

* CPUs
* CPU architectures
* CPU instruction sets
* endianness
* ABIs
* C libraries
* dynamic loaders
* Linux kernels
* framebuffer devices
* framebuffer layouts
* touchscreen devices
* display drivers
* display services
* firmware versions
* system applications
* power-management behavior
* filesystem layouts
* build environments

Because of these differences, support for one printer **does not automatically mean support for another printer**.

The purpose of this repository is therefore to document and verify those differences instead of assuming that hardware is compatible.

---

# Project Philosophy

The project follows an **evidence-first development approach**.

The general process is:

```text
Inspect
   ↓
Measure
   ↓
Understand
   ↓
Form a hypothesis
   ↓
Test
   ↓
Document the result
   ↓
Modify the source when justified
```

Changes should be based on information obtained from:

* the printer
* its firmware
* existing binaries
* kernel information
* sysfs
* device nodes
* framebuffer information
* input devices
* executable headers
* libraries
* configuration files
* controlled experiments

Avoid making hardware-specific changes simply because they work on another printer.

---

# Current Development Baseline

The current development baseline uses the original GuppyScreen architecture based on:

**LVGL 8.3**

The repository currently uses the original GuppyScreen source structure rather than attempting to migrate the project to LVGL 9.

This is intentional.

A migration to another LVGL major version changes a large number of APIs and can introduce problems that are unrelated to printer hardware compatibility.

Therefore, hardware compatibility work should first be established against the original LVGL 8.3 baseline.

Future LVGL upgrades may be investigated separately once the hardware-specific implementation is understood.

---

# Supported Development Platforms

Development can be performed on:

* Linux
* Windows using WSL2
* macOS

The exact commands may differ between operating systems.

Linux is generally the most convenient environment for cross-compilation and embedded Linux development, but the project should not require every contributor to use the same desktop operating system.

---

# Repository Scope

This repository is intended to support work such as:

* GuppyScreen development
* printer hardware research
* firmware research
* CPU architecture investigation
* cross-compilation research
* framebuffer investigation
* touchscreen investigation
* display-system investigation
* Linux target debugging
* LVGL development
* build-system development
* printer-specific compatibility work
* documentation
* reproducible testing
* community contributions

The repository should remain useful even when a particular printer has not yet been fully supported.

---

# Printer Compatibility

Printer compatibility must be determined experimentally.

A printer should not be considered supported simply because it:

* uses the same manufacturer
* has a similar screen
* uses a similar CPU
* has the same advertised resolution
* uses the same firmware family
* uses the same Linux kernel version
* uses another printer's GuppyScreen binary successfully

These similarities can be useful clues, but they do not prove binary or hardware compatibility.

---

# Hardware Investigation

Before modifying GuppyScreen for a new printer, investigate the target hardware.

Useful information includes:

```bash
uname -a
uname -m
cat /proc/cpuinfo
```

Also inspect relevant system information such as:

```bash
ls -l /dev/fb*
ls -l /dev/input/
```

and, where available:

```bash
ls /sys/class/graphics/
ls /sys/class/input/
```

The exact commands available may differ between printer firmware versions.

---

# CPU Architecture

The target CPU architecture must be determined from the printer itself.

Do not assume that all Creality printers use the same architecture.

Useful information includes:

```bash
uname -m
cat /proc/cpuinfo
```

Existing executable files should also be inspected.

For example:

```bash
file <binary>
```

and:

```bash
readelf -h <binary>
```

can provide information about:

* architecture
* ELF class
* endianness
* ABI
* instruction set
* executable type

A CPU family name alone is not enough to select a working cross-compiler.

For example, a printer may use a MIPS-based CPU, but the exact architecture, endianness, ABI, instruction-set requirements, C library, and dynamic loader must still be verified on the target device.

---

# Cross-Compilation

Cross-compilation is required when the development PC and printer use different CPU architectures.

A suitable toolchain must match the target environment.

Important properties include:

* CPU architecture
* endianness
* ABI
* instruction set
* floating-point ABI
* C library
* dynamic loader
* compiler compatibility
* linker compatibility
* required runtime libraries

Do not select a toolchain based only on a CPU name.

A compiler that produces a valid ELF executable can still produce a binary that cannot run on the printer.

---

# Target Executable Investigation

Existing printer binaries are valuable sources of information.

Before attempting to reproduce a binary, inspect known target executables.

Useful commands include:

```bash
file <binary>
```

```bash
readelf -h <binary>
```

```bash
readelf -l <binary>
```

```bash
readelf -d <binary>
```

```bash
strings <binary>
```

and, when useful:

```bash
objdump -f <binary>
```

These can reveal information about:

* ELF format
* architecture
* endianness
* ABI
* interpreter
* shared-library dependencies
* compiler-related information
* embedded paths
* symbols
* strings
* build information

A working stock executable can be especially useful as a reference when investigating compatibility.

---

# Dynamic Loader

The target dynamic loader must be investigated before relying on dynamically linked test binaries.

For example:

```bash
ls -l /lib/ld*
```

and:

```bash
readelf -l <binary>
```

The ELF `INTERP` entry can identify the loader expected by an executable.

A binary requiring a loader that does not exist on the printer will not run normally.

Likewise, a binary can fail because its ABI, libraries, or libc version are incompatible even when the CPU architecture appears correct.

---

# Framebuffer Investigation

GuppyScreen communicates with the display through the Linux graphics system used by the target printer.

Do not assume that every printer uses:

```text
/dev/fb0
```

The correct framebuffer device must be determined from the target printer.

Useful commands include:

```bash
ls -l /dev/fb*
```

and:

```bash
cat /sys/class/graphics/fb0/name
```

For additional framebuffer devices:

```bash
cat /sys/class/graphics/fb1/name
```

Other useful information may include:

```bash
cat /sys/class/graphics/fb1/virtual_size
cat /sys/class/graphics/fb1/bits_per_pixel
cat /sys/class/graphics/fb1/stride
cat /sys/class/graphics/fb1/modes
cat /sys/class/graphics/fb1/state
```

The exact framebuffer number and available sysfs attributes depend on the printer.

---

# Visible Resolution vs. Virtual Resolution

A framebuffer's visible display resolution does not necessarily equal its virtual framebuffer size.

For example, a framebuffer may expose a visible mode such as:

```text
800x480
```

while having a virtual framebuffer height of:

```text
960
```

This can indicate that multiple framebuffer areas are available vertically.

This distinction is important when investigating:

* double buffering
* framebuffer panning
* page flipping
* display synchronization
* framebuffer memory layout

Therefore, both the visible mode and virtual framebuffer dimensions should be investigated.

---

# Framebuffer Panning

Some target systems use framebuffer panning to switch between framebuffer areas.

Linux framebuffer interfaces can expose information such as:

```text
xoffset
yoffset
xres
yres
xres_virtual
yres_virtual
```

Applications may use framebuffer ioctls such as:

```text
FBIOGET_VSCREENINFO
FBIOPAN_DISPLAY
```

when implementing this behavior.

The exact behavior must be confirmed on the target printer.

Do not assume that a virtual framebuffer automatically means that panning or double buffering works in the same way on another printer.

---

# Touchscreen Investigation

The touchscreen input device must also be determined from the target printer.

Do not assume that the touchscreen is always:

```text
/dev/input/event0
```

Investigate available input devices with:

```bash
ls -l /dev/input/
```

and, where available:

```bash
cat /proc/bus/input/devices
```

The correct event device should be identified from the target hardware.

Useful information can include:

* device name
* event number
* input capabilities
* absolute coordinates
* multitouch information
* resolution
* axis ranges

---

# Display Sleep and Wake

Display problems after idle, sleep, wake, or screen-off events must be investigated separately from normal rendering.

A display that works correctly immediately after startup may behave differently after:

* screen timeout
* display sleep
* system idle
* wake-up
* firmware UI transitions
* switching between applications
* restarting the display service

Do not automatically assume that a wake-up problem is an LVGL problem.

Investigate:

* framebuffer state
* framebuffer memory
* display service behavior
* display power state
* firmware services
* kernel messages
* stock GUI behavior

before changing GuppyScreen rendering code.

---

# Firmware Research

Firmware versions can change system behavior even when the printer hardware remains physically identical.

Firmware comparisons may include:

* kernel versions
* system binaries
* display services
* libraries
* configuration files
* startup scripts
* permissions
* device nodes
* framebuffer behavior
* touchscreen behavior
* power-management behavior

When comparing firmware versions, record the exact firmware version being tested.

Do not assume that behavior observed on one firmware version applies to another.

---

# Stock Creality Software

Stock Creality software can provide valuable information about how the printer's display system works.

Depending on the printer, relevant components may include:

* GUI applications
* display services
* framebuffer utilities
* touchscreen services
* startup services
* helper binaries
* libraries
* configuration files

Existing binaries can be inspected to determine how the stock system interacts with the hardware.

However, stock system files should **not** be modified blindly.

Always preserve original files and have a recovery procedure before testing changes on the printer.

---

# Safety and Recovery

Testing software directly on an embedded printer can cause:

* a frozen display
* an unusable GUI
* a failed application
* loss of touchscreen input
* unexpected system behavior

Therefore:

1. Keep backups of original files.
2. Test one change at a time where possible.
3. Keep a known-working recovery method.
4. Avoid overwriting stock binaries unnecessarily.
5. Prefer temporary test files when possible.
6. Record exactly what was changed.
7. Record the firmware version.
8. Record the result of every test.

Never assume that a binary is safe simply because it compiled successfully.

---

# Local Development

Whenever possible, development should first be performed on the development PC.

Useful development approaches include:

* compiling the project locally
* testing non-hardware-dependent code
* testing LVGL behavior
* checking build errors
* using a simulator where practical
* inspecting generated binaries
* running static analysis
* checking compiler warnings

Hardware-specific behavior must eventually be tested on the actual target printer.

A PC build that works correctly does not prove that the target binary will work on the printer.

---

# Build Environment

The project may require additional libraries and tools depending on the selected build configuration.

The exact toolchain should be documented when a target platform becomes reproducible.

Important build information includes:

* host operating system
* compiler
* compiler version
* linker
* binutils version
* target architecture
* target ABI
* C library
* library versions
* build flags
* linker flags
* firmware version

This information should be recorded when a working target build is discovered.

---

# Repository Structure

The repository contains the GuppyScreen source and supporting development material.

Typical project components include:

```text
GuppyScreen-Creality-Development/
├── README.md
├── DEVELOPMENT.md
├── Makefile
├── guppyscreen
├── lvgl/
├── lv_drivers/
├── libhv/
├── spdlog/
├── wpa_supplicant/
└── ...
```

The exact repository structure may change as development progresses.

Submodules should be initialized after cloning when required by the project.

For example:

```bash
git submodule update --init --recursive
```

---

# Known Reference Files and Binaries

The repository may contain binaries or other files retained from development and testing.

A binary stored in the repository should not automatically be considered a universal working binary for every supported printer.

A binary may depend on:

* a specific architecture
* ABI
* libc
* loader
* firmware version
* framebuffer configuration
* touchscreen configuration
* library versions

Reference binaries should therefore be treated as development artifacts unless their compatibility has been explicitly verified.

---

# Printer-Specific Development

Hardware-specific changes should remain isolated where practical.

For example, if one printer requires:

```text
/dev/fb1
```

while another uses:

```text
/dev/fb0
```

the project should not blindly change the global default for every printer.

Likewise, if one printer uses:

```text
/dev/input/event1
```

that does not mean every printer should use the same device.

Printer-specific configuration should eventually be documented clearly so that developers can reproduce the configuration without accidentally affecting other platforms.

---

# Adding Support for a New Printer

When investigating a new printer, collect as much information as possible before modifying the source.

A useful starting checklist is:

### Hardware

* Printer model
* Hardware revision
* SoC
* CPU architecture
* CPU instruction set
* Endianness
* RAM
* Storage
* Display
* Touchscreen

### Linux

* Kernel version
* `uname -m`
* `/proc/cpuinfo`
* device nodes
* framebuffer devices
* input devices

### Display

* framebuffer number
* visible resolution
* virtual resolution
* bits per pixel
* stride
* framebuffer mode
* framebuffer panning behavior

### Touch

* event device
* device name
* axis ranges
* input capabilities

### Executables

* `file`
* `readelf -h`
* `readelf -l`
* `readelf -d`
* dependencies
* interpreter
* libraries

### Firmware

* firmware version
* GUI binaries
* display services
* configuration files
* relevant system libraries

### Testing

* stock display behavior
* GuppyScreen startup behavior
* rendering
* touchscreen input
* sleep
* wake
* application switching
* stability

---

# Printer Development Records

Each printer should eventually have a documented development record.

A development record should include:

```text
Printer:
Hardware revision:
Firmware:
SoC:
CPU architecture:
Endianness:
ABI:
Kernel:
Framebuffer:
Touchscreen:
Toolchain:
Build configuration:
Known working binary:
Known issues:
Test results:
```

This makes it possible for other developers to reproduce the work.

---

# Current Research Platform

One of the current research platforms for this project is the **Creality K1C 2025/2026**.

The K1C research environment is being used to investigate several areas, including:

* Ingenic X2600 / XBurst II hardware
* target executable compatibility
* MIPS executable formats
* ABI compatibility
* dynamic loader compatibility
* framebuffer behavior
* virtual framebuffer behavior
* framebuffer panning
* touchscreen input
* display sleep and wake behavior
* stock display and GUI services
* firmware differences
* LVGL integration

The K1C is a current research platform, not the definition of the entire project.

Findings from the K1C should only be generalized to other printers when the relevant hardware and software characteristics have been verified.

---

# Other Creality Printers

The same research process can be used for other Creality printers.

Potential future development may include printers such as:

* K1
* K1 Max
* K1C
* K2
* K3
* other current or future Creality platforms

These names represent possible development targets and research platforms.

They do **not** automatically indicate that the printer is supported.

Support should only be claimed after the required hardware, software, and runtime behavior have been verified.

---

# Git Workflow

Changes should be committed in logical steps.

Before committing:

```bash
git status
```

Review changes with:

```bash
git diff
```

After committing:

```bash
git log -1 --oneline
```

Push changes with:

```bash
git push
```

Keep commits focused where possible.

For example:

```text
Add framebuffer investigation notes
```

is preferable to combining unrelated framebuffer, touchscreen, toolchain, and documentation changes into one unexplained commit.

---

# Contributions

Contributions are welcome.

Useful contributions include:

* hardware research
* firmware comparisons
* build fixes
* toolchain information
* framebuffer research
* touchscreen research
* compatibility testing
* documentation
* reproducible test results
* printer-specific configuration
* debugging information

When reporting a result, include enough information for another developer to understand how the result was obtained.

---

# Pull Requests

A useful pull request should explain:

* what was changed
* why it was changed
* which printer was tested
* firmware version
* hardware revision if known
* build environment
* toolchain
* test procedure
* test result
* known limitations

Avoid presenting an unverified printer-specific assumption as a universal fix.

---

# Problem Reports

When reporting a problem, include as much diagnostic information as possible.

Useful information may include:

```bash
uname -a
uname -m
cat /proc/cpuinfo
```

and:

```bash
ls -l /dev/fb*
ls -l /dev/input/
```

For executables:

```bash
file <binary>
readelf -h <binary>
readelf -l <binary>
```

Also include:

* printer model
* firmware version
* hardware revision if known
* exact binary tested
* exact command used
* console output
* whether the stock GUI still works
* whether the display freezes
* whether touchscreen input still works
* whether rebooting restores the original behavior

---

# Evidence First

When something does not work, avoid immediately changing multiple parts of the project.

For example, if GuppyScreen freezes on a printer:

Do not immediately assume:

```text
LVGL is broken
```

or:

```text
the framebuffer is wrong
```

or:

```text
the touchscreen is wrong
```

Instead, investigate each layer.

A useful debugging order is:

```text
1. Is the executable compatible?
2. Does the executable start?
3. Is the dynamic loader correct?
4. Are required libraries available?
5. Is the framebuffer correct?
6. Can the framebuffer be written?
7. Is the framebuffer layout correct?
8. Does framebuffer panning work?
9. Is touchscreen input available?
10. Does LVGL initialize correctly?
11. Does rendering work?
12. Does the system remain stable?
13. What happens after sleep/wake?
```

This makes debugging much easier to reproduce.

---

# Recommended Development Process

For a new printer:

```text
1. Identify the hardware
        ↓
2. Identify the firmware
        ↓
3. Inspect Linux
        ↓
4. Identify CPU architecture
        ↓
5. Identify ABI and endianness
        ↓
6. Inspect stock executables
        ↓
7. Identify the dynamic loader
        ↓
8. Identify framebuffer devices
        ↓
9. Identify touchscreen devices
        ↓
10. Build a minimal target test
        ↓
11. Verify the toolchain
        ↓
12. Build GuppyScreen
        ↓
13. Test on the printer
        ↓
14. Document the result
        ↓
15. Make printer-specific changes
        ↓
16. Repeat testing
```

This process is intentionally conservative.

It is much easier to debug one verified change than many simultaneous changes.

---

# What This Repository Does Not Assume

This repository does not assume that:

* every Creality printer uses the same CPU
* every Creality printer uses the same Linux environment
* every printer uses the same framebuffer
* every printer uses `/dev/fb0`
* every printer uses the same touchscreen event device
* every printer uses the same framebuffer resolution
* every printer uses the same virtual framebuffer layout
* every printer uses the same ABI
* every printer uses the same endianness
* every printer uses the same dynamic loader
* every printer uses the same libc
* every printer uses the same firmware behavior
* a binary built for one printer will run on another printer
* a working configuration on one printer is automatically correct for another printer

These assumptions must be verified.

---

# Security and Privacy

When collecting information from a printer, avoid publishing sensitive information unnecessarily.

Do not publish:

* passwords
* authentication tokens
* private keys
* Wi-Fi credentials
* personal information
* private network information
* unique identifiers unless necessary
* proprietary files that cannot legally be redistributed

When sharing diagnostic output, review it before publishing.

---

# Development Status

This repository is an active development and research project.

Hardware support should be considered experimental until it has been properly tested.

A successful build does not automatically mean that a printer is supported.

A successful startup does not automatically mean that:

* rendering is correct
* touchscreen input works
* framebuffer behavior is correct
* sleep/wake works
* the system is stable
* the configuration is suitable for production use

Support should be based on reproducible testing.

---

# Related Printer-Specific Work

This repository is intended to provide a common development and research environment.

Printer-specific projects or configurations may eventually be maintained separately when appropriate.

The relationship can be thought of as:

```text
GuppyScreen
      │
      ▼
GuppyScreen Creality Development
      │
      ├── Printer research
      ├── Shared development
      ├── Toolchain research
      ├── Hardware investigation
      └── Testing
             │
             ├── Printer-specific configuration
             ├── Printer-specific patches
             └── Printer-specific releases
```

The shared repository should contain reusable knowledge rather than assuming that every printer is identical.

---

# Final Rule

## Measure first. Change second. Document everything.

The most important principle of this project is simple:

**Do not guess when the printer can be inspected.**

If the hardware, firmware, executable format, framebuffer, touchscreen, or system behavior is unknown, investigate it first.

A measured result is more valuable than an assumption, and a documented result can help every developer working on another Creality printer.

---

## Credits

This project builds upon the work of the original GuppyScreen project and the broader open-source community.

Please respect the licenses of GuppyScreen, LVGL, its dependencies, and any other third-party software included or used by the project.

---

## License

See the repository and individual project components for their applicable license information.

Third-party components may have separate licenses and requirements.
