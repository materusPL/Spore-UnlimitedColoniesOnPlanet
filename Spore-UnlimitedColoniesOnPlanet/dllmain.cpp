// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Spore\Simulator\cPlaceColonyToolStrategy.h>

using namespace Simulator;

virtual_detour(UnlimitedColoniesOnPlanetDetour, cPlaceColonyToolStrategy, cToolStrategy, bool(cSpaceToolData*, bool, const char16_t**))
{
	bool detoured(cSpaceToolData * pTool, bool showErrors, const char16_t** ppFailText = nullptr)
	{
		auto planet = GetActivePlanetRecord();
		
		if (GetCurrentContext() == SpaceContext::Planet
			&& planet != nullptr
			&& pTool->mCurrentAmmoCount > 0
			&& planet->mType == PlanetType::T3
			&& planet->GetTechLevel() == TechLevel::Empire
			&& GetActiveStarRecord()->mEmpireID == GetPlayerEmpireID())
		{
			return true;
		}
		else
		{
			return original_function(this, pTool, showErrors, ppFailText);
		}
	};
};


void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
}

void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
	UnlimitedColoniesOnPlanetDetour::attach(GetAddress(cPlaceColonyToolStrategy, Update));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

