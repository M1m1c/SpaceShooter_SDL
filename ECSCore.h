#pragma once
#include <cstdint>
#include <bitset>

using CompType = std::uint8_t;

const CompType MAX_COMPONENTS = 32;

using CompSignature = std::bitset<MAX_COMPONENTS>;

using Entity = std::uint32_t;

const Entity MAX_ENTITIES = 10000;