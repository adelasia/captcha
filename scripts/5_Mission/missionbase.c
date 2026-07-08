// captcha — adelasia — CC BY-NC 4.0
modded class MissionBase
{
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		if (id == MENU_JOIN_SCREEN)
		{
			UIScriptedMenu menu = new JoinScreenMenu;
			menu.SetID(id);
			return menu;
		}

		return super.CreateScriptedMenu(id);
	}
}
