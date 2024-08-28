#pragma once
#include <filesystem>
#include <IniFile.hpp>
#include "chao_types.h"

struct nearby_chao {
	float happiness{};
	float char_like{};
	float char_fear{};
	float anger{};
	float boredom{};
	float fear{};
	float joy{};
	float mate_desire{};
	float relax{};
	float surprise{};
	float urge_to_cry{};
	float energy{};
};

struct hit_chao {
	float dizziness{};
};

enum class death_style {
	nothing,
	temporary,
	permanent
};

struct config {
	death_style death_style{};
	bool play_jingle{};
	bool chain_explosions{};

	nearby_chao nearby_chao{};
	hit_chao hit_chao{};

	float bounce_from_jump_height_multiplier{};

	bool debug_output{};

	void load(const std::filesystem::path& path);
};
