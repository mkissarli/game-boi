#if DEBUG_MODE /* ----------------------------------------------------------- */

#include <gb/bgb_emu.h>
#include <stdio.h>

static char debug_buffer[512];

/* Some debugging utilities for the BGB emulator if we're in DEBUG_MODE. */
#define DEBUG_PROFILE_BEGIN(msg) BGB_PROFILE_BEGIN(msg                      )
#define DEBUG_PROFILE_END(  msg) BGB_PROFILE_END  (msg                      )
#define DEBUG_LOG_MESSAGE(  ...) BGB_MESSAGE_FMT  (debug_buffer, __VA_ARGS__)

#else

#define DEBUG_PROFILE_BEGIN(msg)
#define DEBUG_PROFILE_END(  msg)
#define DEBUG_LOG_MESSAGE(  ...)

#endif  /* ------------------------------------------------------------------ */
