#pragma once
#include <SA2ModLoader.h>
#include <unordered_map>
#include "chao_types.h"
#include "config.h"

extern std::unordered_map<ObjectMaster*, struct chao_user_data> chao_user_data;
extern struct config config;
extern std::unordered_map<chao_behaviour, explosion_source> explosion_sources;
