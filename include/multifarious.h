#ifndef MULTIFARIOUS_MULTIFARIOUS_H
#define MULTIFARIOUS_MULTIFARIOUS_H

#include <memory>
#include <new>
#include <tuple>
#include <type_traits>

#include "multifarious/macros.h"
#include "multifarious/type_traits.h"
#include "multifarious/util.h"
#include "multifarious/functional.h"

// Macros pollute the global namespace, so here's a toggle if you don't want to use them
#ifndef MULTIFARIOUS_CONFIG_EXPORT_MACROS
#undef MULTIFARIOUS_FWD
#undef MULTIFARIOUS_FWD_CAPTURE
#undef MULTIFARIOUS_LAMBDA_T
#endif

#endif// MULTIFARIOUS_MULTIFARIOUS_H
