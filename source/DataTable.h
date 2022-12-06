#pragma once
#include <tuple>
#include <array>
#include <variant>
#include "ECSCore.h"

template<size_t size, typename... Types>
using DataTable = std::vector<std::tuple<Types...>>;
