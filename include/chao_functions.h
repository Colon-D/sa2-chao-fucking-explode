#pragma once
#include <SA2ModLoader.h>
#include "chao_types.h"

void elapse_time();

std::uint32_t* set_chao_behaviour(
	ObjectMaster* chao,
	chao_behaviour behaviour,
	const std::uint32_t a3 = 0xFFFFFFFF
);
