# GuppyScreen Creality Development

This repository is a **community development and research repository for GuppyScreen on newer Creality printers**.

The main purpose of this repository is to make it possible for developers with different Creality printers to work together on:

* GuppyScreen development
* Creality hardware research
* MIPS / XBurst / X2600 platform research
* LVGL development
* Display and framebuffer support
* Touchscreen support
* Firmware research
* Sleep/wake behavior
* Platform-specific adaptations
* Cross-compilation
* Testing and debugging

This is **not a printer-specific repository**.

The Creality K1C 2025/2026 is one of the current development and test platforms, but the goal is to support research and development for other compatible Creality printers as well.

---

# 1. Important: Development Is Done on Your PC

The repository is cloned and developed on your **development computer**.

Do **not** clone this repository directly onto the printer for normal development.

The printer is the **target/test device**.

The normal workflow is:

```text
PC
 │
 ├── Clone repository
 ├── Modify source code
 ├── Build
 ├── Analyze binaries
 └── Prepare target build
        │
        ▼
     Printer
        │
        ├── Run test build
        ├── Collect hardware information
        ├── Collect logs
        └── Test display/touch/etc.
        │
        ▼
     PC
        │
        └── Analyze results
```

This means that you can contribute even if you do not own the same printer as another developer.

---

# 2. Supported Development Operating Systems

Development can be performed from:

* Linux
* Windows
* macOS

The printer itself is normally a Linux-based embedded system, but the development computer does not need to be Linux.

For the most predictable embedded development environment, **Linux or Windows with WSL2 is recommended**.

---

# 3. Linux Development

Linux is the recommended native development environment.

Supported distributions can include:

* Ubuntu
* Debian
* Linux Mint
* Fedora
* Arch Linux
* Other modern Linux distributions

For Ubuntu/Debian:

```bash
sudo apt update
sudo apt install -y \
    git \
    build-essential \
    cmake \
    pkg-config
```

For local SDL development:

```bash
sudo apt install -y libsdl2-dev
```

Useful additional development tools:

```bash
sudo apt install -y \
    binutils \
    file \
    gdb \
    make \
    pkg-config \
    xxd
```

Not every tool is required for a normal GuppyScreen build.

Some tools are only required for firmware and binary research.

---

# 4. Windows Development

Windows is supported as a development host.

The recommended method is:

**Windows + WSL2 + Ubuntu**

WSL2 provides a Linux development environment without requiring a separate Linux computer.

After installing WSL2 and Ubuntu, open the Ubuntu terminal and follow the Linux instructions in this document.

Install the required packages:

```bash
sudo apt update
sudo apt install -y \
    git \
    build-essential \
    cmake \
    pkg-config
```

For local SDL development:

```bash
sudo apt install -y libsdl2-dev
```

The repository should preferably be cloned inside the WSL Linux filesystem rather than under a Windows-mounted directory when doing larger builds.

Example:

```bash
cd ~
git clone --recursive https://github.com/SnorritxD/GuppyScreen-Creality-Development.git
cd GuppyScreen-Creality-Development
```

---

# 5. macOS Development

macOS can be used for source-code development and local testing.

Install Apple's command-line developer tools:

```bash
xcode-select --install
```

Install Homebrew if it is not already installed.

Then install the basic development dependencies:

```bash
brew install git cmake pkg-config
```

For SDL-based local development:

```bash
brew install sdl2
```

The exact dependencies may change as the project develops.

The important distinction is that macOS is the **development host**.

A binary compiled for macOS cannot normally be copied to a Creality printer and expected to run there.

The printer requires a separate target build for its own CPU architecture and ABI.

---

# 6. Clone the Repository

Clone the repository on your **PC**.

Use:

```bash
git clone --recursive https://github.com/SnorritxD/GuppyScreen-Creality-Development.git
```

Then:

```bash
cd GuppyScreen-Creality-Development
```

If the repository was already cloned without submodules:

```bash
git submodule update --init --recursive
```

Verify the repository:

```bash
git status
```

You should see the repository state without unexpected changes.

---

