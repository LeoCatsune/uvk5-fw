#include "ui/uhfcb.h"
#include "app/uhfcb.h"
#include "driver/st7565.h"
#include "external/printf/printf.h"
#include "ui/helper.h"
#include <stdint.h>
#include <string.h>
#include <uhfcb.h>

void UI_DisplayUhf(void) {
  char StatusString[10];

  UI_DisplayClear();

  const char *name = uhfcb_flags_name(uhfcbChanList[gUhfCurrChannel - 1].info);

  snprintf_(StatusString, 10, "Ch%u", gUhfCurrChannel);
  if (name != NULL) {
    UI_PrintString(StatusString, 0, LCD_WIDTH, 1, 10);
    UI_PrintString(name, 0, LCD_WIDTH, 3, 10);
  } else {
    UI_PrintString(StatusString, 0, LCD_WIDTH, 2, 10);
  }

  ST7565_BlitFullScreen();
}

// ***************************************************************************
