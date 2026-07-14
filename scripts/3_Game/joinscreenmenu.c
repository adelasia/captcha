// captcha — adelasia — CC BY-NC 4.0
class JoinScreenMenu extends UIScriptedMenu
{
	private int m_ExpectedCode;
	private string m_CodeValue;
	private bool m_InputFocused;
	private bool m_InputHovered;
	private bool m_QuitHovered;
	private bool m_ConfirmHovered;
	private bool m_QuitConfirmHovered;
	private bool m_Joining;
	protected Widget m_CaptchaTitleRow;
	protected Widget m_AccentLine;
	protected Widget m_ChallengeGlyphField;
	protected Widget m_InputField;
	protected Widget m_InputWrap;
	protected TextWidget m_CaptchaPromptLabel;
	protected TextWidget m_FailLabel;
	protected EditBoxWidget m_CodeDisplay;
	protected Widget m_ConfirmButton;
	protected Widget m_QuitButton;
	protected TextWidget m_QuitWarningLabel;
	protected Widget m_QuitConfirmButton;
	protected TextWidget m_QuitConfirmButtonLabel;
	protected TextWidget m_ConfirmButtonLabel;
	protected TextWidget m_QuitButtonLabel;

	void JoinScreenMenu() {}

	void ~JoinScreenMenu() {}

	override void OnShow()
	{
		super.OnShow();
		PPEffects.SetBlurMenu(0.5);
		GetGame().GetInput().ChangeGameFocus(1);
		SetFocus(layoutRoot);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(EnforceInputLimitTick, 50, true);
	}

	override void OnHide()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(EnforceInputLimitTick);
		super.OnHide();
		PPEffects.SetBlurMenu(0);
		GetGame().GetInput().ResetGameFocus();
		g_Game.ClearJoinHudDigits();
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("captcha/gui/layouts/join_screen_dialog.layout");

