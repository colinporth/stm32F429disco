// resize
#pragma once
#define REPLACE_ALPHA_VALUE 0xFF
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

void resize (RESIZE_InitTypedef* R) {

  const uint16_t kBytesPerPixel[6]= {4,3,2,2,2,1};

  // first loop paramaeters
  uint32_t PitchBytes = R->SourcePitch * kBytesPerPixel[R->SourceColorMode];
  uint32_t BaseAddress = (uint32_t)R->SourceBaseAddress + R->SourceY * PitchBytes + R->SourceX * kBytesPerPixel[R->SourceColorMode];

  D2D_Loop_Typedef D2D_First_Loop;
  D2D_First_Loop.Counter = R->OutputHeight;
  D2D_First_Loop.BaseAddress = BaseAddress;
  D2D_First_Loop.SourcePitchBytes = PitchBytes;
  D2D_First_Loop.OutputPitchBytes = R->SourceWidth<<2;
  D2D_First_Loop.BlendCoeff = ((R->SourceHeight-1)<<21) / R->OutputHeight;
  D2D_First_Loop.BlendIndex  = D2D_First_Loop.BlendCoeff>>1;

  DMA2D->OPFCCR  = DMA2D_ARGB8888;
  DMA2D->OMAR = (uint32_t)R->WorkBuffer;
  DMA2D->NLR = (1 | (R->SourceWidth<<16));
  DMA2D->FGPFCCR = (REPLACE_ALPHA_VALUE<<16) | R->SourceColorMode;
  DMA2D->BGPFCCR = (REPLACE_ALPHA_VALUE<<16) | R->SourceColorMode | 0xff000000;

  // 2nd loop parameters
  PitchBytes = R->OutputPitch * kBytesPerPixel[R->SourceColorMode];
  BaseAddress = (uint32_t)R->OutputBaseAddress + R->OutputY * PitchBytes + R->OutputX * kBytesPerPixel[R->OutputColorMode];

  D2D_Loop_Typedef D2D_2nd_Loop;
  D2D_2nd_Loop.Counter = R->OutputWidth;
  D2D_2nd_Loop.BaseAddress = (uint32_t)R->WorkBuffer;
  D2D_2nd_Loop.SourcePitchBytes = 4;
  D2D_2nd_Loop.OutputPitchBytes = kBytesPerPixel[R->OutputColorMode];
  D2D_2nd_Loop.BlendCoeff = ((R->SourceWidth-1)<<21) / R->OutputWidth;
  D2D_2nd_Loop.BlendIndex = D2D_2nd_Loop.BlendCoeff>>1;

  D2D_Misc_Param.OutputBaseAddress = BaseAddress;
  D2D_Misc_Param.OutputColorMode = R->OutputColorMode;
  D2D_Misc_Param.OutputHeight = R->OutputHeight;
  D2D_Misc_Param.OutputPitch = R->OutputPitch;
  D2D_Misc_Param.SourceWidth = R->SourceWidth;

  // start first loop stage
  bool firstStage = true;
  D2D_Loop_Typedef* D2D_Loop = &D2D_First_Loop;

  // calculate and setup address for first and 2nd lines */
  DMA2D->BGMAR = D2D_Loop->BaseAddress + (D2D_Loop->BlendIndex >> 21) * D2D_Loop->SourcePitchBytes;
  DMA2D->FGMAR = DMA2D->BGMAR + D2D_Loop->SourcePitchBytes;

  DMA2D->FGPFCCR &= 0x00ffffff;
  DMA2D->FGPFCCR |= ((D2D_Loop->BlendIndex >> 13) << 24);

  DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
  while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
  DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                 DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;

  while (true) {
    if (--D2D_Loop->Counter) {
      //{{{  loop lines
      DMA2D->OMAR += D2D_Loop->OutputPitchBytes;
      D2D_Loop->BlendIndex += D2D_Loop->BlendCoeff;

      DMA2D->BGMAR = D2D_Loop->BaseAddress + (D2D_Loop->BlendIndex >> 21) * D2D_Loop->SourcePitchBytes;
      DMA2D->FGMAR = DMA2D->BGMAR + D2D_Loop->SourcePitchBytes;

      /* 8 MSB of fractional part as FG alpha (Blend factor) */
      DMA2D->FGPFCCR &= 0x00ffffff;
      DMA2D->FGPFCCR |= ((D2D_Loop->BlendIndex >> 13) << 24);

      DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
      while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
      DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                     DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
      }
      //}}}
    else if (firstStage) {
      //{{{  start 2nd loop stage
      firstStage = false;
      D2D_Loop = &D2D_2nd_Loop;

      DMA2D->FGPFCCR = (REPLACE_ALPHA_VALUE << 16) | CM_ARGB8888;
      DMA2D->BGPFCCR = (REPLACE_ALPHA_VALUE << 16) | CM_ARGB8888 | 0xff000000;
      DMA2D->OPFCCR  = D2D_Misc_Param.OutputColorMode;
      DMA2D->NLR = (D2D_Misc_Param.OutputHeight | (1 << 16));
      DMA2D->OOR = D2D_Misc_Param.OutputPitch - 1;
      DMA2D->FGOR = D2D_Misc_Param.SourceWidth - 1;
      DMA2D->BGOR = D2D_Misc_Param.SourceWidth - 1;
      DMA2D->OMAR = D2D_Misc_Param.OutputBaseAddress;

      DMA2D->BGMAR = D2D_Loop->BaseAddress + (D2D_Loop->BlendIndex >> 21) * D2D_Loop->SourcePitchBytes;
      DMA2D->FGMAR = DMA2D->BGMAR + D2D_Loop->SourcePitchBytes;

      DMA2D->FGPFCCR &= 0x00ffffff;
      DMA2D->FGPFCCR |= ((D2D_Loop->BlendIndex >> 13) << 24);

      DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
      while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
      DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                     DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
      }
      //}}}
    else
      return;
    }
  }
