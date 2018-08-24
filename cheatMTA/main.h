#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <chrono>
#include <thread>


/*

-- DETECTED --

bool turn = true;
void wallhack(){
	printf("Wallhack!\n");
	DWORD ped = *(DWORD*)0xb6f5f0;
	DWORD* dwthis = (DWORD*)(ped + 0x42);
	printf("pThis \n 0x%08X\n", dwthis);

	if (turn) {
		printf("on\n");
		*dwthis = 1;
		turn = false;
	} else {
		turn = true;
		printf("off\n");
		*dwthis = 0;
	}

}

-- DETECTED --

if (GetAsyncKeyState(VK_NUMPAD1)) {
std::this_thread::sleep_for(std::chrono::seconds(2));
float* teleport = (float*)0x5BD7B0;

_asm {
call teleport
}


std::this_thread::sleep_for(std::chrono::seconds(2));
}

-- UNUSED --

void SwitchTermal(bool bStat) {

unsigned int* termal = (unsigned int*)0xC402B9;

*termal = bStat;

}

void SwitchNightly(bool bStat) {

unsigned int* nightly = (unsigned int*)0xC402B8;

*nightly = bStat;

}

bool onNightly = false;
bool onTermal = false;
bool onStop = false;

float Get3DDistance(float * myCoords, float * enemyCoords) {
return sqrt(
pow(double(enemyCoords[0] - myCoords[0]), 2.0) +
pow(double(enemyCoords[1] - myCoords[1]), 2.0) +
pow(double(enemyCoords[2] - myCoords[2]), 2.0));

}

*/

void ToggleEngine(bool bStat) {

	DWORD ped = *(DWORD*)0xb6f5f0;

	DWORD CVehicle = *(DWORD*)0xB6F980;
	if (CVehicle > 0) // Broken check, sometimes crashes.
	{
		DWORD dwThis = *(DWORD*)(ped + 0x58C);
		DWORD dwFunc = 0x41BDD0;
		bool bEngineOn = bStat;

		__asm
		{
			mov ecx, dwThis
			push bEngineOn
			call dwFunc
		}
	}
}

void SetFireCar() {

	DWORD CCar = *(DWORD*)0xB6F980;
	*(float*)(CCar + 1216) = 0.0f; // Car Armor/Health

}

void PlaceBomb() {

	DWORD CCar = *(DWORD*)0xB6F980;
	*(DWORD*)(CCar + 1192) = 0x5;

}

void NoSpread() {

	// Since patching memory is not viable i just keep on resetting it.

	float* spread = (float*)0xB7CDC8;
	*spread = 0.0f; // You can try setting it to a lower number to see if recoil lasts more.

	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void BoomAllCloseVehicles() {

	DWORD addr = 0x439D80;
	__asm call addr;

}

void GetJetpack() {
	DWORD addr = 0x439600;
	__asm call addr;
}

void Wallhack(bool value) {

	DWORD ped = *(DWORD*)0xB7CD98;
	*(float*)(ped + 0x2C) = (float)value;

}

float offset = 0.0f;

void PullPeds() {

	if (GetAsyncKeyState(VK_ADD)) {

		offset += 5.0f;

	}
	else if (GetAsyncKeyState(VK_SUBTRACT)) {

		offset -= 5.0f;

	}

	DWORD* ped = (DWORD*)0xB7CD98;
	DWORD* pMatrix = (DWORD*)(*ped + 0x14);
	float* x = (float*)((*pMatrix) + 0x30);
	float* y = (float*)((*pMatrix) + 0x34);
	float* z = (float*)((*pMatrix) + 0x38);

	DWORD* next = nullptr;
	float hp = 0;
	float hundred = 0;

	// Harcoded 500 because i'm shit enough not to find the actual pool size.
	for (int i = 0; i < 500; i++) {
		next = (DWORD*)(*ped + (0x7C4 * i));

		hundred = *(float*)(next + 0xE); // 0xE because i'm dereferencing it as a float type.

		if (i != 0 && hundred == 100.0f) {

			DWORD* player_info = (DWORD*)(next + 0x5);
			hp = *(float*)(next + 0x150);
			float* _x = (float*)(*player_info + 0x30);
			float* _y = (float*)(*player_info + 0x34);
			float* _z = (float*)(*player_info + 0x38);
			if (hp >= 0) {

				*_x = *x + offset + i;
				*_y = *y;
				*_z = *z;

			}
		}
	}
}

void main() {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	printf("Injected!\n");

	bool bWallhack = false;

	// Works in GTA:SA/MTA:SA/SA-MP since it uses only GTA:SA addresses.

	do {

		NoSpread(); // aka no recoil

		if (GetAsyncKeyState(VK_NUMPAD0)) {

			printf("Vehicles exploded!\n");

			BoomAllCloseVehicles();
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		
		if (GetAsyncKeyState(VK_NUMPAD1)) {

			printf("Wallhack %d!\n", bWallhack);

			std::this_thread::sleep_for(std::chrono::milliseconds(150));

			bWallhack ? Wallhack(true) : Wallhack(false);

			bWallhack = !bWallhack;

		}

		if (GetAsyncKeyState(VK_NUMPAD2)) {

			printf("Car set on fire!\n");

			SetFireCar();
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}

		if (GetAsyncKeyState(VK_NUMPAD3)) {

			printf("Engine started!\n");

			ToggleEngine(true);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		
		if (GetAsyncKeyState(VK_NUMPAD4)) {

			printf("Bomb placed!\n");

			PlaceBomb();
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		
		if (GetAsyncKeyState(VK_NUMPAD5)) {

			printf("Pedestrians !\n");

			PullPeds();
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}

		if (GetAsyncKeyState(VK_NUMPAD6)) {

			printf("Jetpack attached!\n");

			GetJetpack();
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		
		/*if (GetAsyncKeyState(VK_NUMPAD6)) {

			teleport();

		}
				
		if (GetAsyncKeyState(VK_NUMPAD8)) {
			teleport_deathmatch();
		}

		if (GetAsyncKeyState(VK_NUMPAD8) && !onNightly) {

			SwitchNightly(true);
			onNightly = true;

		} else if (GetAsyncKeyState(VK_NUMPAD8)) {

			SwitchNightly(false);
			onNightly = false;
		}

		if (GetAsyncKeyState(VK_NUMPAD7) && !onTermal) {

			SwitchTermal(true);
			onTermal = true;

		}
		else if (GetAsyncKeyState(VK_NUMPAD7)) {

			SwitchTermal(false);
			onTermal = false;
		}
		
		if (GetAsyncKeyState(VK_NUMPAD5) && !onStop) {

			simulate_menu(true);
			onStop = true;

		}
		else if (GetAsyncKeyState(VK_NUMPAD5)) {

			simulate_menu(false);
			onStop = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(250));

		}

		}
	*/
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	} while (!GetAsyncKeyState(VK_END));

	system("cls");
	printf("Finished\n");
	fclose(stdout);
	FreeConsole();

	return;
}