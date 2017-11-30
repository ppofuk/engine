#ifndef CORE_MAKE_FOUR_CC_INL_H_
#define CORE_MAKE_FOUR_CC_INL_H_

#define MAKE_FOUR_CC(ch0, ch1, ch2, ch3)                             \
  ((u32)(u8)(ch0) | ((u32)(u8)(ch1) << 8) | ((u32)(u8)(ch2) << 16) | \
   ((u32)(u8)(ch3) << 24))

#endif  // CORE_MAKE_FOUR_CC_INL_H_
