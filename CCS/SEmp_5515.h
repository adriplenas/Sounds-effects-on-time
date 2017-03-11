/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Variable types                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */




#ifndef SEmp_5515
#define SEmp_5515

#include <stdint.h>


#define CPU_PCGR1 *(volatile uint16_t *)0x1C02
#define CPU_PCGR2 *(volatile uint16_t *)0x1C03


#define CPU_IER0 *(volatile uint16_t *)0x0000
#define CPU_IER1 *(volatile uint16_t *)0x0045
#define CPU_IFR0 *(volatile uint16_t *)0x0001
#define CPU_IFR1 *(volatile uint16_t *)0x0046

#define CPU_ST1_55 *(volatile uint16_t *)0x0003
#define CPU_ST2_55 *(volatile uint16_t *)0x004B

#define CPU_IVPD *(volatile uint16_t *)0x0049
#define CPU_IVPH *(volatile uint16_t *)0x004A

#define EBSR *(volatile uint16_t *)0x1C00

//
// Registros de dirección de los GPIO: 1,2
#define IODIR1 *(( ioport volatile uint16_t *) 0x1C06 )
#define IODIR2 *(( ioport volatile uint16_t *) 0x1C07 )

// Registros de lectura de datos  GPIO: 1,2
#define IODATAOUT1 *(( ioport volatile uint16_t *) 0x1C0A )
#define IODATAOUT2 *(( ioport volatile uint16_t *) 0x1C0B )


//  Registros del Conversor ADC

# define SARCTRL    *((  volatile ioport uint16_t *) 0x7012 )
# define SARDATA    *((  volatile ioport uint16_t *) 0x7014 )
# define SARCLKCTRL *((  volatile ioport uint16_t *) 0x7016 )
# define SARPINCTRL *((  volatile ioport uint16_t *) 0x7018 )
# define SARGPOCTRL *((  volatile ioport uint16_t *) 0x701A )

// Registros del TIMER


# define TIAFR *((  volatile ioport uint16_t *) 0x1C14 )

// TIM0
# define TIM0TCR  *((  volatile ioport uint16_t *) 0x1810 )
# define TIM0PRD1 *((  volatile ioport uint16_t *) 0x1812 )
# define TIM0PRD2 *((  volatile ioport uint16_t *) 0x1813 )
# define TIM0CNT1 *((  volatile ioport uint16_t *) 0x1814 )
# define TIM0CNT2 *((  volatile ioport uint16_t *) 0x1815 )

/* ------------------------------------------------------------------------ *
 *  I2C Module                                                              *
 * ------------------------------------------------------------------------ */
 
#define I2C_IER    	   *(volatile ioport uint16_t*)(0x1A04)
#define I2C_STR    	   *(volatile ioport uint16_t*)(0x1A08)
#define I2C_CLKL           *(volatile ioport uint16_t*)(0x1A0C)
#define I2C_CLKH           *(volatile ioport uint16_t*)(0x1A10)
#define I2C_CNT    	   *(volatile ioport uint16_t*)(0x1A14)
#define I2C_DRR   	   *(volatile ioport uint16_t*)(0x1A18)
#define I2C_SAR    	   *(volatile ioport uint16_t*)(0x1A1C)
#define I2C_DXR    	   *(volatile ioport uint16_t*)(0x1A20)
#define I2C_MDR            *(volatile ioport uint16_t*)(0x1A24)
#define I2C_EDR    	   *(volatile ioport uint16_t*)(0x1A2C)
#define I2C_PSC    	   *(volatile ioport uint16_t*)(0x1A30)
/* ------------------------------------------------------------------------ *
 *  I2S Module                                                              *
 * ------------------------------------------------------------------------ */
#define I2S0_CR            *(volatile ioport uint16_t*)(0x2800)
#define I2S0_SRGR          *(volatile ioport uint16_t*)(0x2804)
#define I2S0_W0_LSW_W      *(volatile ioport uint16_t*)(0x2808)
#define I2S0_W0_MSW_W      *(volatile ioport uint16_t*)(0x2809)
#define I2S0_W1_LSW_W      *(volatile ioport uint16_t*)(0x280C)
#define I2S0_W1_MSW_W      *(volatile ioport uint16_t*)(0x280D)
#define I2S0_IR            *(volatile ioport uint16_t*)(0x2810)
#define I2S0_ICMR          *(volatile ioport uint16_t*)(0x2814)
#define I2S0_W0_LSW_R      *(volatile ioport uint16_t*)(0x2828)
#define I2S0_W0_MSW_R      *(volatile ioport uint16_t*)(0x2829)
#define I2S0_W1_LSW_R      *(volatile ioport uint16_t*)(0x282C)
#define I2S0_W1_MSW_R      *(volatile ioport uint16_t*)(0x282D)
/* I2S2 */
#define I2S2_CR            *(volatile ioport uint16_t*)(0x2A00)
#define I2S2_SRGR          *(volatile ioport uint16_t*)(0x2A04)
#define I2S2_W0_LSW_W      *(volatile ioport uint16_t*)(0x2A08)
#define I2S2_W0_MSW_W      *(volatile ioport uint16_t*)(0x2A09)
#define I2S2_W1_LSW_W      *(volatile ioport uint16_t*)(0x2A0C)
#define I2S2_W1_MSW_W      *(volatile ioport uint16_t*)(0x2A0D)
#define I2S2_IR            *(volatile ioport uint16_t*)(0x2A10)
#define I2S2_ICMR          *(volatile ioport uint16_t*)(0x2A14)
#define I2S2_W0_LSW_R      *(volatile ioport uint16_t*)(0x2A28)
#define I2S2_W0_MSW_R      *(volatile ioport uint16_t*)(0x2A29)
#define I2S2_W1_LSW_R      *(volatile ioport uint16_t*)(0x2A2C)
#define I2S2_W1_MSW_R      *(volatile ioport uint16_t*)(0x2A2D)

/* ------------------------------------------------------------------------ *
 *  UART Module                                                             *
 * ------------------------------------------------------------------------ */
#define UART_RBR           *(volatile ioport uint16_t*)(0x1B00)
#define UART_THR           *(volatile ioport uint16_t*)(0x1B00)
#define UART_IER           *(volatile ioport uint16_t*)(0x1B02)
#define UART_IIR           *(volatile ioport uint16_t*)(0x1B04)
#define UART_FCR           *(volatile ioport uint16_t*)(0x1B04)
#define UART_LCR           *(volatile ioport uint16_t*)(0x1B06)
#define UART_MCR           *(volatile ioport uint16_t*)(0x1B08)
#define UART_LSR           *(volatile ioport uint16_t*)(0x1B0A)
#define UART_SCR           *(volatile ioport uint16_t*)(0x1B0E)
#define UART_DLL           *(volatile ioport uint16_t*)(0x1B10)
#define UART_DLH           *(volatile ioport uint16_t*)(0x1B12)
#define UART_PWREMU_MGMT   *(volatile ioport uint16_t*)(0x1B18)

#endif
