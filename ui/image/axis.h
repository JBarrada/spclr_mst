#ifndef axisH
#define axisH

#ifndef SMfgTypes
#define SMfgTypes

/*======= binary input =======*/
#define b2b(b7,b6,b5,b4,b3,b2,b1,b0) ((unsigned char)((b7)*128u + (b6)*64u + (b5)*32u + (b4)*16u + (b3)*8u + (b2)*4u + (b1)*2u + (b0)))

/*============================================================================*/
/* You have to manualy set correct types TCDATA and TCLISTP                   */
/* for your platform and compiler.                                            */
/*                                                                            */
/* Keil C51 example:                                                          */
/* Character data (TCDATA) are stored in code memory,                         */
/* array of pointers to characters (TCLISTP) is stored in code memory         */
/* and pointers are pointing into code memory.                                */
/*                                                                            */
/* typedef unsigned char code TCDATA;                                         */
/* typedef TCDATA * code TCLISTP;                                             */
/*============================================================================*/

typedef unsigned char TCDATA;
typedef TCDATA* TCLISTP;

#endif

extern TCLISTP axis[256];

#endif
