#ifndef APP_UHF_H
#define APP_UHF_H

#include "driver/keyboard.h"
#include <stdint.h>

extern uint8_t gUhfChanInputLength;
extern uint8_t gUhfChanInputValue;

void UHF_UpdateVfo();
void UHF_SetChannel(uint8_t chan, bool userp);
void UHF_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld);

#endif
