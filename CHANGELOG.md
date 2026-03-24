# Changelog

All notable changes to this modernization project are documented here.
Format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## Goal

Modernize the EA GPL-released C&C Tiberian Dawn source (originally targeting
Watcom C++ 10.6 / DirectX 5 / Win32) to build cleanly on Windows 11 using
C++20 and DirectX 11, with a DirectX 12 upgrade path. The original master
binaries run fine on Windows 11 — this project targets the source.

---

## [03/16/2026] — Phase 0: Project Scaffolding

### Added
- `CMakeLists.txt` — CMake 3.28+ build system replacing Watcom `.IDE` / `.MAK`
  project files. Targets Ninja generator with MSVC toolchain.
- `CMakePresets.json` — `windows-msvc-debug` and `windows-msvc-release` presets
  for reproducible builds. Ninja generator selected over Visual Studio generator
  due to Build Tools registry detection issue with internal version 18.
- `tests/CMakeLists.txt` — GTest harness via `FetchContent` (pinned v1.15.2).
  `gtest_discover_tests` registers each `TEST()` individually with CTest.
- `tests/test_facing.cpp` — Smoke test for `FacingClass` / `DirType`. Confirms
  GTest wiring and header self-containment end-to-end.
- `docs/header_dependency_map.html` — Interactive header dependency map (6 tiers)
  generated from static analysis of all `.H` files. Documents implicit include
  dependencies and known broken includes.
- `.gitignore` — Excludes `build/`, `.vscode/`, `*.user`.

### TBD - Removed
- `DPMI.CPP` — Watcom DPMI / DOS protected-mode stub. Dead code under Win32.
- `COORDA.ASM` — Watcom assembler coordinate helper. No MSVC assembler target.
- `CWSTUB.C` — Watcom C stub. No equivalent needed under MSVC.

### Added
- `wwlib32_stub.hpp`
  - replacement stubs for constants and types originally
    defined in the proprietary Westwood wwlib32 library. Player color palette
    indices: YELLOW, RED, CYAN, LTBLUE, PURPLE, GREEN, BROWN.

---

## [03/17/2026] - Phase 1.1: Compilation Unblocking with Partial modernization (Ongoing)

### Goal

- Zero errors under MSVC/Ninja. No major functional rewrites - stubs, guards and partial rewrites only.
  - In this phase, rewrite MUST NEVER change:
    - Function signature(s)
    - Function input parameter(s)
      (Renaming variables would be okay, provided that it gives clarity to the codebase)
    - Function output

### Fixed
- `CCFILE.H`
  - Commented out `#include <wwlib32.h>`; replaced with `wwlib32_stub.hpp`
  - Replaced the include guard with `#pragma once` for correctness and code hygiene
    Although `#pragma once` is technically non-standard as of the moment (March, 2026)
    the ISO C++ committee is considering to add this in the next C++ improvement
    FYI, MSVC, GCC and Clang support it.
- `COMPAT.H`
  - Removed `#include <i86.h>` (Watcom x86 port I/O intrinsics,
    no MSVC equivalent). Hardware access will route through Win32 API.
  - Replaced the include guard with `#pragma once` for correctness and code hygiene
- `DDE.H` / `CCDDE.H`
  - Stubbed entirely — Westwood Chat lobby service is defunct.
    Modern session coordination is out of scope (Discord etc.).
  - `#include "ccdde.h"` commented out at all call sites:
    `CONQUER.CPP`, `EVENT.CPP`, `INIT.CPP`, `INTERNET.CPP`,
    `MENUS.CPP`, `NETDLG.CPP`, `STARTUP.CPP`, `STATS.CPP`
  - `DDE_STUBBED` marker defined in `network_stub.hpp`
- `DEFINES.H`
  - Documented implicit COORDINATE packing convention at typedef (line 1597).
    TODO: Phase 1.2 — replace with strongly typed struct.
  - Replaced the include guard with `#pragma once` for correctness and code hygiene
- `EVENT.H`
  - Replaced the include guard with `#pragma once` for correctness and code hygiene
- `FACING.H`
  - Added `#include "DEFINES.H"` to make header self-contained.
    Previously relied on implicit include ordering via `CONQUER.H`
  - broken when included in isolation by the GTest harness. `DirType` is defined
    at line 2037 of `DEFINES.H`.
  - Replaced the include guard with `#pragma once` for correctness and code hygiene
- `FUNCTION.H`
  - Commented out `#include "wwlib32.h"` (likely proprietary
    Westwood Studios engine library, not open-sourced).
    Missing symbols will be stubbed incrementally as they surface.
  - Commented out
    - `#include <vqa32\vqafile.h>`
    - `#include <vqa32\vqaplay.h>`
    - `#include <dos.h>`
    - `#include <fast.h>`
    - `#include "memcheck.h"`
    - `#include <modem.h>`
    - `#include <new.h>`
  - Commented out Watcom `bool` compatibility shim since boolean becomes now a native keyword
  - Commented out legacy functions `Interpolate_2X_Scale` and `ModeX_Blit`
  - Removed the obsolete `register` keyword from the header file
  - Introduced the `#ifndef` guard for `WIN32` and `_WIN32`
  - Removed manual `WIN32` / `_WIN32` defines; redundant with
    CMake command-line `/DWIN32` and `<windows.h>` internal definitions.
    Previously caused C4005 redefinition warnings.
  - `Distance_Coord` — commeneted out `#pragma aux` Watcom assembly and
    inserted C++20 equivalent.
    Octagonal distance approximation `max(|dx|,|dy|) + min(|dx|,|dy|)/2`
    COORDINATE packing documented: low 16 bits = X, high 16 bits = Y.
