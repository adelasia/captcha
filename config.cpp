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
		dir = "captcha";
		hideName = 0;
		hidePicture = 0;
		name = "captcha";
		version = "14.07.26";
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
					"captcha/gui/imagesets/screencode_set.imageset",
					"captcha/gui/imagesets/captcha_title.imageset"
				};
			};

			class gameScriptModule
			{
				value = "";
				files[] = { "captcha/scripts/3_Game" };
			};

			class missionScriptModule
			{
				value = "";
				files[] = { "captcha/scripts/5_Mission" };
			};
		};
	};
};
