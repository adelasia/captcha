// captcha — adelasia — CC BY-NC 4.0
modded class DayZGame
{
	bool m_JoinScreenPassed;
	int m_JoinScreenTries = 3;
	bool m_JoinPendingConnect;
	private ref JoinScreenMenu m_JoinScreen;
	private int m_JoinMenuRetries;
	private int m_JoinConnectWaitRetries;
	private Widget m_JoinHudRoot;
	private ImageWidget m_JoinHudGlyphA;
	private ImageWidget m_JoinHudGlyphB;
	private int m_JoinHudCode = -1;

	override void Connect()
	{
		if (!m_JoinScreenPassed)
		{
			m_JoinPendingConnect = true;

			UIManager ui = GetUIManager();
			if ((!ui || !ui.GetMenu()) && m_JoinConnectWaitRetries < 30)
			{
				m_JoinConnectWaitRetries++;
				GetCallQueue(CALL_CATEGORY_GUI).CallLater(RetryConnectForJoinScreen, 500, false);
				return;
			}

			m_JoinConnectWaitRetries = 0;
			DisconnectSessionScript();
			OpenJoinScreen();
			return;
		}

		m_JoinConnectWaitRetries = 0;
		super.Connect();
	}

	void RetryConnectForJoinScreen()
	{
		Connect();
	}

	override void DisconnectSessionScript(bool displayJoinError = false)
	{
		ClearJoinHudDigits();

		if (m_JoinScreen)
		{
			m_JoinScreen.Close();
			m_JoinScreen = null;
		}

		super.DisconnectSessionScript(displayJoinError);
	}

	void OpenJoinScreen()
	{
		if (m_JoinScreenPassed || m_JoinScreen)
			return;

		UIManager ui = GetUIManager();
		if (!ui)
			return;

		if (ui.IsMenuOpen(MENU_JOIN_SCREEN))
			return;

		m_JoinScreen = JoinScreenMenu.Cast(ui.EnterScriptedMenu(MENU_JOIN_SCREEN, ui.GetMenu()));
		if (m_JoinScreen)
		{
			m_JoinMenuRetries = 0;
			m_JoinScreen.BeginRound();
			m_JoinScreen.SetTriesLeft(m_JoinScreenTries);
			return;
		}

		if (m_JoinPendingConnect && m_JoinMenuRetries < 24)
		{
			m_JoinMenuRetries++;
			GetCallQueue(CALL_CATEGORY_GUI).CallLater(OpenJoinScreen, 500, false);
		}
	}

	void OnJoinScreenPassed()
	{
		m_JoinScreenPassed = true;
		m_JoinPendingConnect = false;
		ClearJoinHudDigits();
		GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(TryConnect, 10, false);
	}

	void RequestJoinHudDigits(int code)
	{
		EnsureJoinHudOverlay();
		if (!m_JoinHudGlyphA || !m_JoinHudGlyphB || !m_JoinHudRoot)
			return;

		if (code == m_JoinHudCode && m_JoinHudRoot.IsVisible())
			return;

		string codeText = code.ToStringLen(2);
		m_JoinHudGlyphA.LoadImageFile(0, "set:screencode_set" + " image:glyph_" + codeText.Substring(0, 1));
		m_JoinHudGlyphB.LoadImageFile(0, "set:screencode_set" + " image:glyph_" + codeText.Substring(1, 1));
		m_JoinHudCode = code;
		m_JoinHudRoot.Show(true);
	}

	void ClearJoinHudDigits()
	{
		if (m_JoinHudRoot)
			m_JoinHudRoot.Show(false);

		m_JoinHudCode = -1;
	}

	protected void EnsureJoinHudOverlay()
	{
		if (m_JoinHudRoot)
			return;

		m_JoinHudRoot = GetWorkspace().CreateWidgets("captcha/captcha/gui/layouts/join_glyph_overlay.layout");
		if (!m_JoinHudRoot)
			return;

		m_JoinHudGlyphA = ImageWidget.Cast(m_JoinHudRoot.FindAnyWidget("OverlayGlyphA"));
		m_JoinHudGlyphB = ImageWidget.Cast(m_JoinHudRoot.FindAnyWidget("OverlayGlyphB"));
		m_JoinHudRoot.Show(false);
	}

}