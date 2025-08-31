#pragma once

#include "my_optional.h"
#include <functional>

template <class T>
using optional_ref = opt::optional<std::reference_wrapper<T>>;
