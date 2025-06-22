
#pragma once


typedef unsigned int    SID;
typedef unsigned char   SID_Bits;

#define SID_MAX_BASE    0x00ffffff

#define SID_EMPTY       0x00000000
#define SID_3D          0x01000000
#define SID_2D          0x02000000

#define SID_BITS_EMPTY  0x00
#define SID_BITS_3D     0x01
#define SID_BITS_2D     0x02

inline bool     SID_is_empty(SID sid)   { return sid & SID_EMPTY;   }
inline bool     SID_is_3D(SID sid)      { return sid & SID_3D;      }
inline bool     SID_is_2D(SID sid)      { return sid & SID_2D;      }

inline SID_Bits SID_to_bits(SID sid)    { return sid >> 24;         }