		m_CaptchaTitleRow = layoutRoot.FindAnyWidget("CaptchaTitleRow");
		m_AccentLine = layoutRoot.FindAnyWidget("AccentLine");
		m_ChallengeGlyphField = layoutRoot.FindAnyWidget("ChallengeGlyphField");
		m_InputField = layoutRoot.FindAnyWidget("InputField");
		m_InputWrap = layoutRoot.FindAnyWidget("InputWrap");
		m_CaptchaPromptLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("CaptchaPromptLabel"));
		m_FailLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("FailLabel"));
		m_CodeDisplay = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("CodeDisplay"));
		m_ConfirmButton = layoutRoot.FindAnyWidget("ConfirmButton");
		m_QuitButton = layoutRoot.FindAnyWidget("QuitButton");
		m_QuitWarningLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("QuitWarningLabel"));
		m_QuitConfirmButton = layoutRoot.FindAnyWidget("QuitConfirmButton");
		m_QuitConfirmButtonLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("QuitConfirmButtonLabel"));
		m_ConfirmButtonLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("ConfirmButtonLabel"));
		m_QuitButtonLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("QuitButtonLabel"));

		m_CodeValue = string.Empty;
		m_InputFocused = false;
		m_InputHovered = false;
		if (m_CaptchaPromptLabel)
			m_CaptchaPromptLabel.SetText(CAPTCHA_PROMPT);
		ApplyThemeColors();
		ApplyInputColors();
		ApplyButtonColors();
		UpdateCodeDisplay();
		HideFailMessage();
		HideQuitConfirm();

		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		if (m_Joining)
			return false;

		if (!IsQuitButtonWidget(w))
			HideQuitConfirm();

		if (IsInputAreaWidget(w))
		{
			SetInputFocus();
			return true;
		}

		if (m_InputFocused)
			ClearInputFocus();

		if (IsQuitButtonWidget(w))
		{
			ShowQuitConfirm();
			return true;
		}

		if (IsQuitConfirmButtonWidget(w))
		{
			CancelJoin();
			return true;
		}

		if (IsConfirmButtonWidget(w))
		{
			TryConfirmCode();
			return true;
		}

		return false;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (m_Joining)
			return super.OnMouseButtonDown(w, x, y, button);

		if (button == MouseState.LEFT)
		{
			if (IsInputAreaWidget(w))
			{
				SetInputFocus();
				return true;
			}

			if (m_InputFocused)
				ClearInputFocus();

			if (!IsQuitButtonWidget(w))
				HideQuitConfirm();

			if (IsQuitButtonWidget(w))
			{
				ShowQuitConfirm();
				return true;
			}

			if (IsQuitConfirmButtonWidget(w))
			{
				CancelJoin();
				return true;
			}

			if (IsConfirmButtonWidget(w))
			{
				TryConfirmCode();
				return true;
			}
		}

		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		super.OnMouseEnter(w, x, y);

		if (IsInputAreaWidget(w))
		{
			m_InputHovered = true;
			ApplyInputColors();
		}

		if (IsQuitButtonWidget(w))
		{
			m_QuitHovered = true;
			ApplyButtonColors();
		}

		if (IsConfirmButtonWidget(w) && !m_Joining)
		{
			m_ConfirmHovered = true;
			ApplyButtonColors();
		}

		if (IsQuitConfirmButtonWidget(w) && !m_Joining)
		{
			m_QuitConfirmHovered = true;
			ApplyButtonColors();
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		super.OnMouseLeave(w, enterW, x, y);

		if (IsInputAreaWidget(w) && !IsInputAreaWidget(enterW))
		{
			m_InputHovered = false;
			ApplyInputColors();
		}

		if (IsQuitButtonWidget(w) && !IsQuitButtonWidget(enterW))
		{
			m_QuitHovered = false;
			ApplyButtonColors();
		}

		if (IsConfirmButtonWidget(w) && !IsConfirmButtonWidget(enterW) && !m_Joining)
		{
			m_ConfirmHovered = false;
			ApplyButtonColors();
		}

		if (IsQuitConfirmButtonWidget(w) && !IsQuitConfirmButtonWidget(enterW) && !m_Joining)
		{
			m_QuitConfirmHovered = false;
			ApplyButtonColors();
		}

		return false;
	}

	override bool OnKeyDown(Widget w, int x, int y, int key)
	{
		if (m_Joining)
			return super.OnKeyDown(w, x, y, key);

		if (key == KeyCode.KC_RETURN)
		{
			TryConfirmCode();
			return true;
		}

		bool handled = super.OnKeyDown(w, x, y, key);
		UpdateCodeDisplay();
		return handled;
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		bool handled = super.OnChange(w, x, y, finished);
		UpdateCodeDisplay();
		if (finished && w == m_CodeDisplay)
		{
			TryConfirmCode();
			return true;
		}
		return handled;
	}

	private bool IsInputAreaWidget(Widget w)
	{
		if (!w)
			return false;

		return w == m_InputField || w == m_InputWrap || w == m_CodeDisplay;
	}

	private void SetInputFocus()
	{
		m_InputFocused = true;
		if (m_CodeDisplay)
			SetFocus(m_CodeDisplay);
		else
			SetFocus(layoutRoot);
		ApplyInputColors();
		UpdateCodeDisplay();
	}

	private void ClearInputFocus()
	{
		m_InputFocused = false;
		ApplyInputColors();
		UpdateCodeDisplay();
	}

	private void ApplyThemeColors()
	{
		if (m_ChallengeGlyphField)
			m_ChallengeGlyphField.SetColor(ARGB(210, 22, 22, 22));

		if (m_AccentLine)
			m_AccentLine.SetColor(ARGB(255, 207, 28, 34));

		if (m_CodeDisplay)
			m_CodeDisplay.SetColor(ARGB(255, 255, 255, 255));

		if (m_FailLabel)
			m_FailLabel.SetColor(ARGB(255, 239, 37, 37));

		if (m_QuitWarningLabel)
			m_QuitWarningLabel.SetColor(ARGB(255, 239, 37, 37));
	}

	private void ApplyInputColors()
	{
		if (m_InputWrap)
			m_InputWrap.SetColor(ARGB(0, 0, 0, 0));
		if (m_CodeDisplay)
			m_CodeDisplay.SetColor(ARGB(255, 255, 255, 255));
	}

	private bool IsConfirmButtonWidget(Widget w)
	{
		return w == m_ConfirmButton || w == m_ConfirmButtonLabel;
	}

	private bool IsQuitButtonWidget(Widget w)
	{
		return w == m_QuitButton || w == m_QuitButtonLabel;
	}

	private bool IsQuitConfirmButtonWidget(Widget w)
	{
		return w == m_QuitConfirmButton || w == m_QuitConfirmButtonLabel;
	}

	private void ApplyButtonColors()
	{
		if (m_Joining)
		{
			ApplyJoiningButtonStyle();
			return;
		}

		ApplyMenuButtonStyle(m_ConfirmButton, m_ConfirmButtonLabel, m_ConfirmHovered);
		ApplyMenuButtonStyle(m_QuitButton, m_QuitButtonLabel, m_QuitHovered);
		ApplyMenuButtonStyle(m_QuitConfirmButton, m_QuitConfirmButtonLabel, m_QuitConfirmHovered);
	}

	private void ApplyJoiningButtonStyle()
	{
		if (m_ConfirmButton)
			m_ConfirmButton.SetColor(ARGB(210, 22, 22, 22));

		if (m_ConfirmButtonLabel)
			m_ConfirmButtonLabel.SetColor(ARGB(255, 255, 255, 255));
	}

	private void ApplyMenuButtonStyle(Widget btn, TextWidget label, bool hovered)
	{
		if (!btn || IsWidgetDisabled(btn))
			return;

		if (hovered)
		{
			btn.SetColor(ARGB(255, 0, 0, 0));
			if (label)
				label.SetColor(ARGB(255, 239, 37, 37));
		}
		else
		{
			btn.SetColor(ARGB(210, 22, 22, 22));
			if (label)
				label.SetColor(ARGB(255, 255, 255, 255));
		}
	}

	private bool IsWidgetDisabled(Widget w)
	{
		return w && (w.GetFlags() & WidgetFlags.DISABLED);
	}

	private string GetCodeText()
	{
		UpdateCodeDisplay();
		return m_CodeValue;
	}

	private void ClearCodeInput()
	{
		m_CodeValue = string.Empty;
		if (m_CodeDisplay)
			m_CodeDisplay.SetText(string.Empty);
	}

	private string SanitizeCodeText(string raw)
	{
		string result = string.Empty;
		for (int i = 0; i < raw.Length(); i++)
		{
			string ch = raw.Substring(i, 1);
			if (ch >= "0" && ch <= "9")
			{
				result += ch;
				if (result.Length() >= 2)
					break;
			}
		}
		return result;
	}

	private void UpdateCodeDisplay()
	{
		if (!m_CodeDisplay)
			return;

		string raw = m_CodeDisplay.GetText();
		string cleaned = SanitizeCodeText(raw);
		if (cleaned != raw)
			m_CodeDisplay.SetText(cleaned);
		m_CodeValue = cleaned;
		m_CodeDisplay.Show(true);
	}

	private void EnforceInputLimitTick()
	{
		UpdateCodeDisplay();
	}

	private void TryConfirmCode()
	{
		if (m_Joining)
			return;

		string answer = GetCodeText();
		if (answer == string.Empty)
		{
			ShowEmptyInputMessage();
			return;
		}

		SubmitCode(answer.ToInt());
	}

	private void SubmitCode(int answer)
	{
		if (g_Game.m_JoinScreenPassed)
			return;

		if (answer == m_ExpectedCode)
			OnCodeAccepted();
		else
			OnCodeRejected();
	}

	private void OnCodeAccepted()
	{
		ShowJoiningState();
		g_Game.OnJoinScreenPassed();
	}

	private void ShowJoiningState()
	{
		m_Joining = true;
		m_ConfirmHovered = false;
		m_QuitHovered = false;
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(EnforceInputLimitTick);
		ClearInputFocus();
		HideFailMessage();
		g_Game.ClearJoinHudDigits();

		if (m_CaptchaTitleRow)
			m_CaptchaTitleRow.Show(false);

		if (m_ChallengeGlyphField)
			m_ChallengeGlyphField.Show(false);

		if (m_AccentLine)
			m_AccentLine.Show(false);

		if (m_FailLabel)
			m_FailLabel.Show(false);

		if (m_QuitButton)
			m_QuitButton.Show(false);

		HideQuitConfirm();

		if (m_ConfirmButtonLabel)
			m_ConfirmButtonLabel.SetText("joining..");

		if (m_ConfirmButton)
			m_ConfirmButton.SetFlags(WidgetFlags.DISABLED);

		ApplyJoiningButtonStyle();
	}

	private void OnCodeRejected()
	{
		SetTriesLeft(g_Game.m_JoinScreenTries - 1);
		if (g_Game.m_JoinScreenTries > 0)
		{
			ClearCodeInput();
			UpdateCodeDisplay();
			ClearInputFocus();
			m_ExpectedCode = Math.RandomInt(0, 100);
			UpdateChallengeDigits(m_ExpectedCode);
			ShowFailMessage();
		}
		else
		{
			FailOut();
		}
	}

	private void ShowEmptyInputMessage()
	{
		if (!m_FailLabel)
			return;

		m_FailLabel.SetText("Please enter numbers.");
		m_FailLabel.Show(true);
	}

	private void ShowFailMessage()
	{
		if (!m_FailLabel)
			return;

		int triesLeft = g_Game.m_JoinScreenTries;
		if (triesLeft == 1)
			m_FailLabel.SetText("Wrong answer, 1 try left.");
		else
			m_FailLabel.SetText(string.Format("Wrong answer, %1 tries left.", triesLeft));

		m_FailLabel.Show(true);
	}

	private void HideFailMessage()
	{
		if (m_FailLabel)
			m_FailLabel.Show(false);
	}

	private void ShowQuitConfirm()
	{
		if (m_QuitWarningLabel)
		{
			m_QuitWarningLabel.SetText(CAPTCHA_EXIT_WARNING);
			m_QuitWarningLabel.Show(true);
		}

		if (m_QuitConfirmButton)
			m_QuitConfirmButton.Show(true);
	}

	private void HideQuitConfirm()
	{
		m_QuitConfirmHovered = false;

		if (m_QuitWarningLabel)
			m_QuitWarningLabel.Show(false);

		if (m_QuitConfirmButton)
			m_QuitConfirmButton.Show(false);

		ApplyButtonColors();
	}

	private void CancelJoin()
	{
		ExitJoinScreen();
	}

	private void FailOut()
	{
		ExitJoinScreen();
	}

	private void ExitJoinScreen()
	{
		g_Game.m_JoinPendingConnect = false;
		g_Game.m_JoinScreenPassed = false;
		g_Game.m_JoinScreenTries = 3;
		g_Game.RequestExit(0);
	}

	void BeginRound()
	{
		m_Joining = false;
		ClearCodeInput();
		m_InputFocused = false;
		m_InputHovered = false;
		m_QuitHovered = false;
		m_ConfirmHovered = false;
		m_QuitConfirmHovered = false;

		if (m_CaptchaTitleRow)
			m_CaptchaTitleRow.Show(true);

		if (m_ChallengeGlyphField)
			m_ChallengeGlyphField.Show(true);

		if (m_AccentLine)
			m_AccentLine.Show(true);

		if (m_QuitButton)
		{
			m_QuitButton.ClearFlags(WidgetFlags.DISABLED);
			m_QuitButton.Show(true);
		}

		if (m_ConfirmButton)
			m_ConfirmButton.ClearFlags(WidgetFlags.DISABLED);

		if (m_ConfirmButtonLabel)
		{
			m_ConfirmButtonLabel.SetText("continue");
			m_ConfirmButtonLabel.SetColor(ARGB(255, 255, 255, 255));
		}

		ApplyInputColors();
		ApplyButtonColors();
		UpdateCodeDisplay();
		HideFailMessage();
		HideQuitConfirm();
		m_ExpectedCode = Math.RandomInt(0, 100);
		UpdateChallengeDigits(m_ExpectedCode);
		SetInputFocus();
	}

	private void UpdateChallengeDigits(int code)
	{
		m_ExpectedCode = code;
		g_Game.RequestJoinHudDigits(code);
	}

	void SetTriesLeft(int tries)
	{
		g_Game.m_JoinScreenTries = tries;
	}
}
