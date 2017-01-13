// resize
#pragma once

#include "stm32f4xx.h"
#include "stm32f4xx_dma2d.h"

#define CR_MASK ((uint32_t)0xFFFCE0FC)  /* DMA2D CR Mask */
//{{{  enum D2D_Stage_Typedef
typedef enum {
  D2D_STAGE_IDLE = 0,
  D2D_STAGE_FIRST_LOOP = 1,
  D2D_STAGE_2ND_LOOP = 2,
  D2D_STAGE_DONE = 3,
  D2D_STAGE_ERROR = 4,
  D2D_STAGE_SETUP_BUSY = 5,
  D2D_STAGE_SETUP_DONE = 6
  }D2D_Stage_Typedef;
//}}}
//{{{  struct RESIZE_InitTypedef
// The work buffer area (in the ARGB_8888 format for alignment purpose) must have at
// least ( SourceWidth * OutputHeight ) word avaliable.
// The source bitmap can be in any DMA2D input color format, the alfa channel of related
// color mode is currently ignored. CM_L4 bitmap parameter must be byte aligned.
typedef struct {
  void*    SourceBaseAddress; /* source bitmap Base Address */
  uint16_t SourcePitch;       /* source pixel pitch */
  uint16_t SourceColorMode;   /* source color mode */
  uint16_t SourceX;           /* souce X */
  uint16_t SourceY;           /* sourceY */
  uint16_t SourceWidth;       /* source width */
  uint16_t SourceHeight;      /* source height */

  void*    OutputBaseAddress; /* output bitmap Base Address */
  uint16_t OutputPitch;       /* output pixel pitch */
  uint16_t OutputColorMode;   /* output color mode */
  uint16_t OutputX;           /* output X */
  uint16_t OutputY;           /* output Y */
  uint16_t OutputWidth;       /* output width */
  uint16_t OutputHeight;      /* output height */

  uint32_t* WorkBuffer;       /* storage buffer */
  } RESIZE_InitTypedef;
//}}}
//{{{  struct D2D_Loop_Typedef
typedef struct {
  uint32_t Counter;           /* Loop Counter */
  uint32_t BaseAddress;       /* Loop Base Address */
  uint32_t BlendIndex;        /* Loop Blend index (Q21) */
  uint32_t BlendCoeff;        /* Loop Blend coefficient (Q21) */
  uint16_t SourcePitchBytes;  /* Loop Source pitch bytes */
  uint16_t OutputPitchBytes;  /* Loop Output pitch bytes */
  } D2D_Loop_Typedef;
//}}}
//{{{  struct D2D_Misc_Param
struct {
  uint32_t OutputBaseAddress; /* output bitmap Base Address */
  uint16_t OutputColorMode;   /* output color mode */
  uint16_t OutputHeight;      /* output height */
  uint16_t OutputPitch;       /* output pixel pitch */
  uint16_t SourceWidth;       /* source width */
  } D2D_Misc_Param;
//}}}

