#include "uhfcb.h"
#include <string.h>

#define UHFCB_BASE_HZ 476425000
#define UHFCB_STEP_HZ 12500

#define UHFCB_FLAG_RP_OUT 0x08
#define UHFCB_FLAG_RP_IN 0x10
#define UHFCB_FLAG_SOS 0x20
#define UHFCB_FLAG_DATA 0x40
#define UHFCB_FLAG_RESERVED 0x80

#define UHFCB_MASK_NAMEID 0x7
#define UHFCB_MASK_TX_DISALLOW (UHF_FLAG_DATA | UHF_FLAG_RESERVED)

const char *uhfcbNameList[] = {NULL,    "SOS",   "CONVOY", "CALL",
                               "HIWAY", "BCAST", "ROAD",   "*ERR*"};

#define UHFCB_NUM_NAMES (sizeof(uhfcbNameList) / sizeof(uhfcbNameList[0]))
static_assert(
    UHFCB_NUM_NAMES == 8,
    "Channel name table must contain exactly 7 names (8 entries, 0=NULL)");

const uhfcb_chan_t uhfcbChanList[] = {
    {0, UHFCB_FLAG_RP_OUT},                      // 1
    {2, UHFCB_FLAG_RP_OUT},                      // 2
    {4, UHFCB_FLAG_RP_OUT},                      // 3
    {6, UHFCB_FLAG_RP_OUT},                      // 4
    {8, UHFCB_FLAG_RP_OUT | UHFCB_FLAG_SOS | 1}, // 5 ("SOS")
    {10, UHFCB_FLAG_RP_OUT},                     // 6
    {12, UHFCB_FLAG_RP_OUT},                     // 7
    {14, UHFCB_FLAG_RP_OUT},                     // 8
    {16, 0},                                     // 9
    {18, 2},                                     // 10 ("CONVOY")
    {20, 3},                                     // 11 ("CALL")
    {22, 0},                                     // 12
    {24, 0},                                     // 13
    {26, 0},                                     // 14
    {28, 0},                                     // 15
    {30, 0},                                     // 16
    {32, 0},                                     // 17
    {34, 0},                                     // 18
    {36, 0},                                     // 19
    {38, 0},                                     // 20
    {40, 0},                                     // 21
    {42, UHFCB_FLAG_DATA},                       // 22 (DATA1)
    {44, UHFCB_FLAG_DATA},                       // 23 (DATA2)
    {46, 0},                                     // 24
    {48, 0},                                     // 25
    {50, 0},                                     // 26
    {52, 0},                                     // 27
    {54, 0},                                     // 28
    {56, 4},                                     // 29 ("HIWAY")
    {58, 5},                                     // 30 ("BCAST")
    {60, UHFCB_FLAG_RP_IN},                      // 31
    {62, UHFCB_FLAG_RP_IN},                      // 32
    {64, UHFCB_FLAG_RP_IN},                      // 33
    {66, UHFCB_FLAG_RP_IN},                      // 34
    {68, UHFCB_FLAG_RP_IN | UHFCB_FLAG_SOS | 1}, // 35 (SOS)
    {70, UHFCB_FLAG_RP_IN},                      // 36
    {72, UHFCB_FLAG_RP_IN},                      // 37
    {74, UHFCB_FLAG_RP_IN},                      // 38
    {76, 0},                                     // 39
    {78, 6},                                     // 40 ("ROAD")
    {1, UHFCB_FLAG_RP_OUT},                      // 41
    {3, UHFCB_FLAG_RP_OUT},                      // 42
    {5, UHFCB_FLAG_RP_OUT},                      // 43
    {7, UHFCB_FLAG_RP_OUT},                      // 44
    {9, UHFCB_FLAG_RP_OUT},                      // 45
    {11, UHFCB_FLAG_RP_OUT},                     // 46
    {13, UHFCB_FLAG_RP_OUT},                     // 47
    {15, UHFCB_FLAG_RP_OUT},                     // 48
    {17, 0},                                     // 49
    {19, 0},                                     // 50
    {21, 0},                                     // 51
    {23, 0},                                     // 52
    {25, 0},                                     // 53
    {27, 0},                                     // 54
    {29, 0},                                     // 55
    {31, 0},                                     // 56
    {33, 0},                                     // 57
    {35, 0},                                     // 58
    {37, 0},                                     // 59
    {39, 0},                                     // 60
    {41, UHFCB_FLAG_RESERVED},                   // 61
    {43, UHFCB_FLAG_RESERVED},                   // 62
    {45, UHFCB_FLAG_RESERVED},                   // 63
    {47, 0},                                     // 64
    {49, 0},                                     // 65
    {51, 0},                                     // 66
    {53, 0},                                     // 67
    {55, 0},                                     // 68
    {57, 0},                                     // 69
    {59, 0},                                     // 70
    {61, UHFCB_FLAG_RP_IN},                      // 71
    {63, UHFCB_FLAG_RP_IN},                      // 72
    {65, UHFCB_FLAG_RP_IN},                      // 73
    {67, UHFCB_FLAG_RP_IN},                      // 74
    {69, UHFCB_FLAG_RP_IN},                      // 75
    {71, UHFCB_FLAG_RP_IN},                      // 76
    {73, UHFCB_FLAG_RP_IN},                      // 77
    {75, UHFCB_FLAG_RP_IN},                      // 78
    {77, 0},                                     // 79
    {79, 0},                                     // 80
};

#define UHFCB_NUM_CHANNELS (sizeof(uhfcbChanList) / sizeof(uhfcbChanList[0]))
static_assert(UHFCB_NUM_CHANNELS == 80,
              "Channel table must contain exactly 80 entries");

static inline uint32_t uhfcb_freq_hz(uint8_t slot) {
  if (slot >= 80)
    slot = 0;
  return UHFCB_BASE_HZ + (uint32_t)slot * UHFCB_STEP_HZ;
}

static inline const char *uhfcb_flags_name(uint8_t flags) {
  return uhfcbNameList[flags & UHFCB_MASK_NAMEID];
}
