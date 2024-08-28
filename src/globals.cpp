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

std::unordered_map<chao_behaviour, std::string_view> behaviour_map{
	{ chao_behaviour::race_wait, "race wait" },
	{ chao_behaviour::sleep, "sleep" },
	{ chao_behaviour::getting_tired, "getting tired" },
	{ chao_behaviour::tired, "tired" },
	{ chao_behaviour::move, "move" },
	{ chao_behaviour::tire, "tire" },
	{ chao_behaviour::broom_stick, "broom stick" },
	{ chao_behaviour::pogo_stick, "pogo stick" },
	{ chao_behaviour::handing, "handing" },
	{ chao_behaviour::eating, "eating" },
	{ chao_behaviour::pick_up_fruit, "pick up fruit" },
	{ chao_behaviour::swim, "swim" },
	{ chao_behaviour::super_swim, "super swim" },
	{ chao_behaviour::swim_idle, "swim idle" },
	{ chao_behaviour::trip, "trip" },
	{ chao_behaviour::climb, "climb" },
	{ chao_behaviour::fly, "fly" },
	{ chao_behaviour::go_to_player, "go to player" },
	{ chao_behaviour::being_called, "being called" },
	{ chao_behaviour::go_to_ball, "go to ball" },
	{ chao_behaviour::damaged, "damaged" },
	{ chao_behaviour::thrown, "thrown" },
	{ chao_behaviour::jump_scared, "jump scared" },
	{ chao_behaviour::get_pet, "get pet" },
	{ chao_behaviour::think, "think" },
	{ chao_behaviour::got_animal, "got animal" },
	{ chao_behaviour::think_seed, "think seed" },
	{ chao_behaviour::walk_with_seed, "walk with seed" },
	{ chao_behaviour::found_seed_spot, "found seed spot" },
	{ chao_behaviour::water_can, "water can" },
	{ chao_behaviour::water_can_dance, "water can dance" },
	{ chao_behaviour::holding_item, "holding item" },
	{ chao_behaviour::notice_item, "notice item" },
	{ chao_behaviour::put_hat, "put hat" },
	{ chao_behaviour::throw_item, "throw item" },
	{ chao_behaviour::go_to_item, "go to item" },
	{ chao_behaviour::watching_tv, "watching tv" },
	{ chao_behaviour::watching_radio, "watching radio" },
	{ chao_behaviour::watching_sing, "watching sing" },
	{ chao_behaviour::mate_walk, "mate walk" },
	{ chao_behaviour::watching_dance, "watching dance" },
	{ chao_behaviour::maracas, "maracas" },
	{ chao_behaviour::flute, "flute" },
	{ chao_behaviour::trumpet, "trumpet" },
	{ chao_behaviour::watching_music, "watching music" },
	{ chao_behaviour::want_the_hold_fruit, "want the hold fruit" },
	{ chao_behaviour::go_to_hold_fruit, "go to hold fruit" },
	{ chao_behaviour::hungry, "hungry" },
	{ chao_behaviour::starving, "starving" },
	{ chao_behaviour::fear, "fear" },
	{ chao_behaviour::sadness, "sadness" },
	{ chao_behaviour::urge_to_cry, "urge to cry" },
	{ chao_behaviour::urge_to_cry2, "urge to cry2" },
	{ chao_behaviour::dizzy, "dizzy" },
	{ chao_behaviour::hero_hi, "hero hi" },
	{ chao_behaviour::love_the_player, "love the player" },
	{ chao_behaviour::come_to_player, "come to player" },
	{ chao_behaviour::jump, "jump" },
	{ chao_behaviour::anger, "anger" },
	{ chao_behaviour::chao_hate, "chao hate" },
	{ chao_behaviour::sit_think, "sit think" },
	{ chao_behaviour::unicorn, "unicorn" },
	{ chao_behaviour::gorilla, "gorilla" },
	{ chao_behaviour::wart_hog, "wart hog" },
	{ chao_behaviour::bunny, "bunny" },
	{ chao_behaviour::crawl, "crawl" },
	{ chao_behaviour::walk, "walk" },
	{ chao_behaviour::run1, "run1" },
	{ chao_behaviour::run2, "run2" },
	{ chao_behaviour::drawing, "drawing" },
	{ chao_behaviour::come_out_egg, "come out egg" },
	{ chao_behaviour::thrown_birth, "thrown birth" },
	{ chao_behaviour::cocoon_reincarnate_or_die, "cocoon reincarnate or die" },
	{ chao_behaviour::cocoon_evolve, "cocoon evolve" },
	{ chao_behaviour::thrown_into_departure_machine, "thrown into departure machine" },
	{ chao_behaviour::trip_in_race, "trip in race" },
	{ chao_behaviour::jack_n_box_or_begin_puzzle_race, "jack n box or begin puzzle race" },
	{ chao_behaviour::pitfall_or_jump_scared, "pitfall or jump scared" },
	{ chao_behaviour::succeeded_puzzle, "succeeded puzzle" },
	{ chao_behaviour::failed_puzzle, "failed puzzle" },
	{ chao_behaviour::failed_puzzle_bonk, "failed puzzle bonk" },
	{ chao_behaviour::jack_n_box1, "jack n box1" },
	{ chao_behaviour::jack_n_box2, "jack n box2" },
	{ chao_behaviour::cough, "cough" },
	{ chao_behaviour::cold, "cold" },
	{ chao_behaviour::rash, "rash" },
	{ chao_behaviour::running_nose, "running nose" },
	{ chao_behaviour::hiccups, "hiccups" },
	{ chao_behaviour::stomach_ache, "stomach ache" },
	{ chao_behaviour::land_in_water, "land in water" },
	{ chao_behaviour::bounce, "bounce" }
};