# 7. Git Submodules

This repository contains external dependencies as Git submodules.

Important dependencies include:

* LVGL
* LVGL drivers
* libhv
* spdlog
* wpa_supplicant

Always initialize the submodules before building:

```bash
git submodule update --init --recursive
```

When switching branches, also verify the submodule state:

```bash
git submodule status
```

Do not manually replace a submodule with a random version unless the change is intentional and documented.

---

# 8. Current LVGL Development Version

The current development baseline is **LVGL 8.3**.

This is important.

The GuppyScreen source in this development repository is based on the LVGL 8.x API.

Do not change the repository to LVGL 9 simply because a newer LVGL version exists.

A migration from LVGL 8 to LVGL 9 is a separate development project and requires changes throughout the application and drivers.

Before making LVGL changes, check the actual submodule revision:

```bash
cd lvgl
git log -1
cd ..
```

Also check:

```bash
git submodule status
```

---

# 9. Repository Structure

The repository contains the GuppyScreen application and its development dependencies.

Important directories include:

```text
.
├── assets/
├── build/
├── libhv/
├── lv_drivers/
├── lv_touch_calibration/
├── lvgl/
├── patches/
├── spdlog/
├── src/
├── themes/
├── wpa_supplicant/
├── Makefile
├── VERSION
├── lv_conf.h
├── lv_drv_conf.h
└── DEVELOPMENT.md
```

The main areas are:

| Directory               | Purpose                    |
| ----------------------- | -------------------------- |
| `src/`                  | GuppyScreen application    |
| `lvgl/`                 | LVGL graphics library      |
| `lv_drivers/`           | LVGL display/input drivers |
| `lv_touch_calibration/` | Touchscreen calibration    |
| `libhv/`                | Networking library         |
| `spdlog/`               | Logging                    |
| `wpa_supplicant/`       | Wireless/network support   |
| `themes/`               | UI themes                  |
| `assets/`               | UI assets                  |
| `patches/`              | Development patches        |
| `build/`                | Build output               |

The structure may change as development progresses.

---

# 10. First Build: Test the PC Build

Before working on a printer-specific target build, first make sure the source tree can build on your development computer.

Clean previous build output:

```bash
make clean
```

Then build:

```bash
make -j$(nproc)
```

If `nproc` is unavailable:

```bash
make
```

A successful PC build only proves that the source can be compiled for the PC target.

It does **not** prove that the executable will run on a Creality printer.

---

# 11. PC Build vs Printer Build

There are two fundamentally different development targets.

## PC build

Used for:

* UI development
* LVGL development
* application development
* debugging
* simulator development
* testing logic

The executable is compiled for the development computer.

Examples:

```text
x86_64 Linux
x86_64 Windows/WSL
ARM64 macOS
```

depending on the host.

## Printer build

Used for:

* real display testing
* touchscreen testing
* framebuffer testing
* printer hardware testing

The executable must be compiled for the exact architecture and ABI required by the printer.

These two builds must not be confused.

---

# 12. Local Development

Whenever possible, test application and UI changes on the development computer first.

This is much faster than repeatedly copying binaries to a printer.

Use local development for:

* UI changes
* screen layout
* themes
* application logic
* networking code
* configuration changes
* LVGL experiments

Hardware-specific features cannot always be tested locally.

Those features require the real printer.

---

# 13. Printer Hardware Research

Before adapting GuppyScreen to a new Creality printer, collect information from that printer.

At minimum:

```bash
uname -a
uname -m
cat /proc/cpuinfo
```

Also inspect available framebuffer devices:

```bash
ls -l /dev/fb*
```

And input devices:

```bash
ls -l /dev/input/
```

For Linux input information:

```bash
cat /proc/bus/input/devices
```

Record the printer model and firmware version together with this information.

---

# 14. Determine the CPU Architecture

Do not select a compiler based only on the word "MIPS".

MIPS systems can differ in:

* endianness
* ABI
* instruction set
* floating-point ABI
* C library
* dynamic loader

For example, two MIPS printers can require different toolchains even though both report:

```text
mips
```

on:

