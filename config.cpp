class CfgPatches
{
	class captcha
	{
		requiredAddons[] =
		{
			"DZ_Scripts",
			"DZ_Data"
		};
		units[] = {};
		weapons[] = {};
	};
};

class CfgMods
{
	class captcha
	{
		dir = "captcha/captcha";
		hideName = 0;
		hidePicture = 0;
		name = "captcha";
		version = "08.07.25";
		credits = "adelasia";
		author = "adelasia";
		extra = 0;
		type = "mod";
		dependencies[] = { "Game", "World", "Mission" };

		class defs
		{
			class imageSets
			{
				files[] =
				{
					"captcha/captcha/gui/imagesets/screencode_set.imageset"
				};
			};

			class gameScriptModule
			{
				value = "";
				files[] = { "captcha/captcha/scripts/3_Game" };
			};

			class missionScriptModule
			{
				value = "";
				files[] = { "captcha/captcha/scripts/5_Mission" };
			};
		};
	};
};
