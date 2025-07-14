#include "uhfcb.h"
#include <stdint.h>

uint8_t gUhfCurrChannel = 1;

void UHF_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld) {
  // NOTE: Temp void to stop clangd from yelling at me.
  (void)bKeyHeld;

  // Only handle key release events, we can ignore the initial keypresses
  if (bKeyPressed)
    return;

  switch (Key) {
  case KEY_UP:
    if (gUhfCurrChannel < 80)
      gUhfCurrChannel++;
    break;
  case KEY_DOWN:
    if (gUhfCurrChannel > 1)
      gUhfCurrChannel--;
    break;
  default:
    // do nothing.
    return;
  }
}