```bash
uname -m
```

Therefore the target architecture must be investigated before selecting a cross-compiler.

---

# 15. Inspect Existing Printer Binaries

If the printer contains an existing executable, copy a suitable binary to the development PC.

Then inspect it:

```bash
file <binary>
```

Use:

```bash
readelf -h <binary>
```

And:

```bash
readelf -l <binary>
```

This can reveal:

* ELF class
* endianness
* architecture
* ABI
* program interpreter
* other ELF properties

Also useful:

```bash
strings <binary>
```

and:

```bash
objdump -f <binary>
```

Do this before selecting a target compiler.

---

# 16. Dynamic Loader

The target dynamic loader is important.

On the printer:

```bash
ls -l /lib/ld*
```

If an existing executable is available, inspect it on the PC:

```bash
readelf -l <binary>
```

Look for:

```text
Requesting program interpreter
```

The interpreter must be compatible with the target printer.

For example, a binary requiring one MIPS loader cannot automatically be assumed to work on another MIPS system with a different loader or ABI.

Do not replace system libraries or the dynamic loader just to make a test binary start.

---

# 17. Cross Compilation

Cross compilation is platform-specific.

Do not blindly copy the old GuppyScreen K1 toolchain instructions into this repository.

The original GuppyScreen development documentation contains instructions for the older K1/K1 Max/X2000E environment, including `mips-gcc720` and `mips-linux-gnu-`. Those instructions are not automatically valid for newer Creality platforms.

Before defining:

```bash
export CROSS_COMPILE=...
```

confirm:

1. CPU architecture
2. Endianness
3. ABI
4. C library
5. Dynamic loader
6. Required instruction set
7. Compiler version
8. Linker/binutils compatibility

Only then select the toolchain.

---

# 18. XBurst / X2600 Development

One of the main research targets of this repository is newer Creality hardware using Ingenic XBurst/XBurst II processors, including the X2600 platform.

For example:

```text
Ingenic X2600
XBurst II
MIPS
```

However, the presence of an X2600 does not by itself define the complete compiler configuration.

The target's complete executable environment must still be confirmed.

Therefore this repository intentionally does **not** define one universal X2600 toolchain until it has been verified.

When a working toolchain is confirmed for a specific printer, document:

```text
Printer:
SoC:
Architecture:
Endianness:
ABI:
C library:
Dynamic loader:
Compiler:
Compiler version:
Binutils version:
Toolchain source:
```

---

# 19. Toolchain Verification

Before compiling GuppyScreen, test the toolchain with a small program.

For example, create a simple C program:

```c
#include <stdio.h>

int main(void)
{
    printf("toolchain test\n");
    return 0;
}
```

Compile it using the candidate cross-compiler.

Then inspect the result:

```bash
file ./test
```

and:

```bash
readelf -h ./test
```

and:

```bash
readelf -l ./test
```

Do not copy the test executable to the printer until the architecture, ABI, and loader have been verified.

---

# 20. Framebuffer Research

Display support is one of the most important hardware-specific parts of GuppyScreen.

Do not assume that every Creality printer uses:

```text
/dev/fb0
```

Another printer may use:

```text
/dev/fb1
```

or another framebuffer configuration.

Check:

```bash
ls -l /dev/fb*
```

Then inspect:

```bash
for fb in /sys/class/graphics/fb*; do
    echo "===== $fb ====="

    for f in name virtual_size bits_per_pixel stride modes state; do
        if [ -f "$fb/$f" ]; then
            echo "$f: $(cat "$fb/$f")"
        fi
    done
done
```

Record the results.

---

# 21. Framebuffer Resolution

A framebuffer may have a visible resolution that differs from its virtual resolution.

For example:

```text
visible:
800x480

virtual:
800x960
```

A virtual height larger than the visible height can indicate multiple framebuffer pages or another framebuffer memory arrangement.

Do not automatically remove or change this behavior.

Inspect:

```text
xres
yres
xres_virtual
yres_virtual
xoffset
yoffset
bits_per_pixel
stride
```

before modifying the framebuffer driver.

