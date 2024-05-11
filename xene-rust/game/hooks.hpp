#pragma once
#include "../hinclude.h"
#include "../ongui.h"
#include "../raids.h"
#include "../baseplayer.h"
#include "../utils/lazy_importer.hpp"
#include "../settings.h"

namespace original {
	uintptr_t network_adr;
	uintptr_t sprint_adr;
	//uintptr_t fixedUpdate;
	//uintptr_t input_adr;
}
void onNetwork(uintptr_t client, uintptr_t packet)
{
	//std::cout << "called network" << std::endl;
	if (std::get<bool>(ui::element["raid_esp"].value)) {
		raids::get_raids();
	}
	auto orig_network = reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(original::network_adr);
	SpoofReturn(orig_network, client, packet);
}
/*void blockSprint(rust::BasePlayer* player, float duration)
{
	auto orig_sprint = reinterpret_cast<void(*)(rust::BasePlayer*, float)>(original::sprint_adr);
	if (!std::get<bool>(ui::element["can_shoot"].value)) {
		SpoofReturn(orig_sprint, player, duration);
	}
	else {
		duration = 0;
		SpoofReturn(orig_sprint, player, duration);
	}
}
void fixedUpdate(rust::PlayerWalkMovement* base_movement, rust::ModelState* modelstate)
{
	auto orig_fix = reinterpret_cast<void(*)(rust::PlayerWalkMovement*, rust::ModelState*)>(original::fixedUpdate);

	if (std::get<bool>(ui::element["jesus"].value)) {
		
	}
	//std::cout << "called fixed" << std::endl;
	variables::local_player->get_modelstate()->set_flying(false);
	//*reinterpret_cast<Vector3*>(base_movement + 0x34) = vel;

	SpoofReturn(orig_fix, base_movement, modelstate);
}
*/

namespace hooks
{
	__forceinline bool init()
	{
		ongui::performaceui_hook = *reinterpret_cast<uintptr_t*>(il2cpp::method(xorstr_("PerformanceUI"), xorstr_("Update"), 0, xorstr_("Facepunch")));
		il2cpp::hook(&ongui::hk_performance_ui_update, xorstr_("PerformanceUI"), xorstr_("Update"), 0, xorstr_("Facepunch"));
		il2cpp::hook(&ongui::ongui_hook, xorstr_("DevControls"), xorstr_("OnGUI"), 0, xorstr_(""));

		//il2cpp::hook(&benchmark, xorstr_("BenchmarkData"), xorstr_("BenchmarkData"), 0, xorstr_("Facepunch"));
		original::network_adr = il2cpp::hook_virtual_function(xorstr_("Client"), xorstr_("OnNetworkMessage"), &onNetwork, 1);
		//original::add_overlay_adr = il2cpp::hook(xorstr_("UIBlackoutOverlay"), xorstr_("AddOverlay"), &overlays, 2);
		//original::sprint_adr = il2cpp::hook_virtual_function(xorstr_("BasePlayer"), xorstr_("BlockSprint"), &blockSprint, 1);
		//original::input_adr = il2cpp::hook_virtual_function(xorstr_("BasePlayer"), xorstr_("ClientInput"), &client_input, 1);
		//original::fixedUpdate = il2cpp::hook_virtual_function(xorstr_("PlayerWalkMovement"), xorstr_("DoFixedUpdate"), &fixedUpdate, 1);

		//original::flying_adr = *reinterpret_cast<uintptr_t*>(il2cpp::method(xorstr_("ModelState"), xorstr_("set_flying"), 1, xorstr_("")));
		//il2cpp::hook(&set_flying, xorstr_("ModelState"), xorstr_("set_flying"), 1, xorstr_(""));




		return true;
	}
}