// captcha — adelasia — CC BY-NC 4.0
modded class MissionMainMenu
{
	override void OnMissionStart()
	{
		super.OnMissionStart();

		DayZGame game = DayZGame.Cast(GetGame());
		if (!game || game.m_JoinScreenPassed)
			return;

		string connectAddress;
		if (GetCLIParam("connect", connectAddress) && connectAddress != string.Empty)
			game.m_JoinPendingConnect = true;

		if (!game.m_JoinPendingConnect)
			return;

		game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(game.OpenJoinScreen, 800, false);
	}
}
