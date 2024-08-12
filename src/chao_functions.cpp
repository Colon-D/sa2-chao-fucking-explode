#include "chao_functions.h"
#include "extra_math.h"
#include <unordered_map>
#include <random>
#include "globals.h"
#include <UsercallFunctionHandler.h>
#include <iostream>

// sonic <-> explosion radius is actually 28.f
constexpr float explosion_radius{ 24.f };

std::default_random_engine rng{};
std::uniform_real_distribution<float> percent{ 0.f, 100.f };
std::uniform_real_distribution<float> ratio{ 0.f, 1.f };

void erase_chao_in_departure_machine() {
	AL_GBAManagerExecutor_ptr->insertedChao = 0;
	AL_GBAManagerExecutor_ptr->ChaoDataPointer = nullptr;
}

void create_explosion(NJS_VECTOR position) {
	// don't play throwing sound effect
	const auto original_op = *reinterpret_cast<std::uint8_t*>(0x513CBF);
	constexpr std::uint8_t jmp_rel8{ 0xEB };
	WriteData(reinterpret_cast<std::uint8_t*>(0x513CBF), jmp_rel8);

	// replace explosion sound with one that is loaded.
	// I don't know how to load the original sound in the chao garden.
	const auto original_explosion_sound =
		*reinterpret_cast<std::uint32_t*>(0x512D75 + 0x1);
	constexpr std::uint32_t replacement_explosion_sound{ 0x8015 };
	WriteData(
		reinterpret_cast<std::uint32_t*>(0x512D75 + 0x1),
		replacement_explosion_sound
	);

	// spawn instantly exploding bomb
	constexpr int instantly_explode{ 3 };
	auto bomb = SpawnBomb(nullptr, &position, 0, instantly_explode);

	// restore original explosion sound
	WriteData(
		reinterpret_cast<std::uint32_t*>(0x512D75 + 0x1),
		original_explosion_sound
	);

	// restore original operation
	WriteData(reinterpret_cast<std::uint8_t*>(0x513CBF), original_op);
	
	// check to see if any other chao are in radius of explosion

	// chain reaction
	//   I think some mods let you have up to 24 chao in a garden?
	//   sub_530370 implies there can even be 64 per garden (not saved though)
	//   and 0x1DC0FC0 seems to be the actual beginning of the array...
	constexpr std::size_t chao_limit{ 64 };
	const auto* const chao_objects = &ChaoObjectArray;
	for (auto chao_index{ 0 }; chao_index < chao_limit; ++chao_index) {
		if (auto* const chao = chao_objects[chao_index * 0x12]) {
			// chao that can't be deleted (during race) instead have their
			// render function priveleges removed.
			// These chao should be considered dead.
			if (chao->DisplaySub == nullptr) {
				continue;
			}
			// player will automically drop chao if hurt
			for (auto& player : MainCharObj2) {
				if (player != nullptr && player->HeldObject == chao) {
					continue;
				}
			}
			const auto chao_pos = chao->Data1.Entity->Position;
			const auto delta = chao_pos - position;
			const auto dist = njScalor(&delta);

			// map dist with [0, explosion_radius] to [1, 0.8] and
			// [explosion_radius, 3 * explosion_radius] to [0.2, 0.0]
			const float explosion_ratio =
				dist < explosion_radius
				? map(0.f, explosion_radius, 1.f, 0.8f, dist)
				: (std::max)(map(
					explosion_radius, 3 * explosion_radius, 0.2f, 0.f, dist
				), 0.f);
			auto& chao_data_base = *chao->Data1.Chao->ChaoDataBase_ptr;
			// make sadder
			apply_ratio(
				chao_data_base.Happiness,
				-100,
				100,
				0,
				config.nearby_chao.happiness,
				explosion_ratio
			);
			auto& char_bonds =
				*reinterpret_cast<chao_character_bonds*>(
					&chao_data_base.SA2BCharacterBonds[CurrentCharacter]
				);
			apply_ratio(
				char_bonds.like,
				-100,
				100,
				0,
				config.nearby_chao.char_like,
				explosion_ratio
			);
			apply_ratio(
				char_bonds.fear,
				0,
				200,
				0,
				config.nearby_chao.char_fear,
				explosion_ratio
			);
			auto& emo = chao_data_base.Emotion;
			apply_ratio(
				emo.Anger,
				0,
				200,
				0,
				config.nearby_chao.anger,
				explosion_ratio
			);
			apply_ratio(
				emo.Boredom,
				0,
				10000,
				0,
				config.nearby_chao.boredom,
				explosion_ratio
			);
			apply_ratio(
				emo.Fear,
				0,
				200,
				0,
				config.nearby_chao.fear,
				explosion_ratio
			);
			apply_ratio(
				emo.Joy,
				0,
				200,
				0,
				config.nearby_chao.joy,
				explosion_ratio
			);
			apply_ratio(
				emo.MateDesire,
				0,
				10000,
				0,
				config.nearby_chao.mate_desire,
				explosion_ratio
			);
			apply_ratio(
				emo.Relax,
				0,
				200,
				0,
				config.nearby_chao.relax,
				explosion_ratio
			);
			apply_ratio(
				emo.Surprise,
				0,
				200,
				0,
				config.nearby_chao.surprise,
				explosion_ratio
			);
			apply_ratio(
				emo.UrgeToCry,
				0,
				200,
				0,
				config.nearby_chao.urge_to_cry,
				explosion_ratio
			);
			apply_ratio(
				emo.Energy,
				0,
				10000,
				0,
				config.nearby_chao.energy,
				explosion_ratio
			);

			// if chao is in radius of explosion
			if (dist > 0.1f and dist < explosion_radius) {
				if (config.chain_explosions) {
					if (CurrentChaoArea == ChaoArea::ChaoArea_Race) {
						set_chao_behaviour(chao, chao_behaviour::trip_in_race);
					}
					else {
						set_chao_behaviour(chao, chao_behaviour::damaged);
					}
					// dizziness, only for those in explosion
					apply_ratio(
						emo.Dizziness,
						0,
						200,
						0,
						config.hit_chao.dizziness,
						explosion_ratio
					);
				}
				// get thrown back
				constexpr float knockback_mult{ 16.f };
				auto unit_vector = delta / dist;
				auto vel = (unit_vector * knockback_mult / dist);
				vel.y += 0.8f; // throw upwards
				chao->EntityData2->vector_0 = vel;
			}
		}
	}
}

