// resize
#pragma once
//{{{  struct RESIZE_InitTypedef
// The work buffer area (in the ARGB_8888 format for alignment purpose) must have at
// least ( SrcWidth * OutHeight ) word avaliable.
// The Src bitmap can be in any DMA2D input color format, the alfa channel of related
// color mode is currently ignored. CM_L4 bitmap parameter must be byte aligned.
typedef struct {
  void*    SrcBaseAddress; /* Src bitmap Base Address */
  uint16_t SrcPitch;       /* Src pixel pitch */
  uint16_t SrcFormat;      /* Src color mode */
  uint16_t SrcX;           /* souce X */
  uint16_t SrcY;           /* SrcY */
  uint16_t SrcWidth;       /* Src width */
  uint16_t SrcHeight;      /* Src height */

  void*    DstBaseAddress; /* Dst bitmap Base Address */
  uint16_t DstPitch;       /* Dst pixel pitch */
  uint16_t DstFormat;      /* Dst color mode */
  uint16_t DstX;           /* Dst X */
  uint16_t DstY;           /* Dst Y */
  uint16_t DstWidth;       /* Dst width */
  uint16_t DstHeight;      /* Dst height */
  } RESIZE_InitTypedef;
//}}}
//{{{  struct D2D_Loop_Typedef
typedef struct {
  uint32_t BaseAddress;
  uint32_t BlendIndex;  // loop Blend index (Q21)
  uint32_t BlendCoeff;  // loop Blend coefficient (Q21)
  uint16_t SrcPitch;
  uint16_t DstPitch;
  } D2D_Loop_Typedef;
//}}}

void resize (RESIZE_InitTypedef* R) {

  const uint16_t kFormatBytes[6] = {4,3,2,2,2,1};
  //const uint32_t workFormat = DMA2D_ARGB8888;
  const uint32_t workFormat = DMA2D_RGB565;
  uint32_t* workBuf = (uint32_t*)pvPortMalloc (R->SrcWidth * R->DstHeight * kFormatBytes[workFormat]);

  // first loop paramaeters
  uint32_t BaseAddress = (uint32_t)R->SrcBaseAddress + ((R->SrcY * R->SrcPitch) + R->SrcX) * kFormatBytes[R->SrcFormat];
  uint16_t SrcPitch = R->SrcPitch * kFormatBytes[R->SrcFormat];
  uint16_t DstPitch = R->SrcWidth * kFormatBytes[workFormat];
  uint32_t BlendCoeff = ((R->SrcHeight-1) << 21) / R->DstHeight;
  uint32_t BlendIndex = BlendCoeff >> 1;
  //{{{  first loop
  DMA2D->OPFCCR = workFormat;
  DMA2D->FGPFCCR = R->SrcFormat;
  DMA2D->BGPFCCR = 0xff000000 | R->SrcFormat;
  DMA2D->NLR = (1 | (R->SrcWidth << 16));

  // bgnd = line, fgnd = line + 1
  uint32_t dstPtr = (uint32_t)workBuf;
  DMA2D->OMAR = dstPtr;
  DMA2D->BGMAR = BaseAddress + ((BlendIndex >> 21) * SrcPitch);
  DMA2D->FGMAR = DMA2D->BGMAR + SrcPitch;
  DMA2D->FGPFCCR &= 0x00ffffff;
  DMA2D->FGPFCCR |= ((BlendIndex >> 13) << 24);
  DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
  dstPtr += DstPitch;
  BlendIndex += BlendCoeff;

  while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
  DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                 DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
  //}}}
  auto loop = R->DstHeight;
  while (loop--) {
    //{{{  loop lines, src->work
    DMA2D->OMAR = dstPtr;
    DMA2D->BGMAR = BaseAddress + (BlendIndex >> 21) * SrcPitch;
    DMA2D->FGMAR = DMA2D->BGMAR + SrcPitch;
    DMA2D->FGPFCCR &= 0x00ffffff;
    DMA2D->FGPFCCR |= ((BlendIndex >> 13) << 24);
    DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
    dstPtr += DstPitch;
    BlendIndex += BlendCoeff;

    while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
    DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                   DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
    }
    //}}}

  // 2nd loop parameters
  BaseAddress = (uint32_t)workBuf;
  SrcPitch = kFormatBytes[workFormat];
  DstPitch = kFormatBytes[R->DstFormat];
  BlendCoeff = ((R->SrcWidth-1) << 21) / R->DstWidth;
  BlendIndex = BlendCoeff >> 1;
  //{{{  second loop
  DMA2D->FGPFCCR = workFormat;
  DMA2D->BGPFCCR = 0xff000000 | workFormat;
  DMA2D->OPFCCR  = R->DstFormat;
  DMA2D->NLR = R->DstHeight | (1 << 16);

  DMA2D->OOR = R->DstPitch - 1;
  DMA2D->FGOR = R->SrcWidth - 1;
  DMA2D->BGOR = R->SrcWidth - 1;

  dstPtr = (uint32_t)R->DstBaseAddress + ((R->DstY * R->DstPitch) + R->DstX) * kFormatBytes[R->DstFormat];
  DMA2D->OMAR = dstPtr;
  DMA2D->BGMAR = BaseAddress + (BlendIndex >> 21) * SrcPitch;
  DMA2D->FGMAR = DMA2D->BGMAR + SrcPitch;
  DMA2D->FGPFCCR &= 0x00ffffff;
  DMA2D->FGPFCCR |= ((BlendIndex >> 13) << 24);

  DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
  dstPtr += DstPitch;
  BlendIndex += BlendCoeff;

  while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
  DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                 DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
  //}}}
  loop = R->DstWidth;
  while (loop--) {
    //{{{  loop columns, work->Dst
    DMA2D->OMAR = dstPtr;
    DMA2D->BGMAR = BaseAddress + (BlendIndex >> 21) * SrcPitch;
    DMA2D->FGMAR = DMA2D->BGMAR + SrcPitch;
    DMA2D->FGPFCCR &= 0x00ffffff;
    DMA2D->FGPFCCR |= ((BlendIndex >> 13) << 24);
    DMA2D->CR = DMA2D_M2M_BLEND | DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE | DMA2D_CR_START;
    dstPtr += DstPitch;
    BlendIndex += BlendCoeff;

    while (!(DMA2D->ISR & DMA2D_FLAG_TC)) {}
    DMA2D->IFCR |= DMA2D_IFSR_CTEIF | DMA2D_IFSR_CTCIF | DMA2D_IFSR_CTWIF|
                   DMA2D_IFSR_CCAEIF | DMA2D_IFSR_CCTCIF | DMA2D_IFSR_CCEIF;
    }
    //}}}

  vPortFree (workBuf);
  }
