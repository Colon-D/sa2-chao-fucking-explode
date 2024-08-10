#include "config.h"
#include "globals.h"
#include <iostream>

void config::load(const std::filesystem::path& path) {
	IniFile ini_file{ path / "config.ini" };
	if (const auto* const main_ini = ini_file.getGroup("Main")) {
		permadeath = main_ini->getBool("Permadeath");
		if (permadeath) {
			std::cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n";
			std::cout << "| Permadeath is enabled. Your chao WILL die. |\n";
			std::cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n";
		}
		play_jingle = main_ini->getBool("Play Jingle");
		chain_explosions = main_ini->getBool("Chain Explosions");
	}
	if (const auto* const nearby_chao_ini = ini_file.getGroup("Nearby Chao")) {
		nearby_chao.happiness = nearby_chao_ini->getFloat("Happiness");
		nearby_chao.char_like = nearby_chao_ini->getFloat("Character Like");
		nearby_chao.char_fear = nearby_chao_ini->getFloat("Character Fear");
		nearby_chao.anger = nearby_chao_ini->getFloat("Anger");
		nearby_chao.boredom = nearby_chao_ini->getFloat("Boredom");
		nearby_chao.fear = nearby_chao_ini->getFloat("Fear");
		nearby_chao.joy = nearby_chao_ini->getFloat("Joy");
		nearby_chao.mate_desire = nearby_chao_ini->getFloat("Mate Desire");
		nearby_chao.relax = nearby_chao_ini->getFloat("Relax");
		nearby_chao.surprise = nearby_chao_ini->getFloat("Surprise");
		nearby_chao.urge_to_cry = nearby_chao_ini->getFloat("Urge to Cry");
		nearby_chao.energy = nearby_chao_ini->getFloat("Energy");
	}
	if (const auto* const hit_chao_ini = ini_file.getGroup("Hit Chao")) {
		hit_chao.dizziness = hit_chao_ini->getFloat("Dizziness");
	}
	if (
		const auto* const exp_src_ini = ini_file.getGroup("Explosion Sources")
	) {
		explosion_sources[chao_behaviour::trip].load(exp_src_ini, "Trip");
		explosion_sources[chao_behaviour::thrown].load(exp_src_ini, "Thrown");
		explosion_sources[chao_behaviour::damaged].load(
			exp_src_ini, "Damaged"
		);
		explosion_sources[chao_behaviour::thrown_birth].load(
			exp_src_ini, "Birth: Thrown"
		);
		explosion_sources[chao_behaviour::come_out_egg].load(
			exp_src_ini, "Birth: Natural"
		);
		explosion_sources[chao_behaviour::jump_scared].load(
			exp_src_ini, "Jump Scared"
		);
		explosion_sources[chao_behaviour::dizzy].load(exp_src_ini, "Dizzy");
		explosion_sources[chao_behaviour::cough].load(exp_src_ini, "Cough");
		explosion_sources[chao_behaviour::cold].load(exp_src_ini, "Cold");
		explosion_sources[chao_behaviour::hiccups].load(
			exp_src_ini, "Hiccups"
		);
		explosion_sources[chao_behaviour::stomach_ache].load(
			exp_src_ini, "Stomach Ache"
		);
		explosion_sources[chao_behaviour::thrown_into_departure_machine].load(
			exp_src_ini, "Departure Machine"
		);
		explosion_sources[chao_behaviour::bounce].load(
			exp_src_ini, "Bounce from Jump"
		);
		bounce_from_jump_height_multiplier =
			exp_src_ini->getFloat("Bounce from Jump - Height Multiplier");
		explosion_sources[chao_behaviour::cocoon_reincarnate_or_die].load(
			exp_src_ini, "Cocoon: Death"
		);
		explosion_sources[chao_behaviour::trip_in_race].load(
			exp_src_ini, "Race: Trip"
		);
		explosion_sources[chao_behaviour::failed_puzzle].load(
			exp_src_ini, "Race: Failed Puzzle"
		);
		explosion_sources[chao_behaviour::pitfall_or_jump_scared].load(
			exp_src_ini, "Race: Pitfall or Jump Scared"
		);
	}
}

void explosion_source::load(
	const IniGroup* const ini_group, const std::string_view key
) {
	const auto key_str = std::string{ key };
	chance = ini_group->getFloat(key_str);
	auto value = ini_group->getInt(key_str + " - Min Delay");
	time_range.begin = value == -1 ? -1u : static_cast<unsigned>(value);
	value = ini_group->getInt(key_str + " - Max Delay");
	time_range.end = value == -1 ? -1u : static_cast<unsigned>(value);
}
