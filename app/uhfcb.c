#include "app/generic.h"
#include "audio.h"
#include "driver/bk4819.h"
#include "driver/keyboard.h"
#include "frequencies.h"
#include "misc.h"
#include "radio.h"
#include "settings.h"
#include <stdint.h>
#include <uhfcb.h>

void UHF_UpdateVfo() {
  uint32_t chan = uhfcbChanList[gEeprom.UHFCB_CHANNEL - 1];
  uint8_t flags = UHFCB_EXTRACT_FLAGS(chan);
  uint32_t frequency = UHFCB_EXTRACT_FREQ(chan) / 10;

  gTxVfo->CHANNEL_SAVE = FREQ_CHANNEL_FIRST;
  gTxVfo->Band = BAND7_470MHz;
  gTxVfo->freq_config_RX.Frequency = frequency;
  gTxVfo->OUTPUT_POWER = OUTPUT_POWER_LOW;
  gTxVfo->TX_OFFSET_FREQUENCY = 75000; // it just is.
  gTxVfo->TX_OFFSET_FREQUENCY_DIRECTION =
      (gEeprom.UHFCB_USE_REPEATER && (flags & UHFCB_FLAG_RP_OUT))
          ? TX_OFFSET_FREQUENCY_DIRECTION_ADD
          : TX_OFFSET_FREQUENCY_DIRECTION_OFF;
  gEeprom.ScreenChannel[gEeprom.TX_VFO] = gEeprom.FreqChannel[gEeprom.TX_VFO];
  RADIO_ApplyOffset(gTxVfo);
  SETTINGS_SaveChannel(gTxVfo->CHANNEL_SAVE, gEeprom.TX_VFO, gTxVfo, true);
  BK4819_SetFrequency(frequency);
  BK4819_RX_TurnOn();
}

void UHF_ProcessKey_UP_DOWN(bool bKeyPressed, bool bKeyHeld, int8_t direction) {
  (void)bKeyHeld;
  if (bKeyPressed)
    return;

  uint8_t newChannel = gEeprom.UHFCB_CHANNEL + direction;
  if (newChannel < 1 || newChannel > UHFCB_NUM_CHANNELS) {
    gBeepToPlay = BEEP_500HZ_60MS_DOUBLE_BEEP_OPTIONAL;
    return;
  }

  gEeprom.UHFCB_CHANNEL = newChannel;
  gEeprom.UHFCB_USE_REPEATER = false;
  UHF_UpdateVfo();
  gBeepToPlay = BEEP_1KHZ_60MS_OPTIONAL;
}

void UHF_ProcessKey_STAR(bool bKeyPressed) {
  if (bKeyPressed)
    return;

  uint8_t flags = UHFCB_EXTRACT_FLAGS(uhfcbChanList[gEeprom.UHFCB_CHANNEL - 1]);
  if (flags & UHFCB_FLAG_RP_OUT) {
    gEeprom.UHFCB_USE_REPEATER = !gEeprom.UHFCB_USE_REPEATER;
    gBeepToPlay = BEEP_1KHZ_60MS_OPTIONAL;
    UHF_UpdateVfo();
  } else {
    gBeepToPlay = BEEP_500HZ_60MS_DOUBLE_BEEP_OPTIONAL;
  }
}

void UHF_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld) {
  switch (Key) {
  case KEY_UP:
    UHF_ProcessKey_UP_DOWN(bKeyPressed, bKeyHeld, 1);
    break;
  case KEY_DOWN:
    UHF_ProcessKey_UP_DOWN(bKeyPressed, bKeyHeld, -1);
    break;
  case KEY_STAR:
    // TODO: Change this (use F-keys?)
    UHF_ProcessKey_STAR(bKeyPressed);
    break;
  case KEY_PTT:
    GENERIC_Key_PTT(bKeyPressed);
    break;
  default:
    // do nothing.
    return;
  }
}
