#ifndef APP_UHF_H
#define APP_UHF_H

#include "driver/keyboard.h"
#include <stdint.h>

void UHF_UpdateVfo();
void UHF_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld);

#endif
