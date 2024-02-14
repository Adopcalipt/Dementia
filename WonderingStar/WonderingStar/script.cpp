/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
			Hijacked by Adopcalipt 2024
*/

#include "script.h"
#include "keyboard.h"

#include <fstream>		//ofstream read/write text documents
#include <iostream>		//Header that defines the standard input/output stream objects:
#include <string>
#include <vector>

//#pragma warning(disable : 4244 4305) // double <-> float conversions

int Wait_Time = 45000;
bool Mod_Enable = true;
std::string GetExeFileName()
{
	char buffer[MAX_PATH]{};
	GetModuleFileName(NULL, buffer, ARRAYSIZE(buffer));

	return std::string(buffer);
}
std::string GetDir()
{
	std::string f = GetExeFileName();
	std::string Dir = f.substr(0, f.find_last_of("\\/"));

	return Dir;
}
bool FileExists(const std::string& filename)
{
	try
	{
		std::ifstream Infield(filename);
		return Infield.good();
	}
	catch (...)
	{
		return false;
	}
}
std::vector<std::string> ReadSetFile(const std::string& fileName)
{
	std::string myText;
	std::vector<std::string> textList = {};

	if (FileExists(fileName))
	{
		std::ifstream MyReadFile(fileName);

		while (std::getline(MyReadFile, myText))
		{
			// Output the text from the file
			textList.push_back(myText);
		}
		MyReadFile.close();
	}

	return textList;
}

bool StringContains(const std::string& line, const std::string& wholeString)
{
	bool Contain = false;
	int iLine = 0;
	std::string Word = "";
	for (int i = 0; i < wholeString.length(); i++)
	{
		if (iLine < line.length())
		{
			if (line[iLine] == wholeString[i])
			{
				Contain = true;
				Word += wholeString[i];
				iLine++;
			}
			else
			{
				iLine = 0;
				Word = "";
			}
		}
		else if (iLine > 0)
		{
			if (line == Word)
				break;
			else
			{
				iLine = 0;
				Word = "";

				if (line[0] == wholeString[i])
				{
					Contain = true;
					Word += wholeString[i];
					iLine++;
				}
			}
		}
	}

	if (Contain)
	{
		if (line != Word)
			Contain = false;
	}

	return Contain;
}
int StingNumbersInt(const std::string& line)
{
	int iNum = 0;
	int iTimes = 0;
	bool bNegative = false;
	for (int i = (int)line.size(); i > -1; i--)
	{
		bool bSkip = false;
		int iAdd = 0;
		char sComp = line[i];

		if (sComp == char(45))
			bNegative = true;
		else if (sComp == char(49))
		{
			bNegative = false;
			iAdd = 1;
		}
		else if (sComp == char(50))
		{
			bNegative = false;
			iAdd = 2;
		}
		else if (sComp == char(51))
		{
			bNegative = false;
			iAdd = 3;
		}
		else if (sComp == char(52))
		{
			bNegative = false;
			iAdd = 4;
		}
		else if (sComp == char(53))
		{
			bNegative = false;
			iAdd = 5;
		}
		else if (sComp == char(54))
		{
			bNegative = false;
			iAdd = 6;
		}
		else if (sComp == char(55))
		{
			bNegative = false;
			iAdd = 7;
		}
		else if (sComp == char(56))
		{
			bNegative = false;
			iAdd = 8;
		}
		else if (sComp == char(57))
		{
			bNegative = false;
			iAdd = 9;
		}
		else if (sComp == char(48))
		{

		}
		else
			bSkip = true;

		if (!bSkip)
		{
			if (iTimes == 0)
			{
				iNum = iAdd;
				iTimes = 1;
			}
			else
				iNum += iAdd * iTimes;
			iTimes *= 10;
		}
	}

	if (bNegative)
		iNum = iNum * (-1);

	return iNum;
}
bool StringBool(const std::string& line)
{
	if (StringContains("True", line))
		return true;
	else if (StringContains("true", line))
		return true;
	else if (StringContains("TRUE", line))
		return true;
	else if (StringContains("1", line))
		return true;
	else
		return false;
}

