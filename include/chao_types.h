#pragma once
#include <SA2ModLoader.h>
#include <optional>
#include <functional>
#include <IniFile.hpp>

// slightly updated
struct chao_character_bonds {
	std::int8_t like{};
	std::uint8_t fear{};
	std::uint16_t distance{};
	std::uint16_t meet{};
};

// stolen and expanded from:
// https://github.com/Exant64/CWE/blob/d278bc057a9cb8cf72693770aa0d253288c037fa/CWE/Chao.h#L26
enum class chao_behaviour {
	race_wait = 0x538be0, // pose before the race start
	sleep = 0x54ef10,
	getting_tired = 0x54f980,
	tired = 0x54fcf0, // why the timer goes up on this one?
	move = 0x55c3c0, // posturechange
	tire = 0x55c430, // chao sit (also seems to be bouncing from jumping?)
	broom_stick = 0x55dd10,
	pogo_stick = 0x55e6d0,
	handing = 0x5606d0,
	eating = 0x5607c0,
	pick_up_fruit = 0x5613c0,
	swim = 0x561fb0,
	super_swim = 0x562040, // chao swim but his swim is over 700 points
	swim_idle = 0x562160,
	trip = 0x562d50,
	climb = 0x562eb0,
	fly = 0x5630c0, // fly, my child
	go_to_player = 0x563250, // player used whistle
	being_called = 0x5634d0, // player used whistle
	go_to_ball = 0x563830,
	damaged = 0x563eb0,
	thrown = 0x564320,
	jump_scared = 0x5647b0, // from the jack n box toy
	get_pet = 0x5648a0,
	// this also kill prevfunc
	// so that's gonna suck to detect previous behaviour
	think = 0x565be0,
	got_animal = 0x566df0,
	think_seed = 0x567710,
	walk_with_seed = 0x567830,
	found_seed_spot = 0x567b10,
	water_can = 0x5680f0,
	water_can_dance = 0x568460,
	holding_item = 0x569340,
	notice_item = 0x56b480,
	put_hat = 0x569430,
	throw_item = 0x569550,
	go_to_item = 0x56ba80,
	watching_tv = 0x598890,
	watching_radio = 0x598b90,
	watching_sing = 0x59b3d0,
	mate_walk = 0x59bf30,
	watching_dance = 0x59cda0,
	maracas = 0x59d5a0,
	flute = 0x59dd10,
	trumpet = 0x59e120,
	watching_music = 0x59e710,
	want_the_hold_fruit = 0x59ea00, // chao begs player holding a fruit
	go_to_hold_fruit = 0x59ed60, // chao goes to player holding a fruit
	hungry = 0x59ed60,
	starving = 0x59ef40,
	fear = 0x59f350,
	sadness = 0x59f970,
	urge_to_cry = 0x59fa50,
	urge_to_cry2 = 0x59fca0, // oh god there's tears
	dizzy = 0x5a00a0,
	hero_hi = 0x5a03b0,
	love_the_player = 0x5a0820,
	come_to_player = 0x5a0960,
	jump = 0x5a0f40,
	anger = 0x5a1460,
	chao_hate = 0x5a1800, // "baaah!"
	sit_think = 0x5a1ba0,
	unicorn = 0x5a1fc0,
	gorilla = 0x5a2010,
	wart_hog = 0x5a2310,
	bunny = 0x5a2860,
	crawl = 0x5a3280,
	walk = 0x5a3380,
	run1 = 0x5a3460,
	run2 = 0x5a3580,
	drawing = 0x5ab2c0,
	come_out_egg = 0x5685f0,

	thrown_birth = 0x563c00,
	cocoon_reincarnate_or_die = 0x568f60,
	cocoon_evolve = 0x568d30,
	thrown_into_departure_machine = 0x53d720,
	trip_in_race = 0x538d00, // doesn't get uncalled?
	jack_n_box_or_begin_puzzle_race = 0x55f4a0,
	pitfall_or_jump_scared = 0x55f700, // doesn't get uncalled
	succeeded_puzzle = 0x538350,
	failed_puzzle = 0x55fe80,
	failed_puzzle_bonk = 0x560090, // doesn't get uncalled
	jack_n_box1 = 0x5a1020,
	jack_n_box2 = 0x599770,
	cough = 0x599D40,
	cold = 0x599F30,
	rash = 0x59A0B0, // don't think this is worth exploding over
	running_nose = 0x59A260, // don't think this is worth exploding over
	hiccups = 0x59A400,
	stomach_ache = 0x59A5A0,
	land_in_water = 0x562330,
	bounce = 0x562d20
};

struct chao_user_data {
	chao_behaviour prev_behaviour{};
	// when "primed" (below is set), will explode in x number of frames
	// OR when the behaviour changes.
	std::optional<unsigned> explode_timer{};
};

// in frames
struct time_range {
	unsigned begin{};
	unsigned end{};
};

// loaded from config, except for the extra_conditions which are hardcoded
struct explosion_source {
	float chance{};
	time_range time_range{};
	std::function<bool(ObjectMaster&)> extra_condition{};

	void load(const IniGroup* ini_group, std::string_view key);
};