- `JSHELL.H`
  - Replaced the include guard with `#pragma once` for correctness and code hygiene
  - Added forward declaration of 'ShapeFlages_Type' to resolve operator instantiation error
    - [] TODO: Phase 1.1 - Convert to proper scoped enum class.
    - [] TODO: Phase 1.1 - Above these lines, constrain template bitwise operators with require, etc.
  - Replaced all `#pragma aux` assembly
        implementations with C++20 equivalents. Function signatures unchanged.
    - `First_True_Bit` → `std::countr_zero` (`<bit>`)
    - `First_False_Bit` → `std::countr_one` (`<bit>`)
    - `Bound` → `std::clamp` (`<algorithm>`)
    - `Fixed_To_Cardinal` / `Cardinal_To_Fixed` → explicit 64-bit arithmetic
    - `Set_Bit` / `Get_Bit` → idiomatic C++ bit mask operations
    - [] TODO: Phase 1.1 - Verify 1:1 correspondance between the modern functions and ASM blocks
        above; if all checks out, delete the commented-out ASM blocks.
  - Replaced the Keyboard class definitions to suprress the errors for the Phase 1.1
    - [] TODO: Phase 1.2 - replace with Win32 GetAsyncKeyState, etc.
- `MIXFILE.H`
  - Commented out `#include <wwlib32.h>`; replaced with `wwlib32_stub.hpp`
  - Replaced the include guard with `#pragma once` for correctness and code hygiene
- `NULLCONN.H` / `NULLMGR.H`
  - Commented out `#include <commlib.h>`; replaced with `network_stub.hpp`
    Greenleaf Communications Library (GCL), proprietary, not open-sourced.
    - Dial-up/serial multiplayer subsystem excluded from modernization scope.
      LAN multiplayer (IPX stack) is the target instead.
  - Replaced the include guard with `#pragma once` for correctness and code hygiene
- `RAWFILE.H`
  - Commented out `#include <wwlib32.h>`; replaced with `wwlib32_stub.hpp`
  - Replaced the include guard with `#pragma once` for correctness and code hygiene
- `wwlib32_stub.hpp`
  - Added the following functions, structs and classes placeholders
    - `VQAHandle`
    - `BufferClass`
    - `GraphicViewPortClass` and its derived `GraphicBufferClass`
    - `Desired_Facing256`
    - `Desired_Facing8`
    - `Extract_String`
  - Added the work packing macros, presumably from wwlib32 library
    - `MAKE_LONG`
    - `LOW_WORD`
      - This macro is converted into constexpr for the type safety over macro text substitution
        Name retained for call site compatibility
    - `HIGH_WORD`
      - This macro is converted into constexpr for the type safety over macro text substitution
        Name retained for call site compatibility

### Added
- Added `network_stub.hpp` — Placeholder for any GCL, Westwood Chat-related
  type references that surface in non-networking code.

## [TBA] — Phase 1.2: C++20 Language Modernization

### Fixed

### Added

---

## Background

### Why this project exists

The EA GPL release compiles against a toolchain (Watcom C++ 10.6, DirectX 5 SDK,
Greenleaf Communications Library, HMI SOS audio) that is unavailable on modern
systems. The three immediate blockers for any Windows 11 build are:

1. **DirectX 5** — `IDirectDraw` / `IDirectDrawSurface` surface model removed
   from modern Windows SDK. Replacement: DirectX 11 renderer behind `IRenderer`
   abstraction, with DirectX 12 upgrade path.
2. **Watcom-specific headers** — `i86.h`, `watcom.h`, and related toolchain
   headers have no MSVC equivalent.
3. **Proprietary libraries** — `wwlib32` (Westwood engine), GCL (networking),
   HMI SOS (audio) were never open-sourced. Functionality will be replaced
   with Win32 / modern equivalents incrementally.

### Modernization principles

- C++20 throughout. No C-style casts, raw owning pointers, or `#define` constants.
- Self-documenting names. Rename only where the original name is opaque or
  misleading — preserve names that are already clear. (CODE SHOULD BE READ LIKE A BOOK)
- Headers must be self-contained. Every `.hpp` must compile cleanly in isolation.
- GTest coverage before refactoring. Tests are written before a module is
  modernized, not after.
- Top-down dependency order. Fix tier 1 headers (foundation) before tier 6
  (god headers). Each tier-1 fix cascades and clears multiple downstream errors.