void delete_chao_from_save_file(ObjectMaster& chao) {
	if (auto* const chao_data_base = chao.Data1.Chao->ChaoDataBase_ptr) {
		*chao_data_base = {};
	}
}

void explode_chao(ObjectMaster& chao) {
	chao_user_data[&chao].explode_timer.reset();
	create_explosion(chao.Data1.Entity->Position);
	if (config.play_jingle) {
		PlayJingle(reinterpret_cast<char*>(0x12f5944));
	}
	switch (config.death_style) {
	case death_style::nothing:
		break;
	case death_style::permanent:
		// seems to work with Chao Races too? It'll probably be fine.
		delete_chao_from_save_file(chao);
		[[fallthrough]];
	case death_style::temporary:
		switch (CurrentChaoArea) {
		case ChaoArea::ChaoArea_Race:
			// game crashes if race chao is deleted.
			// game crashes if camera is at inf and the race it quit.

			// chao doesn't move if behaviour is manually set to pitfall
			set_chao_behaviour(&chao, chao_behaviour::pitfall_or_jump_scared);
			// doesn't render most of the chao. Their emotionball and shadow still
			// does though :(
			chao.DisplaySub = nullptr;
			break;
		default:
			chao.MainSub = DeleteObject_;
			break;
		}
		break;
	}
}

void elapse_time() {
	for (auto& [chao, data] : chao_user_data) {
		if (data.explode_timer) {
			if (--*data.explode_timer == 0) {
				explode_chao(*chao);
				if (
					chao_user_data[chao].prev_behaviour
					== chao_behaviour::thrown_into_departure_machine
				) {
					// only really changes chao spawn position on exit, and
					// whether you can delete the chao in the machine, so not
					// too important I don't think
					if (config.death_style != death_style::nothing) {
						erase_chao_in_departure_machine();
					}
				}
			}

			// kind of a hardcoded fix to a problem I am unsure why is occuring
			// make sure, fr fr, chao is unholdable...
			chao->Data1.Entity->Status &= ~Status::Status_Unknown4;
		}
	}
}

std::uint32_t* set_chao_behaviour(
	ObjectMaster* const chao,
	chao_behaviour behaviour,
	const std::uint32_t a3
) {
	// +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
	// | BEGIN UGLY REIMPLEMENTATION OF ORIGINAL FUNCTION! |
	// +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
	const auto v3 = (int)chao->Data1.Undefined + 436;
	WriteData(reinterpret_cast<short*>(v3 + 2), short{ 0 });
	WriteData(reinterpret_cast<short*>(v3 + 4), short{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 12), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 16), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 20), _DWORD{ 3600 });
	WriteData(
		reinterpret_cast<_DWORD*>(v3 + 636),
		*(_DWORD*)(v3 + 4 * *(_DWORD*)(v3 + 644) + 648)
	);
	WriteData(reinterpret_cast<_DWORD*>(v3 + 652), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 656), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 660), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 664), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 668), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 672), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 676), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 680), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 684), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 688), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 692), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 696), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 700), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 704), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 708), _DWORD{ 0 });
	WriteData(reinterpret_cast<chao_behaviour*>(v3 + 648), behaviour);
	WriteData(reinterpret_cast<_DWORD*>(v3 + 640), _DWORD{ 1 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 644), _DWORD{ 0 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 712), _DWORD{ a3 });
	WriteData(reinterpret_cast<_DWORD*>(v3 + 616), _DWORD{ 0 });
	const auto v4 = chao->UnknownA_ptr;
	if (v4) {
		const auto v5 = v4[1].field_8;
		if (v5) {
			if (*(ObjUnknownA**)(v5 + 64) == v4) {
				WriteData(reinterpret_cast<short*>(v5 + 52), short{ 3 });
			}
		}
	}

	const auto result = GenerateUsercallWrapper<std::uint32_t*(*)(const ObjectMaster*)>(
		registers::rEAX, 0x53D760, registers::rEAX
	)(chao);
	// +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
	// | END UGLY REIMPLEMENTATION OF ORIGINAL FUNCTION! |
	// +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+

	auto& chao_user_data = ::chao_user_data[chao];
	if (chao_user_data.prev_behaviour != behaviour) {
		if (
			const auto it = explosion_sources.find(behaviour);
			it != explosion_sources.end()
		) {
			const auto& randomness = it->second;
			if (
				percent(rng) < randomness.chance
				and
				(
					randomness.extra_condition == nullptr
					or randomness.extra_condition(*chao)
				)
			) {
				const auto& [begin, end] = randomness.time_range;
				chao_user_data.explode_timer =
					(std::min)(
						chao_user_data.explode_timer.value_or(-1u),
						static_cast<unsigned>(
							std::lerp(begin, end, ratio(rng))
						)
					);
				// make unholdable
				chao->Data1.Entity->Status &= ~Status::Status_Unknown4;
			}
		}
		// if primed and changing state to something else
		else if (auto& explode_timer = chao_user_data.explode_timer) {
			explode_chao(*chao);
		}
	}

	chao_user_data.prev_behaviour = behaviour;

	return result;
}
