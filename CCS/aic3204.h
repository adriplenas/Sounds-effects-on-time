/*
 * aic3204.h
 *
 *  Created on: 12/11/2013
 *      Author: isidro
 */

#ifndef AIC3204_H_
#define AIC3204_H_


int16_t AIC3204_rget(  uint16_t regnum, uint16_t* regval );
int16_t AIC3204_rset( uint16_t regnum, uint16_t regval );
void AIC3204_init(void);

#endif /* AIC3204_H_ */