std::string BoolToString(bool b)
{
	std::string output = "false";
	if (b)
		output = "true";
	return output;
}

void AddSettinngs()
{
	std::string outFile = GetDir() + "/Dementia.ini";

	std::vector<std::string> AddIni = {
		"[Settings]",
		"Timeout_Sec=" + std::to_string(Wait_Time / 1000),
		"Enabled=" + BoolToString(Mod_Enable),
	};

	std::ofstream MyFile(outFile);

	for (int i = 0; i < AddIni.size(); i++)
	{
		MyFile << AddIni[i];
		MyFile << "\n";
	}

	MyFile.close();
}
void FindSettings()
{
	std::vector<std::string> MySet = ReadSetFile(GetDir() + "/Dementia.ini");

	if (MySet.size() == 0)
		AddSettinngs();
	else
	{
		for (int i = 0; i < (int)MySet.size(); i++)
		{
			std::string line = MySet[i];
			if (StringContains("Timeout_Sec", line))
				Wait_Time = StingNumbersInt(line) * 1000;
			else if (StringContains("Enabled", line))
				Mod_Enable = StringBool(line);
		}
	}

}

int BottomLeft(const std::string& sText)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)sText.c_str());
	return UI::_DRAW_NOTIFICATION(1, 1);
}

float DistanceTo(Vector3 postion1, Vector3 postion2)
{
	double num = postion2.x - postion1.x;
	double num2 = postion2.y - postion1.y;
	double num3 = postion2.z - postion1.z;
	return (float)sqrt(num * num + num2 * num2 + num3 * num3);
}
float DistanceTo(Entity entity1, Vector3 postion1)
{
	Vector3 entpos = ENTITY::GET_ENTITY_COORDS(entity1, false);

	return DistanceTo(entpos, postion1);
}
float DistanceTo(Entity entity1, Entity entity2)
{
	Vector3 postion1 = ENTITY::GET_ENTITY_COORDS(entity1, false);
	Vector3 postion2 = ENTITY::GET_ENTITY_COORDS(entity2, false);

	return DistanceTo(postion1, postion2);
}

void FlyHeli(Ped Pedd, Vehicle Vhick, Vehicle target, float fSpeed, float flanding)
{
	AI::CLEAR_PED_TASKS(Pedd);
	float HeliSpeed = fSpeed;
	float HeliLandArea = flanding;

	Vector3 VHickPos = ENTITY::GET_ENTITY_COORDS(Vhick, false);
	Vector3 TargetPos = ENTITY::GET_ENTITY_COORDS(target, false);

	float dx = VHickPos.x - TargetPos.x;
	float dy = VHickPos.y - TargetPos.y;
	float HeliDirect = GAMEPLAY::GET_HEADING_FROM_VECTOR_2D(dx, dy) - 180.00f;

	AI::TASK_HELI_MISSION(Pedd, Vhick, target, 0, 0, 0, 0, 9, HeliSpeed, HeliLandArea, HeliDirect, -1, -1, -1, 0);
	PED::SET_PED_KEEP_TASK(Pedd, true);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Pedd, true);
}

bool Mod_loaded = false;
bool Go_Wonder = false;
bool InAveh = false;

int YourViews = 0;

