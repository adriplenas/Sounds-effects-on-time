/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  I2C header file
 *
 */

#ifndef I2C_
#define I2C_

#include "SEmp_5515.h"

#define MDR_STT			0x2000
#define MDR_TRX			0x0200
#define MDR_MST			0x0400
#define MDR_IRS			0x0020
#define MDR_FREE		0x4000
#define STR_XRDY		0x0010
#define STR_RRDY		0x0008
#define MDR_STP 		0x0800
/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
int16_t USBSTK5515_I2C_init ( );
int16_t USBSTK5515_I2C_close( );
int16_t USBSTK5515_I2C_read( uint16_t i2c_addr, uint16_t* data, uint16_t len );
int16_t USBSTK5515_I2C_write( uint16_t i2c_addr, uint16_t* data, uint16_t len );

#endif