//{{{
void D2D_Resize_Setup (RESIZE_InitTypedef* R) {

  const uint16_t BitsPerPixel[6]= {32,24,16,16,16,8};

  // DMA2D operation mode
  DMA2D->CR &= (uint32_t)CR_MASK;
  DMA2D->CR |= DMA2D_M2M_BLEND;

  // DMA2D operation mode
  DMA2D_ITConfig(DMA2D_IT_TC | DMA2D_IT_TE | DMA2D_IT_CE , ENABLE);

  // first loop parameter init
  uint16_t PixelBytes = BitsPerPixel[R->SourceColorMode] >> 3;
  uint16_t PitchBytes = R->SourcePitch * PixelBytes;
  uint32_t BaseAddress = (uint32_t)R->SourceBaseAddress + R->SourceY * PitchBytes + R->SourceX * PixelBytes;

  D2D_Loop_Typedef D2D_First_Loop;
  D2D_First_Loop.Counter = R->OutputHeight;
  D2D_First_Loop.SourcePitchBytes = PitchBytes;
  D2D_First_Loop.OutputPitchBytes = R->SourceWidth<<2;
  D2D_First_Loop.BaseAddress = BaseAddress;
  D2D_First_Loop.BlendCoeff = ((R->SourceHeight-1)<<21) / R->OutputHeight;
  D2D_First_Loop.BlendIndex = D2D_First_Loop.BlendCoeff>>1;

  DMA2D->FGPFCCR = (REPLACE_ALPHA_VALUE<<16) | R->SourceColorMode;
  DMA2D->BGPFCCR = (REPLACE_ALPHA_VALUE<<16) | R->SourceColorMode | 0xff000000;
  DMA2D->OPFCCR = DMA2D_ARGB8888;
  DMA2D->NLR = (1 | (R->SourceWidth<<16));
  DMA2D->OMAR = (uint32_t)R->WorkBuffer;

  // 2nd loop parameter init
  PixelBytes = BitsPerPixel[R->OutputColorMode] >> 3;
  PitchBytes = R->OutputPitch * PixelBytes;
  BaseAddress = (uint32_t)R->OutputBaseAddress + R->OutputY * PitchBytes + R->OutputX * PixelBytes;

  D2D_Loop_Typedef  D2D_2nd_Loop;
  D2D_2nd_Loop.Counter = R->OutputWidth;
  D2D_2nd_Loop.SourcePitchBytes = 4;
  D2D_2nd_Loop.OutputPitchBytes = PixelBytes;
  D2D_2nd_Loop.BaseAddress = (uint32_t)R->WorkBuffer;
  D2D_2nd_Loop.BlendCoeff = ((R->SourceWidth-1)<<21) / R->OutputWidth;
  D2D_2nd_Loop.BlendIndex = D2D_2nd_Loop.BlendCoeff>>1;

  D2D_Misc_Param.OutputBaseAddress= BaseAddress;
  D2D_Misc_Param.OutputColorMode = R->OutputColorMode;
  D2D_Misc_Param.OutputHeight = R->OutputHeight;
  D2D_Misc_Param.OutputPitch = R->OutputPitch;
  D2D_Misc_Param.SourceWidth = R->SourceWidth;

  // start first loop stage
  D2D_Loop_Typedef* D2D_Loop = &D2D_First_Loop;
  D2D_Stage_Typedef D2D_Loop_Stage = D2D_STAGE_FIRST_LOOP;

  // calculate and setup address for first and 2nd lines */
  uint32_tFirstLine = D2D_Loop->BlendIndex >> 21;
  DMA2D->BGMAR = D2D_Loop->BaseAddress + FirstLine * D2D_Loop->SourcePitchBytes;
  DMA2D->FGMAR = DMA2D->BGMAR + D2D_Loop->SourcePitchBytes;

  // 8 MSB of fractional part as FG alpha (Blend factor)
  uint32_tFGalpha = D2D_Loop->BlendIndex>>13;
  DMA2D->FGPFCCR &= 0x00ffffff;
  DMA2D->FGPFCCR |= (FGalpha << 24);

  // start DMA2D transfer
  DMA2D_StartTransfer();
  // wait;

  while (true) {
    if (--D2D_Loop->Counter) {
      //{{{  loop lines
      /* Update output memory address */
      DMA2D->OMAR += D2D_Loop->OutputPitchBytes;
      /* Add BlenCoeff to BlendIndex */
      D2D_Loop->BlendIndex += D2D_Loop->BlendCoeff;

      /* calculate and setup address for first and 2nd lines */
      uint32_tFirstLine = D2D_Loop->BlendIndex >> 21;
      DMA2D->BGMAR = D2D_Loop->BaseAddress + FirstLine * D2D_Loop->SourcePitchBytes;
      DMA2D->FGMAR = DMA2D->BGMAR + D2D_Loop->SourcePitchBytes;

      /* 8 MSB of fractional part as FG alpha (Blend factor) */
      uint32_tFGalpha = D2D_Loop->BlendIndex>>13;
      DMA2D->FGPFCCR &= 0x00ffffff;
      DMA2D->FGPFCCR |= (FGalpha << 24);

      /* restart DMA2D transfer*/
      DMA2D_StartTransfer();
      // wait;
      }
      //}}}
    else if (D2D_Loop_Stage == D2D_STAGE_FIRST_LOOP) {
      //{{{  start 2nd loop stage
      D2D_Loop = &D2D_2nd_Loop;
      D2D_Loop_Stage = D2D_STAGE_2ND_LOOP;

      /* setup DMA2D register */
      DMA2D->FGPFCCR = (REPLACE_ALPHA_VALUE << 16) | CM_ARGB8888;
      DMA2D->BGPFCCR = (REPLACE_ALPHA_VALUE << 16) | CM_ARGB8888 | 0xff000000;
      DMA2D->OPFCCR  = D2D_Misc_Param.OutputColorMode;
      DMA2D->NLR     = (D2D_Misc_Param.OutputHeight | (1 << 16));
      DMA2D->OOR     = D2D_Misc_Param.OutputPitch - 1;
      DMA2D->FGOR    = D2D_Misc_Param.SourceWidth - 1;
      DMA2D->BGOR    = D2D_Misc_Param.SourceWidth - 1;
      DMA2D->OMAR    = D2D_Misc_Param.OutputBaseAddress;

      /* calculate and setup address for first and 2nd lines */
      uint32_tFirstLine = D2D_Loop->BlendIndex >> 21;
      DMA2D->BGMAR = D2D_Loop->BaseAddress + FirstLine * D2D_Loop->SourcePitchBytes;
      DMA2D->FGMAR = DMA2D->BGMAR + D2D_Loop->SourcePitchBytes;

      /* 8 MSB of fractional part as FG alpha (Blend factor) */
      uint32_tFGalpha = D2D_Loop->BlendIndex>>13;
      DMA2D->FGPFCCR &= 0x00ffffff;
      DMA2D->FGPFCCR |= (FGalpha << 24);

      /* restart DMA2D transfer*/
      DMA2D_StartTransfer();
      // wait;
      }
      //}}}
    else
      break;
    }
  }
//}}}

#define Source_BMP (uint16_t*)(0x08100010) /* skip 16 byte header */
#define WorkBuffer (uint32_t*)(0xD0100000)
#define BG_Framebuffer (uint16_t*)LCD_FRAME_BUFFER

RESIZE_InitTypedef Resize = {
  Source_BMP,     /* source bitmap Base Address */
  480,            /* source pixel pitch */
  CM_RGB565,      /* source color mode */
  0,              /* souce X */
  0,              /* sourceY */
  480,            /* source width */
  640,            /* source height */
  BG_Framebuffer, /* output bitmap Base Address */
  240,            /* output pixel pitch */
  DMA2D_RGB565,   /* output color mode */
  0,              /* output X */
  0,              /* output Y */
  240,            /* output width */
  320,            /* output height */
  WorkBuffer      /* storage buffer */
};

D2D_Resize_Setup (&Resize);
