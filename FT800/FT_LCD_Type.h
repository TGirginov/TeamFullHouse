/* mbed Library for FTDI FT800  Enbedded Video Engine "EVE"
 * ported to mbed by Peter Drescher, DC2PD 2014
 * Released under the MIT License: http://mbed.org/license/mit */
 
 /* change this file to adapt other LCD screens */
 
#ifndef FT_LCD_TYPE_H
#define FT_LCD_TYPE_H

/* Global variables for display resolution to support various display panels */
/* Default is WQVGA - 480x272 */

#define my_DispWidth  480
#define my_DispHeight 272
#define my_DispHCycle 548
#define my_DispHOffset 43
#define my_DispHSync0  0
#define my_DispHSync1  41
#define my_DispVCycle  292
#define my_DispVOffset  12
#define my_DispVSync0  0
#define my_DispVSync1  10
#define my_DispPCLK  5
#define my_DispSwizzle  0
#define my_DispPCLKPol  1

/* the GLYN display has inverted backlite */
#define Inv_Backlite    

#endif