---

# 22. Framebuffer Panning

The LVGL framebuffer driver can use framebuffer offsets and framebuffer panning.

Relevant Linux framebuffer operations include:

```text
FBIOGET_VSCREENINFO
FBIOPUT_VSCREENINFO
FBIOPAN_DISPLAY
```

If a printer uses a virtual framebuffer larger than the visible display, investigate how the stock Creality software handles it before changing GuppyScreen.

Document:

```text
Framebuffer device:
Visible resolution:
Virtual resolution:
Bits per pixel:
Stride:
X offset:
Y offset:
Panning behavior:
```

---

# 23. Touchscreen Research

Do not assume that:

```text
/dev/input/event0
```

is the touchscreen.

Another printer may use:

```text
/dev/input/event1
```

or another input device.

Check:

```bash
ls -l /dev/input/
```

Then:

```bash
cat /proc/bus/input/devices
```

If available, `evtest` can also be useful:

```bash
evtest
```

Identify the touchscreen by its device information and event capabilities.

Document the confirmed touchscreen device for each printer.

---

# 24. Display Sleep and Wake

Sleep/wake behavior must be treated as a separate hardware and firmware feature.

A display can work normally and still fail after:

```text
Display ON
    ↓
Sleep
    ↓
Display power management
    ↓
Wake
    ↓
Framebuffer/display reinitialization
```

Firmware updates can change this behavior.

When investigating a sleep/wake problem, collect information:

```text
Firmware version:
Framebuffer before sleep:
Framebuffer after wake:
Display state before sleep:
Display state after wake:
Touchscreen state after wake:
GuppyScreen logs:
Kernel messages:
```

Useful command:

```bash
dmesg | tail -n 100
```

Do not assume a sleep/wake failure is automatically caused by LVGL or GuppyScreen.

First determine whether the underlying framebuffer or display state changed.

---

# 25. Firmware Research

Firmware versions can change:

* system services
* framebuffer behavior
* display power management
* touchscreen handling
* startup procedures
* libraries
* binaries
* permissions
* system configuration

When comparing firmware versions, record:

```text
Printer:
Firmware version:
Previous firmware:
New firmware:
Relevant files:
Relevant binaries:
Observed behavior:
```

Useful PC tools include:

```bash
file
strings
readelf
objdump
nm
grep
diff
cmp
sha256sum
```

For file hashes:

```bash
sha256sum <file>
```

For binary strings:

```bash
strings <binary>
```

For ELF information:

```bash
readelf -h <binary>
readelf -l <binary>
```

---

# 26. Stock Creality Software

Newer Creality printers can contain multiple display-related components.

Examples may include:

```text
display services
GUI applications
framebuffer utilities
touchscreen services
rendering components
startup scripts
power-management components
```

Do not assume that a component needs to be replaced just because it is related to the display.

First determine:

```text
What starts the component?
What device does it access?
What libraries does it use?
What happens if it stops?
What happens during sleep?
What happens during wake?
```

Use evidence from the printer before modifying stock software.

---

# 27. Testing GuppyScreen on a Printer

Target testing should be performed carefully.

Prefer placing test builds in a dedicated development directory.

For example:

```text
/usr/data/guppyscreen/
```

if that location is appropriate for the target printer.

Before testing:

```text
1. Confirm architecture.
2. Confirm endianness.
3. Confirm ABI.
4. Confirm dynamic loader.
5. Confirm required libraries.
6. Confirm framebuffer.
7. Confirm touchscreen.
8. Copy test executable.
9. Run it manually.
10. Capture logs.
```

Do not immediately replace the stock Creality GUI or startup system.

---

# 28. Never Blindly Modify the Stock System

The goal of development is to understand the target platform first.

Avoid blindly changing:

```text
/etc/init.d/*
/usr/bin/*
/usr/apps/*
/lib/*
```

or other stock system components.

If a modification becomes necessary, document:

```text
Original file:
Original hash:
Original behavior:
Change:
Reason:
Result:
Recovery method:
```

Always keep a way to restore the original system.

---

# 29. Build Output

