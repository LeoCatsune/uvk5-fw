#include "ui/uhfcb.h"
#include "app/uhfcb.h"
#include "driver/st7565.h"
#include "external/printf/printf.h"
#include "misc.h"
#include "settings.h"
#include "ui/helper.h"
#include "ui/ui.h"
#include <stdint.h>
#include <string.h>
#include <uhfcb.h>

bool lDidInit = false;

void UI_UHF_TimeSlice500ms() {
  if (gScreenToDisplay == DISPLAY_UHF) {
    // TODO: other stuff here?
  }
}

void UI_DisplayUhf(void) {
  char StatusString[16];
  char FreqString[12];

  if (gEeprom.DUAL_WATCH || gEeprom.CROSS_BAND_RX_TX) {
    gEeprom.DUAL_WATCH = 0;
    gEeprom.CROSS_BAND_RX_TX = 0;
    gUpdateStatus = true;
  }

  if (gEeprom.UHFCB_CHANNEL < 1 || gEeprom.UHFCB_CHANNEL > UHFCB_NUM_CHANNELS) {
    gEeprom.UHFCB_CHANNEL = 1;
  }

  // We don't currently store the frequency data to EEPROM,
  // so best to initialize the first time around just in case
  if (!lDidInit) {
    lDidInit = true;
    UHF_UpdateVfo();
  }

  UI_DisplayClear();

  const uint32_t chan = uhfcbChanList[gEeprom.UHFCB_CHANNEL - 1];
  const uint8_t flags = UHFCB_EXTRACT_FLAGS(chan);

  const char *name = uhfcb_flags_name(flags);
  const char *repeater =
      (gEeprom.UHFCB_USE_REPEATER && (flags & UHFCB_FLAG_RP_OUT)) ? "R" : "";
  uint32_t freq = UHFCB_EXTRACT_FREQ(chan);

  uint16_t mhz = freq / 1000000;
  uint16_t khz = (freq % 1000000) / 1000;

  if (name != NULL) {
    snprintf_(StatusString, 16, "%u%s: %s", gEeprom.UHFCB_CHANNEL, repeater,
              name);
  } else {
    snprintf_(StatusString, 10, "Ch%u%s", gEeprom.UHFCB_CHANNEL, repeater);
  }
  snprintf_(FreqString, 12, "%lu.%03lu MHz", mhz, khz);

  UI_PrintString(StatusString, 0, LCD_WIDTH, 1, 10);
  UI_PrintString(FreqString, 0, LCD_WIDTH, 3, 8);

  ST7565_BlitFullScreen();
}

// ***************************************************************************
