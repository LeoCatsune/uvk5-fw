#include "uhfcb.h"
#include "dcs.h"
#include "frequencies.h"
#include "radio.h"
#include "settings.h"
#include <string.h>

const uint32_t uhfcbChanList[] = {
    UHFCB_PACK_FLAGS(0, UHFCB_FLAG_RP_OUT),                      // 1
    UHFCB_PACK_FLAGS(2, UHFCB_FLAG_RP_OUT),                      // 2
    UHFCB_PACK_FLAGS(4, UHFCB_FLAG_RP_OUT),                      // 3
    UHFCB_PACK_FLAGS(6, UHFCB_FLAG_RP_OUT),                      // 4
    UHFCB_PACK_FLAGS(8, UHFCB_FLAG_RP_OUT | UHFCB_FLAG_SOS | 1), // 5 ("SOS")
    UHFCB_PACK_FLAGS(10, UHFCB_FLAG_RP_OUT),                     // 6
    UHFCB_PACK_FLAGS(12, UHFCB_FLAG_RP_OUT),                     // 7
    UHFCB_PACK_FLAGS(14, UHFCB_FLAG_RP_OUT),                     // 8
    UHFCB_PACK_FLAGS(16, 0),                                     // 9
    UHFCB_PACK_FLAGS(18, 2),                // 10 ("CONVOY")
    UHFCB_PACK_FLAGS(20, 3),                // 11 ("CALL")
    UHFCB_PACK_FLAGS(22, 0),                // 12
    UHFCB_PACK_FLAGS(24, 0),                // 13
    UHFCB_PACK_FLAGS(26, 0),                // 14
    UHFCB_PACK_FLAGS(28, 0),                // 15
    UHFCB_PACK_FLAGS(30, 0),                // 16
    UHFCB_PACK_FLAGS(32, 0),                // 17
    UHFCB_PACK_FLAGS(34, 0),                // 18
    UHFCB_PACK_FLAGS(36, 0),                // 19
    UHFCB_PACK_FLAGS(38, 0),                // 20
    UHFCB_PACK_FLAGS(40, 0),                // 21
    UHFCB_PACK_FLAGS(42, UHFCB_FLAG_DATA),  // 22 (DATA1)
    UHFCB_PACK_FLAGS(44, UHFCB_FLAG_DATA),  // 23 (DATA2)
    UHFCB_PACK_FLAGS(46, 0),                // 24
    UHFCB_PACK_FLAGS(48, 0),                // 25
    UHFCB_PACK_FLAGS(50, 0),                // 26
    UHFCB_PACK_FLAGS(52, 0),                // 27
    UHFCB_PACK_FLAGS(54, 0),                // 28
    UHFCB_PACK_FLAGS(56, 4),                // 29 ("HIWAY")
    UHFCB_PACK_FLAGS(58, 5),                // 30 ("BCAST")
    UHFCB_PACK_FLAGS(60, UHFCB_FLAG_RP_IN), // 31
    UHFCB_PACK_FLAGS(62, UHFCB_FLAG_RP_IN), // 32
    UHFCB_PACK_FLAGS(64, UHFCB_FLAG_RP_IN), // 33
    UHFCB_PACK_FLAGS(66, UHFCB_FLAG_RP_IN), // 34
    UHFCB_PACK_FLAGS(68, UHFCB_FLAG_RP_IN | UHFCB_FLAG_SOS | 1), // 35 (SOS)
    UHFCB_PACK_FLAGS(70, UHFCB_FLAG_RP_IN),                      // 36
    UHFCB_PACK_FLAGS(72, UHFCB_FLAG_RP_IN),                      // 37
    UHFCB_PACK_FLAGS(74, UHFCB_FLAG_RP_IN),                      // 38
    UHFCB_PACK_FLAGS(76, 0),                                     // 39
    UHFCB_PACK_FLAGS(78, 6),                                     // 40 ("ROAD")
    UHFCB_PACK_FLAGS(1, UHFCB_FLAG_RP_OUT),                      // 41
    UHFCB_PACK_FLAGS(3, UHFCB_FLAG_RP_OUT),                      // 42
    UHFCB_PACK_FLAGS(5, UHFCB_FLAG_RP_OUT),                      // 43
    UHFCB_PACK_FLAGS(7, UHFCB_FLAG_RP_OUT),                      // 44
    UHFCB_PACK_FLAGS(9, UHFCB_FLAG_RP_OUT),                      // 45
    UHFCB_PACK_FLAGS(11, UHFCB_FLAG_RP_OUT),                     // 46
    UHFCB_PACK_FLAGS(13, UHFCB_FLAG_RP_OUT),                     // 47
    UHFCB_PACK_FLAGS(15, UHFCB_FLAG_RP_OUT),                     // 48
    UHFCB_PACK_FLAGS(17, 0),                                     // 49
    UHFCB_PACK_FLAGS(19, 0),                                     // 50
    UHFCB_PACK_FLAGS(21, 0),                                     // 51
    UHFCB_PACK_FLAGS(23, 0),                                     // 52
    UHFCB_PACK_FLAGS(25, 0),                                     // 53
    UHFCB_PACK_FLAGS(27, 0),                                     // 54
    UHFCB_PACK_FLAGS(29, 0),                                     // 55
    UHFCB_PACK_FLAGS(31, 0),                                     // 56
    UHFCB_PACK_FLAGS(33, 0),                                     // 57
    UHFCB_PACK_FLAGS(35, 0),                                     // 58
    UHFCB_PACK_FLAGS(37, 0),                                     // 59
    UHFCB_PACK_FLAGS(39, 0),                                     // 60
    UHFCB_PACK_FLAGS(41, UHFCB_FLAG_RESERVED),                   // 61
    UHFCB_PACK_FLAGS(43, UHFCB_FLAG_RESERVED),                   // 62
    UHFCB_PACK_FLAGS(45, UHFCB_FLAG_RESERVED),                   // 63
    UHFCB_PACK_FLAGS(47, 0),                                     // 64
    UHFCB_PACK_FLAGS(49, 0),                                     // 65
    UHFCB_PACK_FLAGS(51, 0),                                     // 66
    UHFCB_PACK_FLAGS(53, 0),                                     // 67
    UHFCB_PACK_FLAGS(55, 0),                                     // 68
    UHFCB_PACK_FLAGS(57, 0),                                     // 69
    UHFCB_PACK_FLAGS(59, 0),                                     // 70
    UHFCB_PACK_FLAGS(61, UHFCB_FLAG_RP_IN),                      // 71
    UHFCB_PACK_FLAGS(63, UHFCB_FLAG_RP_IN),                      // 72
    UHFCB_PACK_FLAGS(65, UHFCB_FLAG_RP_IN),                      // 73
    UHFCB_PACK_FLAGS(67, UHFCB_FLAG_RP_IN),                      // 74
    UHFCB_PACK_FLAGS(69, UHFCB_FLAG_RP_IN),                      // 75
    UHFCB_PACK_FLAGS(71, UHFCB_FLAG_RP_IN),                      // 76
    UHFCB_PACK_FLAGS(73, UHFCB_FLAG_RP_IN),                      // 77
    UHFCB_PACK_FLAGS(75, UHFCB_FLAG_RP_IN),                      // 78
    UHFCB_PACK_FLAGS(77, 0),                                     // 79
    UHFCB_PACK_FLAGS(79, 0),                                     // 80
};

