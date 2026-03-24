#pragma once

// wwlib32_stub.hpp
// Replacement stubs for constants and types originally defined in the
// likely proprietary Westwood Studios wwlib32 library (not open-sourced).
// These palette color index constants were used for player color assignment.
// Values match the original Westwood 8-bit palette layout.

inline constexpr int YELLOW = 5;
inline constexpr int RED    = 3;
inline constexpr int CYAN   = 14;
inline constexpr int LTBLUE = 9;
inline constexpr int PURPLE = 6;
inline constexpr int GREEN  = 4;
inline constexpr int BROWN  = 11;
inline constexpr int TBLACK = 0;    // Transparent black palette

// VQAHandle — Westwood proprietary video format (VQA).
// vqa32 library not open-sourced. FMV playback stubbed.
// TODO: Phase 2 — replace with modern video playback if needed.
struct VQAHandle {};

// Westwood graphics classes — originally from wwlib32.
// These are the DirectX 5 surface abstraction layer.
// TODO: Phase 2 — replace with IRenderer / DX11 equivalents.
class BufferClass {};
class GraphicViewPortClass {};
class GraphicBufferClass : public GraphicViewPortClass {};

// Word packing macros — originally from wwlib32.
// TODO: Phase 1.2 — replace call sites with std::bit_cast or direct arithmetic.
#ifndef MAKE_LONG
#define MAKE_LONG(lo, hi) \
    (static_cast<long>((static_cast<unsigned short>(lo)) | \
    (static_cast<unsigned long>(static_cast<unsigned short>(hi))) << 16))
#endif

[[nodiscard]] constexpr unsigned short LOW_WORD(unsigned long value) noexcept {
    return static_cast<unsigned short>(value & 0xFFFFu);
}

[[nodiscard]] constexpr unsigned short HIGH_WORD(unsigned long value) noexcept {
    return static_cast<unsigned short>((value >> 16) & 0xFFFFu);
}

// Facing calculation — originally from wwlib32.
// TODO: Phase 1.2 — reimplement with std::atan2 based calculation.
inline int Desired_Facing256(int x1, int y1, int x2, int y2) { return 0; }
inline int Desired_Facing8(int x1, int y1, int x2, int y2)   { return 0; }

// String table extraction — originally from wwlib32.
// TODO: Phase 1.2 — replace with std::string_view based lookup.
inline char const* Extract_String(void const*, int) { return ""; }