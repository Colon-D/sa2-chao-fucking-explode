#include "hooks.h"
#include "globals.h"
#include "chao_functions.h"
#include <SA2ModLoader.h>
#include <FunctionHook.h>
#include <UsercallFunctionHandler.h>

void __cdecl dont_drop_rings(const int /*player_number*/) {
}

void hook() {
	// set Drop Rings function to a dummy function, instead of a nullptr that
	// gets dereferenced when hit and crashes the game inside the chao garden.
	static FunctionHook<int> chao_main_ctor{
		ChaoMain_Constructor,
		[] {
			DropRingsFunc_ptr = dont_drop_rings;

			const auto result = chao_main_ctor.Original();

			return result;
		}
	};

	static FunctionHook<void, ObjectMaster*> chao_dtor {
		Chao_Delete,
		[](auto* const chao) {
			chao_user_data.erase(chao);
			return chao_dtor.Original(chao);
		}
	};

	GenerateUsercallHook(
		set_chao_behaviour,
		registers::rEAX,
		reinterpret_cast<intptr_t>(Chao_BehaviourPtr),
		registers::rEAX,
		registers::stack4,
		registers::stack4
	);
}
