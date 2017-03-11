;vectors.asm
;
; Información sobre los vectores de interrupción 5515:
;    SPRUFX5D: http://www.ti.com/lit/ug/sprufx5d/sprufx5d.pdf
;
;
         .global _c_int00, _ISR_I2S_rx

         .global _Reset  ; Es necesario incluirla con este nombre
                         ; Lo utiliza el entorno de C
                         ; en el fichero .cmd aparece ls instruccion: -u _Reset


             .sect "vectors"
_Reset:         .ivec _c_int00,USE_RETA
NMI:            .ivec    dummy_isr             ; Nonmaskable Interrupt
INT0:           .ivec    dummy_isr             ; External User Interrupt #0
INT1:           .ivec    dummy_isr             ; External User Interrupt #1
TINT0:          .ivec    dummy_isr             ; Timer #0 / Software Interrupt #4
PROG0:          .ivec    dummy_isr             ; Programmable 0 Interrupt
UART:           .ivec    dummy_isr             ; IIS #1 Receive Interrupt
PROG1:          .ivec    dummy_isr             ; Programmable 1 Interrupt
DMA:            .ivec    dummy_isr             ; DMA Interrupt
PROG2:          .ivec    dummy_isr             ; Programmable 2 Interrupt
COPROCFFT:      .ivec    dummy_isr             ; Coprocessor FFT Module Interrupt
PROG3:          .ivec    dummy_isr             ; Programmable 3 Interrupt
LCD:            .ivec    dummy_isr             ; LCD Interrupt
SARADC:         .ivec    dummy_isr             ; SAR ADC Interrupt
XMIT2:          .ivec    dummy_isr             ; I2S2 Tx Interrupt
RCV2:           .ivec    _ISR_I2S_rx           ; I2S2 Rx Interrupt
XMIT3:          .ivec    dummy_isr             ; I2S3 Tx Interrupt
RCV3:           .ivec    dummy_isr             ; I2S3 Rx Interrupt
RTC:            .ivec    dummy_isr             ; RTC interrupt
SPI:            .ivec    dummy_isr             ; SPI Receive Interrupt
USB:            .ivec    dummy_isr             ; USB Transmit Interrupt
GPIO:           .ivec    dummy_isr             ; GPIO Interrupt
EMIF:           .ivec    dummy_isr             ; EMIF Interrupt
I2C:            .ivec    dummy_isr             ; IIC interrupt
BERRIV:
IV24:           .ivec    dummy_isr             ; Bus error interrupt
DLOGIV:
IV25:           .ivec    dummy_isr             ; Data log (RTDX) interrupt
RTOSIV:
IV26:           .ivec    dummy_isr             ; Real-time OS interrupt
IV27:           .ivec    dummy_isr             ; General-purpose software-only interrupt
IV28:           .ivec    dummy_isr             ; General-purpose software-only interrupt
IV29:           .ivec    dummy_isr             ; General-purpose software-only interrupt
IV30:           .ivec    dummy_isr             ; General-purpose software-only interrupt
IV31:           .ivec    dummy_isr             ; General-purpose software-only interrupt

            .text           ; El código siguiente se incluye en la seccion .text
dummy_isr   B dummy_isr     ; Bucle infinito, bloquea el DSP en caso de error
                            ;   en las interrpciones

.end
