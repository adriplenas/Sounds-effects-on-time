/******************************************************************************/
/* LNKX.CMD - COMMAND FILE FOR LINKING C PROGRAMS IN LARGE/HUGE MEMORY MODEL  */
/*                                                                            */
/* Usage:                                                                     */
/*  cl55 <src files> -z -o<out file> -m<map file> lnkx.cmd -l<RTS library>   */
/*                                                                            */
/* Description: This file is a sample command file that can be used for       */
/*              linking programs built with the C Compiler.  Use it as a      */
/*              guideline; you  may want to change the allocation scheme      */
/*              according to the size of your program and the memory layout   */
/*              of your target system.                                        */
/*                                                                            */
/*   Notes: (1) You must specify the directory in which <RTS library> is      */
/*              located.  Either add a "-i<directory>" line to this file      */
/*              file, or use the system environment variable C55X_C_DIR to    */
/*              specify a search path for the libraries.                      */
/*                                                                            */
/******************************************************************************/

-stack    0x2000      /* Primary stack size   */
-sysstack 0x1000      /* Secondary stack size */
-heap     0x2000      /* Heap area size       */

-c                    /* Use C linking conventions: auto-init vars at runtime */
-u _Reset             /* Force load of reset interrupt handler                */

/* SPECIFY THE SYSTEM MEMORY MAP */

MEMORY
{
 PAGE 0:  /* ---- Unified Program/Data Address Space ---- */

  MMR    (RWIX): origin = 0x000000, length = 0x0000c0  /* MMRs */
  DARAM0 (RWIX): origin = 0x0000c0, length = 0x00ff40  /*  64KB - MMRs */
  SARAM0 (RWIX): origin = 0x010000, length = 0x010000  /*  64KB */
  SARAM1 (RWIX): origin = 0x020000, length = 0x020000  /* 128KB */
  // SARAM2 (RWIX): origin = 0x040000, length = 0x00FE00  /*  64KB */
  SARAM_24 (RWIX) : origin = 0040000h length = 002000h  /* on-chip SARAM 24 */
  SARAM_25 (RWIX) : origin = 0042000h length = 002000h  /* on-chip SARAM 25 */
  SARAM_26 (RWIX) : origin = 0044000h length = 002000h  /* on-chip SARAM 26 */
  SARAM_27 (RWIX) : origin = 0046000h length = 002000h  /* on-chip SARAM 27 */
  SARAM_28 (RWIX) : origin = 0048000h length = 002000h  /* on-chip SARAM 28 */
  SARAM_29 (RWIX) : origin = 004A000h length = 002000h  /* on-chip SARAM 29 */
  SARAM_30 (RWIX) : origin = 004C000h length = 001E00h  /* on-chip SARAM 30 */

  //SARAM_31 (RWIX) : origin = 004E000h length = 002000h  /* on-chip SARAM 31 */ NO LO USAMOS PORQUE ES LA REGIÓN DEL BOOTLOADER

  VECS   (RWIX): origin = 0x04DE00, length = 0x000200  /*  512B */
  PDROM   (RIX): origin = 0xff8000, length = 0x008000  /*  32KB */

 PAGE 2:  /* -------- 64K-word I/O Address Space -------- */

  IOPORT (RWI) : origin = 0x000000, length = 0x020000
}

/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */

SECTIONS
{
   .text     >> SARAM1|SARAM0  /* Code                        */

   /* Both stacks must be on same physical memory page               */
   .stack    >  DARAM0                /* Primary system stack        */
   .sysstack >  DARAM0                /* Secondary system stack      */

   .data     >  DARAM0|SARAM0|SARAM1  /* Initialized vars            */
   .bss      >  DARAM0|SARAM0|SARAM1  /* Global & static vars        */
   .const    >  DARAM0|SARAM0|SARAM1  /* Constant data               */
   .sysmem   >  DARAM0|SARAM0|SARAM1  /* Dynamic memory (malloc)     */
   .switch   >  SARAM_30                /* Switch statement tables     */
   .cinit    >  SARAM_30|SARAM_29               /* Auto-initialization tables  */
   .pinit    >  SARAM_30|SARAM_29               /* Initialization fn tables    */
   .cio      >  SARAM_30|SARAM_29               /* C I/O buffers               */
   .args     >  SARAM_30|SARAM_29               /* Arguments to main()         */

    vectors   >  SARAM_28                  /* Interrupt vectors           */

   .ioport   >  IOPORT PAGE 2         /* Global & static ioport vars */
}
