#ifndef UHFCB_H
#define UHFCB_H

#include <stdint.h>

typedef struct {
  // Frequency Slot (25kHz steps)
  const uint8_t freq_slot;
  // Channel Info (bitfield)
  // 0-2: Name Bits (see uhfcbNameList)
  // 3: Repeater Output
  // 4: Repeater Input
  // 5: SOS
  // 6: Data Only
  // 7: (Channel) Reserved
  const uint8_t info;
} uhfcb_chan_t;

static uint32_t uhfcb_freq_hz(uint8_t slot);
static const char *uhfcb_flags_name(uint8_t flags);

#endif
