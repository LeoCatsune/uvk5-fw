#ifndef UHFCB_H
#define UHFCB_H

#include <stdint.h>

#define UHFCB_BASE_HZ 476425000
#define UHFCB_STEP_HZ 12500

#define UHFCB_FLAG_RP_OUT 0x08
#define UHFCB_FLAG_RP_IN 0x10
#define UHFCB_FLAG_SOS 0x20
#define UHFCB_FLAG_DATA 0x40
#define UHFCB_FLAG_RESERVED 0x80

#define UHFCB_MASK_NAMEID 0x7
#define UHFCB_MASK_TX_DISALLOW (UHF_FLAG_DATA | UHF_FLAG_RESERVED)

#define UHFCB_NUM_CHANNELS 80
#define UHFCB_NUM_NAMES 8
#define UHFCB_RP_SLOT_OFFSET 60
#define UHFCB_RP_FREQ_OFFSET (UHFCB_RP_SLOT_OFFSET * UHFCB_STEP_HZ)

#define UHFCB_FREQ_MASK 0x00FFFFFF
#define UHFCB_FREQ_MAGIC 0x1c000000
#define UHFCB_FLAGS_MASK 0xFF000000
#define UHFCB_PACK_FLAGS(slot, flags)                                          \
  (((UHFCB_BASE_HZ + (UHFCB_STEP_HZ * slot)) & UHFCB_FREQ_MASK) | (flags << 24))
#define UHFCB_EXTRACT_FREQ(packed)                                             \
  ((packed & UHFCB_FREQ_MASK) | UHFCB_FREQ_MAGIC)
#define UHFCB_EXTRACT_FLAGS(packed) ((packed & UHFCB_FLAGS_MASK) >> 24)

extern const uint32_t uhfcbChanList[UHFCB_NUM_CHANNELS];

const char *uhfcb_flags_name(uint8_t flags);

#endif
