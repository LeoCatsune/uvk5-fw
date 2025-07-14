#ifndef APP_UHF_H
#define APP_UHF_H

#include "driver/keyboard.h"
#include <stdint.h>

extern uint8_t gUhfCurrChannel;
void UHF_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld);

#endif
