/* IIo Gen 3 Tx Eq values for cards

   This table contains the whole set of values for C0/M0 steppings. 
   When initialize Gen3 the system will check if the attached card
   belongs to any of the specified cards on the table. 
   
   The format is: 
   Vendor  Device  Rsvd1  Platform  Socket  Port  UpstreamTxEqValid  UpstreamTxEq       DownstreamTxEqValid  DownstreamTxEq
   ID      ID      0,     0,        0,      0,    1,                 {Pre, Cur, Post},  0,                   {0, 0, 0}
   ------------------------------------------------------------

For instance:
  {0x8086, 0x0000, 0, 0xFF, 0xFF, 0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // Intel empty slot    This entry will be set for all Platforms, all Sockets and all Ports
  {0x8086, 0x0000, 0, 0x13, 0x0B, 0x00FF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // Intel empty slot    This entry will be set for Platform with BoardId = 0x13, Socket[3,1,0], Ports[7-0]
  {0x8086, 0x0000, 0, 0x18, 0x0B, 0x00FF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // Intel empty slot    This entry will be set for Platform with BoardId = 0x18, Socket[3,1,0], Ports[7-0]

 */
#ifndef IIOGEN3TXSEQ_H
#define IIOGEN3TXSEQ_H

#include "IioGen3.h"

PORTPARAMS_STRUCT Gen3OverrideTxEqParamTable[] = {
// Vid    Did   Rsvd1 Platform Socket Port UpstreamTxEqValid UpstreamTxEq DownstreamTxEqValid, DownstreamTxEq
  {0x1000, 0x0090, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x0091, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x0092, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x0093, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x0094, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x0095, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x0098, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x0099, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x009a, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x009b, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x009c, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x009d, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Invader
  {0x1000, 0x0096, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3008 Fury
  {0x1000, 0x0097, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Fury
  {0x1000, 0x009e, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Fury
  {0x1000, 0x009f, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Fury
  {0x1000, 0x006c, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Fury
  {0x1000, 0x006d, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // LSI 3108 Fury
  {0x1000, 0x0000, 0, 0xFF,  0xFF,  0xFFFF, 0, {  0,  00,   0}, 0, {0,  0, 0}}, // LSI Empty slot
  {0x1002, 0x6798, 0, 0xFF,  0xFF,  0xFFFF, 1, {  5,  45,  13}, 0, {0,  0, 0}}, // AMD ATI 7970 (Tahiti)
  {0x1002, 0x679A, 0, 0xFF,  0xFF,  0xFFFF, 1, {  5,  45,  13}, 0, {0,  0, 0}}, // AMD ATI 7950 (Tahiti)
  {0x1002, 0x665c, 0, 0xFF,  0xFF,  0xFFFF, 1, {  5,  45,  13}, 0, {0,  0, 0}}, // AMD 7700 (Bonaire)
  {0x1002, 0x0000, 0, 0xFF,  0xFF,  0xFFFF, 0, {  0,   0,   0}, 0, {0,  0, 0}}, // AMD Empy slot
  {0x1077, 0x2031, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x2831, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x8030, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x8031, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x8032, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x8033, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x8034, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x8830, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x8831, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x8832, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x8833, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x1077, 0x8834, 0, 0xFF,  0xFF,  0xFFFF, 0, {  6,  44,  13}, 0, {0,  0, 0}}, // Qlogic QLE2672
  {0x10b5, 0x8747, 0, 0xFF,  0xFF,  0xFFFF, 1, {  0,  56,   7}, 0, {0,  0, 0}}, // PLX CA TSE
  {0x10de, 0x1024, 0, 0xFF,  0xFF,  0xFFFF, 1, {  7,  42,  14}, 0, {0,  0, 0}}, // NVidia K40
  {0x10de, 0x103A, 0, 0xFF,  0xFF,  0xFFFF, 1, {  7,  42,  14}, 0, {0,  0, 0}}, // Nvidia K600
  {0x10de, 0x0000, 0, 0xFF,  0xFF,  0xFFFF, 0, {  0,   0,   0}, 0, {0,  0, 0}}, // Nvidia Empty
  {0x10df, 0x0720, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // Emulex Skyhawk
  {0x10df, 0xe200, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // Emulex LPe16002 (Lancer)
  {0x10df, 0xe220, 0, 0xFF,  0xFF,  0xFFFF, 1, {  8,  55,   0}, 0, {0,  0, 0}}, // Emulex Lancer Ethernet.
  {0x10df, 0x0000, 0, 0xFF,  0xFF,  0xFFFF, 0, {  0,   0,   0}, 0, {0,  0, 0}}, // Emulex empty slot
  {0x8086, 0x0953, 0, 0xFF,  0xFF,  0xFFFF, 1, {  6,  44,  13}, 0, {0,  0, 0}}, // Intel SSD (Fultondale)
  {0x8086, 0x1572, 0, 0xFF,  0xFF,  0xFFFF, 1, {  6,  57,   0}, 0, {0,  0, 0}}, // Intel fortville
  {0x8086, 0x0000, 0, 0xFF,  0xFF,  0xFFFF, 0, {  0,   0,   0}, 0, {0,  0, 0}}, // Intel empty slot
};
#endif