Build output may appear in the repository's build directories depending on the current Makefile configuration.

After building, locate the executable with:

```bash
find build -type f -name 'guppyscreen' -o -name 'guppyscreen*'
```

Before copying a target binary to a printer:

```bash
file <path-to-guppyscreen>
```

and:

```bash
readelf -h <path-to-guppyscreen>
```

For dynamically linked binaries:

```bash
readelf -l <path-to-guppyscreen>
```

---

# 30. Debugging a Target Binary

If a target binary does not start, do not immediately rebuild everything.

First collect:

```text
file <binary>
readelf -h <binary>
readelf -l <binary>
```

Then check the target:

```bash
uname -a
uname -m
cat /proc/cpuinfo
ls -l /lib/ld*
```

Check the error printed by the printer.

A message such as:

```text
syntax error: unexpected "("
```

can indicate that the shell is attempting to interpret the file instead of the kernel executing it as a compatible binary.

This can happen when the executable format is incompatible with the target environment.

Always verify the ELF format before debugging application code.

---

# 31. Git Development Workflow

Before making changes:

```bash
git status
```

Create a development branch:

```bash
git switch -c <branch-name>
```

Example:

```bash
git switch -c x2600-display-research
```

After making changes:

```bash
git status
git diff
```

Review the changes before committing.

Then:

```bash
git add .
git commit -m "Describe the change"
```

Keep unrelated changes in separate commits.

---

# 32. Working With Hardware-Specific Changes

If a change only applies to one printer, do not automatically make it global.

For example, do not assume every printer should use:

```text
/dev/fb1
```

and do not assume every printer should use:

```text
/dev/input/event1
```

Instead, document the hardware difference.

The long-term goal is:

```text
Common GuppyScreen code
        +
Platform-specific configuration
        +
Printer-specific hardware information
```

rather than:

```text
One printer's hardware configuration
        ↓
Global hard-coded assumption
```

---

# 33. Adding Support for a New Printer

When investigating a new printer, collect at least:

```text
Printer model:
Firmware version:
SoC:
CPU architecture:
Endianness:
ABI:
Kernel:
C library:
Dynamic loader:
Framebuffer:
Display resolution:
Virtual framebuffer resolution:
Bits per pixel:
Stride:
Touchscreen device:
Input event:
Known limitations:
```

Useful commands:

```bash
uname -a
uname -m
cat /proc/cpuinfo
ls -l /dev/fb*
ls -l /dev/input/
cat /proc/bus/input/devices
ls -l /lib/ld*
```

Framebuffer information:

```bash
for fb in /sys/class/graphics/fb*; do
    echo "===== $fb ====="

    for f in name virtual_size bits_per_pixel stride modes state; do
        if [ -f "$fb/$f" ]; then
            echo "$f: $(cat "$fb/$f")"
        fi
    done
done
```

Only document values that have actually been confirmed.

---

# 34. Printer Development Record

When adding research for a new printer, use a record similar to:

```text
Printer:
Firmware:
SoC:
Architecture:
Endianness:
ABI:
Kernel:
C library:
Dynamic loader:

Display:
Framebuffer:
Resolution:
Virtual resolution:
Bits per pixel:
Stride:

Touch:
Input device:

Toolchain:
Compiler:
Compiler version:
Binutils:
C library/toolchain:

Status:
Known issues:
Sleep/wake:
Notes:
```

This makes research from different developers comparable.

---

# 35. Pull Requests

Contributions are welcome.

When submitting a pull request, explain:

1. What was changed?
2. Why was it changed?
3. Which printer was tested?
4. Which firmware was tested?
5. Was the change tested on real hardware?
6. What was the result?
7. Are there known limitations?
8. Does the change affect other printers?

For hardware-specific changes, include the relevant hardware information.

---

# 36. Evidence First

This repository follows an **evidence-first development approach**.

Before changing platform-specific code:

```text
Collect information
        ↓
Compare with known platforms
        ↓
Form a hypothesis
        ↓
Test the hypothesis
        ↓
Document the result
        ↓
Implement the change
```

Avoid:

