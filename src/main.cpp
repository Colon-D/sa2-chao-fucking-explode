#include <SA2ModLoader.h>
#include <iostream>
#include "hooks.h"
#include "chao_functions.h"
#include "globals.h"
#include "config.h"

extern "C" {

__declspec(dllexport)
void Init(
	const char* const path,
	const HelperFunctions& /*helperFunctions*/
) {
	config.load(path);
	hook();
}

__declspec(dllexport)
void __cdecl OnFrame() {
	if (GameState == GameStates::GameStates_Pause) {
		return;
	}

	elapse_time();
}

__declspec(dllexport)
ModInfo SA2ModInfo{ ModLoaderVer };

}
