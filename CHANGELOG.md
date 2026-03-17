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
- `wwlib32_stub.hpp` — replacement stubs for constants and types originally
  defined in the proprietary Westwood wwlib32 library. Player color palette
  indices: YELLOW, RED, CYAN, LTBLUE, PURPLE, GREEN, BROWN.

---

## [03/17/2026] — Phase 1 (Ongoing): C++20 Language Modernization

### Fixed
- `COMPAT.H` — Removed `#include <i86.h>` (Watcom x86 port I/O intrinsics,
  no MSVC equivalent). Hardware access will route through Win32 API.
- `FUNCTION.H` — Commented out `#include "wwlib32.h"` (likely proprietary
  Westwood Studios engine library, not open-sourced). Missing symbols will be
  stubbed incrementally as they surface.
- `FACING.H` — Added `#include "DEFINES.H"` to make header self-contained.
  Previously relied on implicit include ordering via `CONQUER.H` — broken when
  included in isolation by the GTest harness. `DirType` is defined at line 2037
  of `DEFINES.H`.

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
  misleading — preserve names that are already clear.
- Headers must be self-contained. Every `.hpp` must compile cleanly in isolation.
- GTest coverage before refactoring. Tests are written before a module is
  modernized, not after.
- Top-down dependency order. Fix tier 1 headers (foundation) before tier 6
  (god headers). Each tier-1 fix cascades and clears multiple downstream errors.