```text
It works on printer A
        ↓
It must work on printer B
        ↓
Hard-code the same configuration
```

Creality hardware and firmware can differ significantly between models and firmware versions.

---

# 37. Recommended Development Process

The recommended workflow is:

```text
1. Clone repository on PC
        ↓
2. Initialize submodules
        ↓
3. Build PC version
        ↓
4. Test local changes
        ↓
5. Identify target printer
        ↓
6. Collect printer hardware information
        ↓
7. Identify CPU architecture
        ↓
8. Identify endianness
        ↓
9. Identify ABI
        ↓
10. Identify C library and loader
        ↓
11. Select/verify toolchain
        ↓
12. Build target executable
        ↓
13. Inspect executable
        ↓
14. Test manually on printer
        ↓
15. Collect logs/results
        ↓
16. Compare with expected behavior
        ↓
17. Document findings
        ↓
18. Commit changes
        ↓
19. Submit pull request
```

---

# 38. Development Environment Summary

## Linux

Recommended.

```bash
sudo apt update
sudo apt install -y git build-essential cmake pkg-config
```

Clone:

```bash
git clone --recursive https://github.com/SnorritxD/GuppyScreen-Creality-Development.git
cd GuppyScreen-Creality-Development
```

Build:

```bash
make clean
make -j$(nproc)
```

---

## Windows

Recommended:

```text
Windows
   ↓
WSL2
   ↓
Ubuntu
   ↓
Linux development environment
```

Then follow the Linux instructions.

Clone inside WSL:

```bash
git clone --recursive https://github.com/SnorritxD/GuppyScreen-Creality-Development.git
cd GuppyScreen-Creality-Development
```

---

## macOS

Install:

```bash
xcode-select --install
```

Then:

```bash
brew install git cmake pkg-config
```

For SDL:

```bash
brew install sdl2
```

Clone:

```bash
git clone --recursive https://github.com/SnorritxD/GuppyScreen-Creality-Development.git
cd GuppyScreen-Creality-Development
```

---

# 39. Important Distinction: PC vs Printer

Always remember:

```text
DEVELOPMENT PC
===============
Source code
Git
Compiler
Cross compiler
Docker
Firmware analysis
Build system
Testing
        │
        │
        ▼
PRINTER
=======
Target executable
Framebuffer
Touchscreen
Display
Hardware
Firmware
Logs
```

The development repository belongs on the **PC**.

The resulting executable is what is transferred to the printer for hardware testing.

---

# 40. Security and Privacy

Never commit:

* Wi-Fi passwords
* API keys
* SSH private keys
* authentication tokens
* personal credentials
* private configuration files
* private network information
* other sensitive information

Before committing:

```bash
git diff
```

Check that no credentials or private information are included.

---

# 41. Current Research Platform

A major current research platform for this repository is the newer Creality generation using the Ingenic X2600/XBurst II platform.

The Creality K1C 2025/2026 is one of the development/test platforms.

Current research includes:

* MIPS executable compatibility
* target ABI
* dynamic loader compatibility
* framebuffer behavior
* dual/virtual framebuffer behavior
* touchscreen input
* display sleep/wake behavior
* Creality GUI/display services
* firmware differences
* LVGL integration

These areas are still under investigation.

Do not treat experimental findings as universal platform requirements until they have been confirmed.

---

# 42. Development Status

This repository is an active community research and development project.

Support for a specific printer may be:

* experimental
* partially working
* hardware-specific
* firmware-specific
* untested

A printer should only be considered supported when the relevant hardware and software behavior has been tested and documented.

---

# 43. Final Rule

## Measure first. Change second. Document everything.

The goal of this repository is not only to make GuppyScreen work on one Creality printer.

The goal is to create a development base where developers with different Creality printers can:

* investigate their hardware
* share findings
* build GuppyScreen
* test changes
* compare firmware
* develop platform support
* contribute improvements
* reproduce each other's work

If you have a different Creality printer, **please contribute its hardware and firmware information**.

Every confirmed platform difference helps make GuppyScreen easier to support on the next printer.