static_assert((sizeof(uhfcbChanList) / sizeof(uint32_t)) == UHFCB_NUM_CHANNELS,
              "Channel table must contain exactly 80 entries");

const char *uhfcbNameList[] = {NULL,    "SOS",   "CONVOY", "CALL",
                               "HIWAY", "BCAST", "ROAD",   "*ERR*"};

static_assert(
    (sizeof(uhfcbNameList) / sizeof(uhfcbNameList[0])) == UHFCB_NUM_NAMES,
    "Channel name table must contain exactly 7 names (8 entries, 0=NULL)");

inline const char *uhfcb_flags_name(uint8_t flags) {
  if (flags & UHFCB_FLAG_DATA)
    return "DATA";
  if (flags & UHFCB_FLAG_RESERVED)
    return "RESV";
  return uhfcbNameList[flags & UHFCB_MASK_NAMEID];
}

VFO_Info_t uhfcb_vfoinfo(uint8_t channel, bool repeater) {
  if (channel < 1 || channel > UHFCB_NUM_CHANNELS) {
    channel = 1;
  }
  channel--; // channel to index
  const uint32_t chan = uhfcbChanList[channel];

  VFO_Info_t vfoInfo;
  FREQ_Config_t freqInfo;

  uint8_t flags = UHFCB_EXTRACT_FLAGS(chan);

  freqInfo.Frequency = UHFCB_EXTRACT_FREQ(chan);
  freqInfo.CodeType = CODE_TYPE_OFF;

  vfoInfo.Band = BAND7_470MHz;
  vfoInfo.Modulation = MODULATION_FM;
  vfoInfo.CHANNEL_BANDWIDTH = BANDWIDTH_NARROW;

  vfoInfo.freq_config_TX = freqInfo;
  vfoInfo.freq_config_RX = freqInfo;
  vfoInfo.pTX = &vfoInfo.freq_config_TX;
  vfoInfo.pRX = &vfoInfo.freq_config_RX;
  vfoInfo.FrequencyReverse = 0;
  vfoInfo.Compander = 0;

#ifdef ENABLE_DTMF_CALLING
  vfoInfo.DTMF_DECODING_ENABLE = 0;
#endif
  vfoInfo.DTMF_PTT_ID_TX_MODE = PTT_ID_OFF;
  vfoInfo.STEP_SETTING = STEP_12_5kHz;
  vfoInfo.TX_OFFSET_FREQUENCY = UHFCB_RP_FREQ_OFFSET;
  vfoInfo.TX_OFFSET_FREQUENCY_DIRECTION =
      (repeater && (flags & UHFCB_FLAG_RP_OUT))
          ? TX_OFFSET_FREQUENCY_DIRECTION_ADD
          : TX_OFFSET_FREQUENCY_DIRECTION_OFF;

  return vfoInfo;
}
