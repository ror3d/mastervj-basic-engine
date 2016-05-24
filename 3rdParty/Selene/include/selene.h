#pragma once

#if defined(_MSC_VER) && _MSC_VER < 1900 // before MSVS-14 CTP1
#define _constexpr const
#define _noexcept
#define _noexcept_(...)
#else
#define _constexpr constexpr


#define _noexcept noexcept
#define __noexcept(b) noexcept(b)
#endif

#include "selene/State.h"
#include "selene/Tuple.h"
