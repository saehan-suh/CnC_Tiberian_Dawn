#pragma once

// Placeholder for networking subsystems excluded from initial modernization.

// DDE (Dynamic Data Exchange) — used for Westwood Chat lobby matchmaking.
// Westwood Chat is defunct. Modern alternatives (Discord etc.) are out of scope.
// DDE call sites in CONQUER.CPP, EVENT.CPP, INIT.CPP, INTERNET.CPP,
// MENUS.CPP, NETDLG.CPP, STARTUP.CPP, STATS.CPP are stubbed pending
// future multiplayer reimplementation.
#define DDE_STUBBED 1

// GCL (Greenleaf Communications Library) — dial-up/serial multiplayer.
// Proprietary, not open-sourced. LAN multiplayer (IPX) is the target instead.
#define GCL_STUBBED 1