Ped FindThatPed(Vector3 area)
{
	Ped Bob = NULL;

	const int PED_ARR_SIZE = 1024;
	Ped Pedds[PED_ARR_SIZE];
	int Pedcount = worldGetAllPeds(Pedds, PED_ARR_SIZE);
	std::vector<Ped> NpcList = {};

	for (int i = 0; i < Pedcount; i++)
	{
		Ped Pent = Pedds[i];
		if (Pent != NULL && ENTITY::DOES_ENTITY_EXIST(Pent))
		{
			if (!ENTITY::IS_ENTITY_A_MISSION_ENTITY(Pent) && DistanceTo(ENTITY::GET_ENTITY_COORDS(Pent, false), area) > 45.0f && PED::GET_PED_TYPE(Pent) != 2 && !(bool)PED::IS_PED_IN_ANY_VEHICLE(Pent, 0) && !(bool)PED::IS_PED_FLEEING(Pent) && (bool)AI::IS_PED_WALKING(Pent))	//&& DistanceTo(ENTITY::GET_ENTITY_COORDS(Pent, false), area) < 95.0f
			{
				Bob = Pedds[i];
				break;
			}
		}
	}
		
	return Bob;
}
Vehicle FindThatVeh(Vector3 area)
{
	Vehicle Vick = NULL;
	const int PED_ARR_SIZE = 1024;
	Vehicle Vehcs[PED_ARR_SIZE];
	int Vehhcount = worldGetAllVehicles(Vehcs, PED_ARR_SIZE);
	std::vector<Ped> NpcList = {};

	for (int i = 0; i < Vehhcount; i++)
	{
		Vick = Vehcs[i];
		if (Vick != NULL && ENTITY::DOES_ENTITY_EXIST(Vick))
		{
			if (!ENTITY::IS_ENTITY_A_MISSION_ENTITY(Vick) && DistanceTo(ENTITY::GET_ENTITY_COORDS(Vick, false), area) > 45.0f && VEHICLE::_IS_VEHICLE_ENGINE_ON(Vick)) //&& DistanceTo(ENTITY::GET_ENTITY_COORDS(Vick, false), area) < 255.0f
				break;
		}
	}

	return Vick;
}

Vehicle followVeh = NULL;
Ped followMe = NULL;
int timeTwo = 0;
float distanceToo = 5000.0f;

void WonderYeOnda(bool InVeh)
{
	Ped player = PLAYER::PLAYER_PED_ID();
	if (InVeh)
	{
		if (followVeh == NULL)
		{
			Go_Wonder = true;
			Vector3 PlayPos = ENTITY::GET_ENTITY_COORDS(player, false);
			followVeh = FindThatVeh(PlayPos);
			if (followVeh != NULL)
			{
				Vehicle PlayVeh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);

				if (VEHICLE::IS_THIS_MODEL_A_HELI(PlayVeh))
					FlyHeli(PLAYER::PLAYER_PED_ID(), PlayVeh, followVeh, 45.0f, 10.0f);
				else
					AI::TASK_VEHICLE_ESCORT(PLAYER::PLAYER_PED_ID(), PlayVeh, followVeh, -1, 21.0f, 786603, 10.0f, -1, 30.0f);

				timeTwo = (int)GAMEPLAY::GET_GAME_TIMER() + 10000;
			}
		}
		else if (timeTwo < (int)GAMEPLAY::GET_GAME_TIMER())
		{
			timeTwo = (int)GAMEPLAY::GET_GAME_TIMER() + 10000;
			if (followVeh != NULL)
			{
				if (!ENTITY::DOES_ENTITY_EXIST(followVeh))
					followVeh = NULL;
				else
				{
					if (DistanceTo(player, followVeh) < 15.0f)
						followVeh = NULL;
				}
			}
		}
	}
	else
	{
		if (followMe == NULL)
		{
			Go_Wonder = true;
			Vector3 PlayPos = ENTITY::GET_ENTITY_COORDS(player, false);
			followMe = FindThatPed(PlayPos);
			timeTwo = (int)GAMEPLAY::GET_GAME_TIMER() + 10000;
			if (followMe != NULL)
			{
				AI::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(PLAYER::PLAYER_PED_ID(), followMe, 0.0f, 0.0f, 0.0f, 1.0f, -1, 3.0f, true);
				distanceToo = DistanceTo(player, followMe);
			}
		}
		else if (timeTwo < (int)GAMEPLAY::GET_GAME_TIMER())
		{
			timeTwo = (int)GAMEPLAY::GET_GAME_TIMER() + 10000;
			if (followMe != NULL)
			{
				if (!ENTITY::DOES_ENTITY_EXIST(followMe))
					followMe = NULL;
				else
				{
					if (DistanceTo(player, followMe) < distanceToo)
					{
						if (DistanceTo(player, followMe) < 5.0f)
							followMe = NULL;
						else
							distanceToo = DistanceTo(player, followMe);
					}
					else
						followMe = NULL;
				}
			}
		}
	}
}
void StopHere()
{
	if (Go_Wonder)
	{
		Go_Wonder = false;
		AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
		if (InAveh && PLAYER::GET_PLAYERS_LAST_VEHICLE() != NULL)
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYERS_LAST_VEHICLE(), -1);
	}
	
	CAM::SET_FOLLOW_PED_CAM_VIEW_MODE(YourViews);
}

