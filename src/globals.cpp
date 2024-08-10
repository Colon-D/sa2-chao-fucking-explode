#include <SA2ModLoader.h>
#include <UsercallFunctionHandler.h>
#include <unordered_map>
#include "chao_types.h"
#include "config.h"

std::unordered_map<ObjectMaster*, struct chao_user_data> chao_user_data{};
struct config config{};

// presumably, chao bounce upwards a little before changing behaviour from
// jumping. This height is enough to explode a chao from the highest rock
// in the Neutral Garden down to infront of the waterfall.
constexpr float vertical_velocity_jump_threshold{ 0.6f };

// hardcoded extra conditions
std::unordered_map<chao_behaviour, explosion_source> explosion_sources{
	{
		chao_behaviour::bounce, // after bouncing from jump
		{
			.extra_condition = [&](auto& chao) {
				return
					(
						chao_user_data[&chao].prev_behaviour
						== chao_behaviour::jump
					)
					and (
						chao.EntityData2->vector_0.y
						> vertical_velocity_jump_threshold * config.bounce_from_jump_height_multiplier
					);
			}
		}
	},
	{
		chao_behaviour::cocoon_reincarnate_or_die,
		{
			.extra_condition = [&](auto& chao) {
				static const auto should_chao_reincarnate =
					GenerateUsercallWrapper<bool(*)(const ObjectMaster*)>(
						registers::rEAX,
						0x568F30,
						registers::rEAX
					);
				auto result = not should_chao_reincarnate(&chao);
				return result;
			}
		}
	}
};
