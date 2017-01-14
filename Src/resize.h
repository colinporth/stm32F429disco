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
  uint16_t SourcePitch;  /* Loop Source pitch bytes */
  uint16_t OutputPitch;  /* Loop Output pitch bytes */
  } D2D_Loop_Typedef;
//}}}

void resize (RESIZE_InitTypedef* R) {

  const uint16_t kBytesPerPixel[6] = {4,3,2,2,2,1};
  //const uint32_t workFormat = DMA2D_ARGB8888;
  const uint32_t workFormat = DMA2D_RGB565;
  const uint32_t workFormatBytesPerPixel = kBytesPerPixel[workFormat];

  // first loop paramaeters
  uint32_t BaseAddress = (uint32_t)R->SourceBaseAddress +
                         ((R->SourceY * R->SourcePitch) + R->SourceX) * kBytesPerPixel[R->SourceColorMode];

  D2D_Loop_Typedef d2dLoop;
  d2dLoop.Counter = R->OutputHeight;
  d2dLoop.BaseAddress = BaseAddress;
  d2dLoop.SourcePitch = R->SourcePitch * kBytesPerPixel[R->SourceColorMode];
  d2dLoop.OutputPitch = R->SourceWidth * kBytesPerPixel[workFormat];
  d2dLoop.BlendCoeff = ((R->SourceHeight-1) << 21) / R->OutputHeight;
  d2dLoop.BlendIndex = d2dLoop.BlendCoeff >> 1;
  //{{{  start first loop
  DMA2D->OPFCCR = workFormat;
  DMA2D->NLR = (1 | (R->SourceWidth << 16));
  DMA2D->FGPFCCR = (REPLACE_ALPHA_VALUE << 16) | R->SourceColorMode;
  DMA2D->BGPFCCR = 0xff000000 | (REPLACE_ALPHA_VALUE << 16) | R->SourceColorMode;

  // bgnd = line, fgnd = line + 1
  DMA2D->OMAR = (uint32_t)R->WorkBuffer;
  DMA2D->BGMAR = d2dLoop.BaseAddress + (d2dLoop.BlendIndex >> 21) * d2dLoop.SourcePitch;
  DMA2D->FGMAR = DMA2D->BGMAR + d2dLoop.SourcePitch;
  DMA2D->FGPFCCR &= 0x00ffffff;
  DMA2D->FGPFCCR |= ((d2dLoop.BlendIndex >> 13) << 24);
  DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
  d2dLoop.BlendIndex += d2dLoop.BlendCoeff;

  while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
  DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                 DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
  //}}}
  while (--d2dLoop.Counter) {
    //{{{  loop lines
    DMA2D->OMAR += d2dLoop.OutputPitch;
    DMA2D->BGMAR = d2dLoop.BaseAddress + (d2dLoop.BlendIndex >> 21) * d2dLoop.SourcePitch;
    DMA2D->FGMAR = DMA2D->BGMAR + d2dLoop.SourcePitch;
    DMA2D->FGPFCCR &= 0x00ffffff;
    DMA2D->FGPFCCR |= ((d2dLoop.BlendIndex >> 13) << 24);
    DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
    d2dLoop.BlendIndex += d2dLoop.BlendCoeff;

    while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
    DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                   DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
    }
    //}}}

  // 2nd loop parameters
  BaseAddress = (uint32_t)R->OutputBaseAddress +
               ((R->OutputY * R->OutputPitch) + R->OutputX) * kBytesPerPixel[R->OutputColorMode];

  d2dLoop.Counter = R->OutputWidth;
  d2dLoop.BaseAddress = (uint32_t)R->WorkBuffer;
  d2dLoop.SourcePitch = kBytesPerPixel[workFormat];
  d2dLoop.OutputPitch = kBytesPerPixel[R->OutputColorMode];
  d2dLoop.BlendCoeff = ((R->SourceWidth-1) << 21) / R->OutputWidth;
  d2dLoop.BlendIndex = d2dLoop.BlendCoeff >> 1;
  //{{{  start 2nd loop
  DMA2D->FGPFCCR = (REPLACE_ALPHA_VALUE << 16) | workFormat;
  DMA2D->BGPFCCR = 0xff000000 | (REPLACE_ALPHA_VALUE << 16) | workFormat;
  DMA2D->OPFCCR  = R->OutputColorMode;
  DMA2D->NLR = R->OutputHeight | (1 << 16);
  DMA2D->OOR = R->OutputPitch - 1;
  DMA2D->FGOR = R->SourceWidth - 1;
  DMA2D->BGOR = R->SourceWidth - 1;
  DMA2D->OMAR = BaseAddress;
  DMA2D->BGMAR = d2dLoop.BaseAddress + (d2dLoop.BlendIndex >> 21) * d2dLoop.SourcePitch;
  DMA2D->FGMAR = DMA2D->BGMAR + d2dLoop.SourcePitch;
  DMA2D->FGPFCCR &= 0x00ffffff;
  DMA2D->FGPFCCR |= ((d2dLoop.BlendIndex >> 13) << 24);
  DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
  d2dLoop.BlendIndex += d2dLoop.BlendCoeff;

  while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
  DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                 DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
  //}}}
  while (--d2dLoop.Counter) {
    //{{{  loop lines
    DMA2D->OMAR += d2dLoop.OutputPitch;
    DMA2D->BGMAR = d2dLoop.BaseAddress + (d2dLoop.BlendIndex >> 21) * d2dLoop.SourcePitch;
    DMA2D->FGMAR = DMA2D->BGMAR + d2dLoop.SourcePitch;
    DMA2D->FGPFCCR &= 0x00ffffff;
    DMA2D->FGPFCCR |= ((d2dLoop.BlendIndex >> 13) << 24);
    DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
    d2dLoop.BlendIndex += d2dLoop.BlendCoeff;

    while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
    DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                   DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
    }
    //}}}
  }