int Last_Press = 0;
int countDown = 0;
Vector3 PlayPos;

bool InPressed()
{
	bool bDone = false;
	for (int i = 0; i < 343; i++)
	{
		if (CONTROLS::IS_CONTROL_PRESSED(i, 1))
		{
			bDone = true;
			break;
		}
	}
	return bDone;
}
bool MotionTracking()
{
	bool b = false;

	if (!CUTSCENE::IS_CUTSCENE_PLAYING())
	{
		if (countDown == 0)
		{
			PlayPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
			countDown = (int)GAMEPLAY::GET_GAME_TIMER() + Wait_Time;
		}
		else if (countDown < (int)GAMEPLAY::GET_GAME_TIMER())
		{
			if (DistanceTo(PlayPos, ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false)) < 1.0f)
				b = true;
			else
				countDown = 0;
		}
	}

	return b;
}
bool PlayerDriver()
{
	bool b = false;

	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
	{
		Vehicle PlayVeh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
		if (PLAYER::PLAYER_PED_ID() == VEHICLE::GET_PED_IN_VEHICLE_SEAT(PlayVeh, -1) && !VEHICLE::IS_THIS_MODEL_A_PLANE(PlayVeh))
			b = true;
	}

	return b;
}
bool IsOn = false;
float Rotate_Cam()
{	
	static bool moment = false;
	static float r_Cam = 180.0f;
	if (moment)
	{
		if (r_Cam < 160.0f)
			moment = false;
		else
			r_Cam -= 0.1f;
	}
	else
	{
		if (r_Cam > 200.0f)
			moment = true;
		else
			r_Cam += 0.1f;
	}
	return r_Cam;
} 
void Launch_Adventure(bool isVeh)
{
	IsOn = true;
	InAveh = isVeh;
	if (InAveh)
		AI::TASK_VEHICLE_DRIVE_WANDER(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), 25.0f, 786603);
	else
		AI::TASK_WANDER_STANDARD(PLAYER::PLAYER_PED_ID(), 10.0f, 10);
	Last_Press = 0;
	YourViews = CAM::GET_FOLLOW_PED_CAM_VIEW_MODE();
	CAM::SET_FOLLOW_PED_CAM_VIEW_MODE((Any)2);
}
void main()
{	
	while (true)
	{
		if (!Mod_loaded)
		{
			if (!(bool)DLC2::GET_IS_LOADING_SCREEN_ACTIVE())
				Mod_loaded = true;
			BottomLeft("~b~Adventure with Dementia ~w~by ~r~Adopcalipt");
			FindSettings();
		}
		else
		{
			if (IsKeyDown(VK_DELETE))
				FindSettings();

			if (Mod_Enable)
			{
				if (IsOn)
				{
					if (InPressed())
					{
						if (Go_Wonder)
							StopHere();
						IsOn = false;
					}
					else
						WonderYeOnda(InAveh);
					CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(Rotate_Cam());

				}
				else
				{
					if (MotionTracking())
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
						{
							if (PlayerDriver())
								Launch_Adventure(true);
						}
						else
							Launch_Adventure(false);
					}
				}
			}
		}
		WAIT(1);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
