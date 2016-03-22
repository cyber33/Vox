// ******************************************************************************
// Filename:    OptionsMenu.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 12/10/14
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "ModMenu.h"
#include "../../VoxGame.h"
#include "../FrontendManager.h"
#include "../../gui/openglgui.h"
#include "../../utils/FileUtils.h"
#include "../../Mods/ModsManager.h"


ModMenu::ModMenu(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontPageManager, int windowWidth, int windowHeight)
	: FrontendPage(pRenderer, pGUI, pFrontPageManager, FrontendScreen_ModMenu, windowWidth, windowHeight)
{
	// Mod Window
	m_pModWindow = new GUIWindow(m_pRenderer, m_pFrontendManager->GetFrontendFont_Medium(), "Mods");
	m_pModWindow->AllowMoving(true);
	m_pModWindow->AllowClosing(false);
	m_pModWindow->AllowMinimizing(false);
	m_pModWindow->AllowScrolling(false);
	m_pModWindow->SetRenderTitleBar(true);
	m_pModWindow->SetRenderWindowBackground(true);
	m_pModWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pModWindow->Hide();

	m_pTitleBarIcon = new Icon(m_pRenderer, "", 44, 44);
	m_pTitleBarIcon->SetDepth(4.0f);

	m_pModWindowBackgroundIcon = new Icon(m_pRenderer, "", 600, 475);
	m_pModWindowBackgroundIcon->SetDepth(1.0f);

	m_pTitleBarBackgroundIcon = new Icon(m_pRenderer, "", 86, 35);
	m_pTitleBarBackgroundIcon->SetDepth(1.0f);

	m_pCloseExitButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pCloseExitButton->SetLabelOffset(0, 5);
	m_pCloseExitButton->SetCallBackFunction(_CloseExitPressed);
	m_pCloseExitButton->SetCallBackData(this);

	m_pModWindow->AddComponent(m_pTitleBarIcon);
	m_pModWindow->AddComponent(m_pCloseExitButton);

	// Tabs
	m_pModsModeController = new OptionController(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), "Mode");
	m_pModsModeController->SetDisplayLabel(false);
	m_pModsModeController->SetDisplayBorder(false);

	m_pGameplayMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "Gameplay", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGameplayMode->SetCallBackFunction(_GameplayTabPressed);
	m_pGameplayMode->SetCallBackData(this);
	m_pGameplayMode->SetDepth(2.0f);
	m_pGameplayMode->SetPressedOffset(0, -2);
	//m_pGameplayMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pGameplayMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pGraphicsMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "Graphics", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGraphicsMode->SetCallBackFunction(_GraphicsTabPressed);
	m_pGraphicsMode->SetCallBackData(this);
	m_pGraphicsMode->SetDepth(2.1f);
	m_pGraphicsMode->SetPressedOffset(0, -2);
	//m_pGraphicsMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pGraphicsMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pSoundMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "Sound", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pSoundMode->SetCallBackFunction(_SoundTabPressed);
	m_pSoundMode->SetCallBackData(this);
	m_pSoundMode->SetDepth(2.2f);
	m_pSoundMode->SetPressedOffset(0, -2);
	//m_pSoundMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pSoundMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pHUDMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "HUD", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pHUDMode->SetCallBackFunction(_GUITabPressed);
	m_pHUDMode->SetCallBackData(this);
	m_pHUDMode->SetDepth(2.3f);
	m_pHUDMode->SetPressedOffset(0, -2);
	//m_pHUDMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pHUDMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pMiscMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "Misc", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pMiscMode->SetCallBackFunction(_MiscTabPressed);
	m_pMiscMode->SetCallBackData(this);
	m_pMiscMode->SetDepth(2.4f);
	m_pMiscMode->SetPressedOffset(0, -2);
	//m_pMiscMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pMiscMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	// Scrollbar
	m_pModsScrollbar = new ScrollBar(m_pRenderer);
	m_pModsScrollbar->SetScrollDirection(EScrollBarDirection_Vertical);
	m_pModsScrollbar->SetScrollSize(0.25f);
	m_pModsScrollbar->SetScrollPosition(1.0f);
	m_pModsScrollbar->SetDepth(4.0f);
	m_pModsScrollbar->SetScissorEnabled(true);

	m_pModsModeController->Add(m_pGameplayMode);
	m_pModsModeController->Add(m_pGraphicsMode);
	m_pModsModeController->Add(m_pSoundMode);
	m_pModsModeController->Add(m_pHUDMode);
	m_pModsModeController->Add(m_pMiscMode);	
	m_pGameplayMode->SetToggled(true);

	m_pModWindow->AddComponent(m_pModsModeController);
	m_pModWindow->AddComponent(m_pModsScrollbar);

	SetWindowDimensions(m_windowWidth, m_windowHeight);

	m_returnToMainMenu = false;
}

ModMenu::~ModMenu()
{
	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	delete m_pModWindow;
	delete m_pTitleBarIcon;
	delete m_pTitleBarBackgroundIcon;
	delete m_pModWindowBackgroundIcon;
	delete m_pCloseExitButton;

	// Tabs
	delete m_pModsModeController;
	delete m_pGameplayMode;
	delete m_pGraphicsMode;
	delete m_pSoundMode;
	delete m_pHUDMode;
	delete m_pMiscMode;

	// Scrollbar
	delete m_pModsScrollbar;
}

void ModMenu::Reset()
{
}

void ModMenu::ClearModButtonData()
{
	for (unsigned int i = 0; i < m_vpModButtonData.size(); i++)
	{
		delete m_vpModButtonData[i];
		m_vpModButtonData[i] = 0;
	}
	m_vpModButtonData.clear();
}

void ModMenu::SetWindowDimensions(int windowWidth, int windowHeight)
{
	FrontendPage::SetWindowDimensions(windowWidth, windowHeight);

	m_modButtonWidth = 170;
	m_modButtonheight = 150;
	m_modButtonSpace = 15;

	m_modWindowWidth = 600;
	m_modWindowHeight = 510;
	m_titlebarWidth = 118;
	m_titlebarHeight = 35;

	m_pModWindow->SetDimensions((int)((m_windowWidth*0.5f) - (m_modWindowWidth*0.5f)), (int)((m_windowHeight*0.5f) - (m_modWindowHeight*0.5f)), m_modWindowWidth, m_modWindowHeight);
	m_pModWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);
	m_pModWindow->SetTitleOffset(50, 5);
	m_pModWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pModWindow->SetApplicationBorder(25, 15, 10, 10);

	m_pTitleBarIcon->SetDimensions(0, m_modWindowHeight, 44, 44);

	m_pCloseExitButton->SetDimensions(m_modWindowWidth-32, m_modWindowHeight, 32, 32);
	
	// Tabs
	m_pModsModeController->SetDimensions(0, m_modWindowHeight, m_modWindowWidth, 32);
	m_pGameplayMode->SetDisplayLabel(false);
	m_pGameplayMode->SetDimensions(m_modWindowWidth-32-375, 0, 75, 32);
	m_pGameplayMode->SetDisplayLabel(true);
	int textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pGameplayMode->GetLabel().GetText().c_str());
	m_pGameplayMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pGraphicsMode->SetDisplayLabel(false);
	m_pGraphicsMode->SetDimensions(m_modWindowWidth-32-300, 0, 75, 32);
	m_pGraphicsMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pGraphicsMode->GetLabel().GetText().c_str());
	m_pGraphicsMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pSoundMode->SetDisplayLabel(false);
	m_pSoundMode->SetDimensions(m_modWindowWidth-32-225, 0, 75, 32);
	m_pSoundMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pSoundMode->GetLabel().GetText().c_str());
	m_pSoundMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pHUDMode->SetDisplayLabel(false);
	m_pHUDMode->SetDimensions(m_modWindowWidth-32-150, 0, 75, 32);
	m_pHUDMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pHUDMode->GetLabel().GetText().c_str());
	m_pHUDMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pMiscMode->SetDisplayLabel(false);
	m_pMiscMode->SetDimensions(m_modWindowWidth-32-75, 0, 75, 32);
	m_pMiscMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pMiscMode->GetLabel().GetText().c_str());
	m_pMiscMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	// Scrollbar
	int scrollbarHeight = m_modWindowHeight-6;
	m_pModsScrollbar->SetDimensions(m_modWindowWidth-28, 4, 24, scrollbarHeight);
	m_pModsScrollbar->SetScrollArea(-(m_modWindowWidth-40), 0, m_modWindowWidth-40, scrollbarHeight);
}

// Pre-tick the already loaded mods and set them as toggled
void ModMenu::SelectLoadedMods()
{
	int buttonWidth = m_modButtonWidth;

	ModsManager* pModsManager = VoxGame::GetInstance()->GetModsManager();

	for (int i = 0; i < pModsManager->GetNumMods(); i++)
	{
		Mod* pMod = pModsManager->GetMod(i);

		for (int j = 0; j < m_vpModButtonData.size(); j++)
		{
			if (pMod->m_gameplayMod && m_vpModButtonData[j]->m_gameplayButton ||
				pMod->m_graphicsMod && m_vpModButtonData[j]->m_graphicsButton ||
				pMod->m_soundMod && m_vpModButtonData[j]->m_soundButton ||
				pMod->m_HUDMod && m_vpModButtonData[j]->m_HUDButton ||
				pMod->m_miscMod && m_vpModButtonData[j]->m_miscButton)
			{
				if (pMod->m_modName == m_vpModButtonData[j]->m_modName)
				{
					m_vpModButtonData[j]->m_toggled = true;
					string themeName = VoxGame::GetInstance()->GetModsManager()->GetHUDTextureTheme();
					string tickIcon = "media/textures/gui/" + themeName + "/common/tick.tga";
					m_vpModButtonData[j]->m_pModButton->AddIcon(m_pRenderer, tickIcon.c_str(), 32, 32, 32, 32, buttonWidth - 38, 4, 2.0f);
				}
			}
		}
	}
}

// Are we returning to the main menu, or are we in the game?
void ModMenu::SetReturnToMainMenu(bool mainMenu)
{
	m_returnToMainMenu = mainMenu;
}

// Skinning
void ModMenu::SkinGUI()
{
	string themeName = VoxGame::GetInstance()->GetModsManager()->GetHUDTextureTheme();

	string iconName = "media/textures/gui/" + themeName + "/ModMenu/mod_title_icon.tga";
	m_pTitleBarIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/ModMenu/mod_window_background.tga";
	m_pModWindowBackgroundIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/ModMenu/titlebar_background.tga";
	m_pTitleBarBackgroundIcon->SetIcon(iconName);

	m_pModWindow->SetBackgroundIcon(m_pModWindowBackgroundIcon);
	m_pModWindow->SetTitlebarBackgroundIcon(m_pTitleBarBackgroundIcon);

	Point location = m_pModWindow->GetLocation();
	m_pModWindow->SetDimensions(location.m_x, location.m_y, m_modWindowWidth, m_modWindowHeight);
	m_pModWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);

	m_pFrontendManager->SetScrollbarIcons(m_pModsScrollbar);

	m_pCloseExitButton->SetDefaultIcon(m_pFrontendManager->GetCloseExitButtonIcon());
	m_pCloseExitButton->SetHoverIcon(m_pFrontendManager->GetCloseExitButtonIcon_Hover());
	m_pCloseExitButton->SetSelectedIcon(m_pFrontendManager->GetCloseExitButtonIcon_Pressed());
	m_pCloseExitButton->SetDisabledIcon(m_pFrontendManager->GetCloseExitButtonIcon());
}

void ModMenu::UnSkinGUI()
{
	m_pModsScrollbar->SetDefaultIcons(m_pRenderer);
}

// Load unload
void ModMenu::Load()
{
	m_pGameplayMode->SetToggled(true);
	GameplayTabPressed();

	m_loaded = true;
}

void ModMenu::Unload()
{
	VoxGame::GetInstance()->GetModsManager()->SaveMods();

	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	m_pGUI->RemoveWindow(m_pModWindow);

	ClearModButtonData();

	m_loaded = false;
}

void ModMenu::CreateGameplayModButtons()
{
	int buttonWidth = m_modButtonWidth;
	int buttonHeight = m_modButtonheight;
	int buttonSpacer = m_modButtonSpace;
	int buttonAndSpacerWidth = buttonWidth + buttonSpacer;
	int buttonAndSpacerHeight = buttonHeight + buttonSpacer;

	int buttonX = -(m_modWindowWidth - 42);
	int buttonY = m_modWindowHeight - buttonHeight - 17;
	for (int y = 0; y < 4; y++)
	{
		buttonX = -(m_modWindowWidth - 42);

		for (int x = 0; x < 3; x++)
		{
			Button* m_pNewButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_35(), "");
			m_pNewButton->SetDimensions(buttonX, buttonY, buttonWidth, buttonHeight);
			m_pNewButton->SetPressedOffset(0, -1);

			char buttonText[64];
			sprintf(buttonText, "TestButton: %i,%i", x, y);
			m_pNewButton->AddText(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), buttonText, Colour(1.0f, 1.0f, 1.0f, 1.0f), 7, buttonHeight - 20, true, Colour(0.0f, 0.0f, 0.0f, 1.0f));

			ModButtonData* pModButtonData = new ModButtonData();
			pModButtonData->m_pModMenu = this;
			pModButtonData->m_pModButton = m_pNewButton;
			pModButtonData->m_modName = buttonText;
			pModButtonData->m_toggled = false;
			pModButtonData->m_allowToggleOff = true;
			pModButtonData->m_allowMultipleSelection = true;
			pModButtonData->m_gameplayButton = true;
			pModButtonData->m_graphicsButton = false;
			pModButtonData->m_soundButton = false;
			pModButtonData->m_HUDButton = false;
			pModButtonData->m_miscButton = false;

			m_pNewButton->SetCallBackFunction(_ModButtonPressed);
			m_pNewButton->SetCallBackData(pModButtonData);

			m_vpModButtonData.push_back(pModButtonData);

			m_pModsScrollbar->AddScrollAreaItem(m_pNewButton);

			m_vpGameplayModButtons.push_back(m_pNewButton);

			buttonX += buttonAndSpacerWidth;
		}

		buttonY -= buttonAndSpacerHeight;
	}
}

void ModMenu::RemoveGameplayModButtons()
{
	m_pModsScrollbar->ClearScrollAreaItems();
	for (unsigned int i = 0; i < m_vpGameplayModButtons.size(); i++)
	{
		delete m_vpGameplayModButtons[i];
		m_vpGameplayModButtons[i] = 0;
	}
	m_vpGameplayModButtons.clear();
}

void ModMenu::CreateGraphicsModButtons()
{
	int buttonWidth = m_modButtonWidth;
	int buttonHeight = m_modButtonheight;
	int buttonSpacer = m_modButtonSpace;
	int buttonAndSpacerWidth = buttonWidth + buttonSpacer;
	int buttonAndSpacerHeight = buttonHeight + buttonSpacer;

	int buttonX = -(m_modWindowWidth - 42);
	int buttonY = m_modWindowHeight - buttonHeight - 17;

	char importDirectory[128];
	sprintf(importDirectory, "media/graphics/*.*");

	vector<string> listFiles;
	listFiles = listFilesInDirectory(importDirectory);
	int modButtonCounter = 0;
	int yCounter = 0;
	while (modButtonCounter < listFiles.size())
	{
		if (strcmp(listFiles[modButtonCounter].c_str(), ".") == 0 || strcmp(listFiles[modButtonCounter].c_str(), "..") == 0)
		{
			modButtonCounter++;
			continue;
		}

		buttonX = -(m_modWindowWidth - 42);

		for (int x = 0; x < 3 && modButtonCounter < listFiles.size(); x++)
		{
			Button* m_pNewButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_35(), "");
			m_pNewButton->SetDimensions(buttonX, buttonY, buttonWidth, buttonHeight);
			m_pNewButton->SetPressedOffset(0, -1);

			m_pNewButton->AddText(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), listFiles[modButtonCounter].c_str(), Colour(1.0f, 1.0f, 1.0f, 1.0f), 7, buttonHeight - 20, true, Colour(0.0f, 0.0f, 0.0f, 1.0f));

			ModButtonData* pModButtonData = new ModButtonData();
			pModButtonData->m_pModMenu = this;
			pModButtonData->m_pModButton = m_pNewButton;
			pModButtonData->m_modName = listFiles[modButtonCounter];
			pModButtonData->m_toggled = false;
			pModButtonData->m_allowToggleOff = false;
			pModButtonData->m_allowMultipleSelection = false;
			pModButtonData->m_gameplayButton = false;
			pModButtonData->m_graphicsButton = true;
			pModButtonData->m_soundButton = false;
			pModButtonData->m_HUDButton = false;
			pModButtonData->m_miscButton = false;

			m_pNewButton->SetCallBackFunction(_ModButtonPressed);
			m_pNewButton->SetCallBackData(pModButtonData);

			m_vpModButtonData.push_back(pModButtonData);

			m_pModsScrollbar->AddScrollAreaItem(m_pNewButton);

			m_vpGraphicsModButtons.push_back(m_pNewButton);

			buttonX += buttonAndSpacerWidth;

			modButtonCounter++;
		}

		yCounter++;

		buttonY -= buttonAndSpacerHeight;
	}
}

void ModMenu::RemoveGraphicsModButtons()
{
	m_pModsScrollbar->ClearScrollAreaItems();
	for (unsigned int i = 0; i < m_vpGraphicsModButtons.size(); i++)
	{
		delete m_vpGraphicsModButtons[i];
		m_vpGraphicsModButtons[i] = 0;
	}
	m_vpGraphicsModButtons.clear();
}

void ModMenu::CreateSoundModButtons()
{
	int buttonWidth = m_modButtonWidth;
	int buttonHeight = m_modButtonheight;
	int buttonSpacer = m_modButtonSpace;
	int buttonAndSpacerWidth = buttonWidth + buttonSpacer;
	int buttonAndSpacerHeight = buttonHeight + buttonSpacer;

	int buttonX = -(m_modWindowWidth - 42);
	int buttonY = m_modWindowHeight - buttonHeight - 17;

	char importDirectory[128];
	sprintf(importDirectory, "media/audio/*.*");

	vector<string> listFiles;
	listFiles = listFilesInDirectory(importDirectory);
	int modButtonCounter = 0;
	int yCounter = 0;
	while (modButtonCounter < listFiles.size())
	{
		if (strcmp(listFiles[modButtonCounter].c_str(), ".") == 0 || strcmp(listFiles[modButtonCounter].c_str(), "..") == 0)
		{
			modButtonCounter++;
			continue;
		}

		buttonX = -(m_modWindowWidth - 42);

		for (int x = 0; x < 3 && modButtonCounter < listFiles.size(); x++)
		{
			Button* m_pNewButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_35(), "");
			m_pNewButton->SetDimensions(buttonX, buttonY, buttonWidth, buttonHeight);
			m_pNewButton->SetPressedOffset(0, -1);

			m_pNewButton->AddText(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), listFiles[modButtonCounter].c_str(), Colour(1.0f, 1.0f, 1.0f, 1.0f), 7, buttonHeight - 20, true, Colour(0.0f, 0.0f, 0.0f, 1.0f));

			ModButtonData* pModButtonData = new ModButtonData();
			pModButtonData->m_pModMenu = this;
			pModButtonData->m_pModButton = m_pNewButton;
			pModButtonData->m_modName = listFiles[modButtonCounter];
			pModButtonData->m_toggled = false;
			pModButtonData->m_allowToggleOff = false;
			pModButtonData->m_allowMultipleSelection = false;
			pModButtonData->m_gameplayButton = false;
			pModButtonData->m_graphicsButton = false;
			pModButtonData->m_soundButton = true;
			pModButtonData->m_HUDButton = false;
			pModButtonData->m_miscButton = false;

			m_pNewButton->SetCallBackFunction(_ModButtonPressed);
			m_pNewButton->SetCallBackData(pModButtonData);

			m_vpModButtonData.push_back(pModButtonData);

			m_pModsScrollbar->AddScrollAreaItem(m_pNewButton);

			m_vpSoundModButtons.push_back(m_pNewButton);

			buttonX += buttonAndSpacerWidth;

			modButtonCounter++;
		}

		yCounter++;

		buttonY -= buttonAndSpacerHeight;
	}
}

void ModMenu::RemoveSoundModButtons()
{
	m_pModsScrollbar->ClearScrollAreaItems();
	for (unsigned int i = 0; i < m_vpSoundModButtons.size(); i++)
	{
		delete m_vpSoundModButtons[i];
		m_vpSoundModButtons[i] = 0;
	}
	m_vpSoundModButtons.clear();
}

void ModMenu::CreateHUDModButtons()
{
	int buttonWidth = m_modButtonWidth;
	int buttonHeight = m_modButtonheight;
	int buttonSpacer = m_modButtonSpace;
	int buttonAndSpacerWidth = buttonWidth + buttonSpacer;
	int buttonAndSpacerHeight = buttonHeight + buttonSpacer;

	int buttonX = -(m_modWindowWidth-42);
	int buttonY = m_modWindowHeight - buttonHeight - 17;

	char importDirectory[128];
	sprintf(importDirectory, "media/textures/gui/*.*");

	vector<string> listFiles;
	listFiles = listFilesInDirectory(importDirectory);
	int modButtonCounter = 0;
	int yCounter = 0;
	while (modButtonCounter < listFiles.size())
	{
		if (strcmp(listFiles[modButtonCounter].c_str(), ".") == 0 || strcmp(listFiles[modButtonCounter].c_str(), "..") == 0)
		{
			modButtonCounter++;
			continue;
		}

		buttonX = -(m_modWindowWidth - 42);

		for (int x = 0; x < 3 && modButtonCounter < listFiles.size(); x++)
		{
			Button* m_pNewButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_35(), "");
			m_pNewButton->SetDimensions(buttonX, buttonY, buttonWidth, buttonHeight);
			m_pNewButton->SetPressedOffset(0, -1);

			m_pNewButton->AddText(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), listFiles[modButtonCounter].c_str(), Colour(1.0f, 1.0f, 1.0f, 1.0f), 7, buttonHeight - 20, true, Colour(0.0f, 0.0f, 0.0f, 1.0f));

			ModButtonData* pModButtonData = new ModButtonData();
			pModButtonData->m_pModMenu = this;
			pModButtonData->m_pModButton = m_pNewButton;
			pModButtonData->m_modName = listFiles[modButtonCounter];
			pModButtonData->m_toggled = false;
			pModButtonData->m_allowToggleOff = false;
			pModButtonData->m_allowMultipleSelection = false;
			pModButtonData->m_gameplayButton = false;
			pModButtonData->m_graphicsButton = false;
			pModButtonData->m_soundButton = false;
			pModButtonData->m_HUDButton = true;
			pModButtonData->m_miscButton = false;

			m_pNewButton->SetCallBackFunction(_ModButtonPressed);
			m_pNewButton->SetCallBackData(pModButtonData);

			m_vpModButtonData.push_back(pModButtonData);

			m_pModsScrollbar->AddScrollAreaItem(m_pNewButton);

			m_vpHUDModButtons.push_back(m_pNewButton);

			buttonX += buttonAndSpacerWidth;

			modButtonCounter++;
		}

		yCounter++;

		buttonY -= buttonAndSpacerHeight;
	}
}

void ModMenu::RemoveHUDModButtons()
{
	m_pModsScrollbar->ClearScrollAreaItems();
	for(unsigned int i = 0; i < m_vpHUDModButtons.size(); i++)
	{
		delete m_vpHUDModButtons[i];
		m_vpHUDModButtons[i] = 0;
	}
	m_vpHUDModButtons.clear();
}

void ModMenu::CreateMiscModButtons()
{
	int buttonWidth = m_modButtonWidth;
	int buttonHeight = m_modButtonheight;
	int buttonSpacer = m_modButtonSpace;
	int buttonAndSpacerWidth = buttonWidth + buttonSpacer;
	int buttonAndSpacerHeight = buttonHeight + buttonSpacer;

	int buttonX = -(m_modWindowWidth - 42);
	int buttonY = m_modWindowHeight - buttonHeight - 17;
	for (int y = 0; y < 4; y++)
	{
		buttonX = -(m_modWindowWidth - 42);

		for (int x = 0; x < 3; x++)
		{
			Button* m_pNewButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_35(), "");
			m_pNewButton->SetDimensions(buttonX, buttonY, buttonWidth, buttonHeight);
			m_pNewButton->SetPressedOffset(0, -1);

			char buttonText[64];
			sprintf(buttonText, "TestButton: %i,%i", x, y);
			m_pNewButton->AddText(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), buttonText, Colour(1.0f, 1.0f, 1.0f, 1.0f), 7, buttonHeight - 20, true, Colour(0.0f, 0.0f, 0.0f, 1.0f));

			ModButtonData* pModButtonData = new ModButtonData();
			pModButtonData->m_pModMenu = this;
			pModButtonData->m_pModButton = m_pNewButton;
			pModButtonData->m_modName = buttonText;
			pModButtonData->m_toggled = false;
			pModButtonData->m_allowToggleOff = true;
			pModButtonData->m_allowMultipleSelection = true;
			pModButtonData->m_gameplayButton = false;
			pModButtonData->m_graphicsButton = false;
			pModButtonData->m_soundButton = false;
			pModButtonData->m_HUDButton = false;
			pModButtonData->m_miscButton = true;

			m_pNewButton->SetCallBackFunction(_ModButtonPressed);
			m_pNewButton->SetCallBackData(pModButtonData);

			m_vpModButtonData.push_back(pModButtonData);

			m_pModsScrollbar->AddScrollAreaItem(m_pNewButton);

			m_vpMiscModButtons.push_back(m_pNewButton);

			buttonX += buttonAndSpacerWidth;
		}

		buttonY -= buttonAndSpacerHeight;
	}
}

void ModMenu::RemoveMiscModButtons()
{
	m_pModsScrollbar->ClearScrollAreaItems();
	for (unsigned int i = 0; i < m_vpMiscModButtons.size(); i++)
	{
		delete m_vpMiscModButtons[i];
		m_vpMiscModButtons[i] = 0;
	}
	m_vpMiscModButtons.clear();
}

// Update
void ModMenu::Update(float dt)
{
	FrontendPage::Update(dt);

	if (VoxGame::GetInstance()->IsPaused() == false)
	{
		float orbitTimer = m_pFrontendManager->GetCameraOrbitTimer();
		orbitTimer += dt;

		float orbitTime = 120.0f;
		if (orbitTimer >= orbitTime)
		{
			orbitTimer = 0.0f;
		}
		m_pFrontendManager->SetCameraOrbitTimer(orbitTimer);

		float ratio = orbitTimer / orbitTime;
		float radius = 125.0f;
		float angle = DegToRad((ratio * 360.0f));
		vec3 position = vec3(cos(angle) * radius, radius*0.75f, sin(angle) * radius);

		m_cameraPosition = position;
		m_cameraView = vec3(0.0f, 8.0f, 0.0f);
	}

	VoxGame::GetInstance()->SetGlobalBlurAmount(0.00125f);

	int buttonWidth = m_modButtonWidth;
	int buttonHeight = m_modButtonheight;
	int buttonSpacer = m_modButtonSpace;
	int buttonAndSpacer = buttonHeight + buttonSpacer;

	// Gameplay
	if (m_pGameplayMode->GetToggled())
	{
		int visibleSize = m_pModsScrollbar->GetScrollArea().m_height;
		int numYLines = (int)ceil((float)m_vpGameplayModButtons.size() / 3.0f);
		int neededHeight = numYLines * buttonAndSpacer + 4;
		int heightDiff = neededHeight - visibleSize;
		if (heightDiff > 0)
		{
			m_pModsScrollbar->SetScrollSize((float)visibleSize / (float)neededHeight);
			m_pModsScrollbar->SetDisabled(false);

			float scrollRatio = m_pModsScrollbar->GetScrollRatio();
			int offsetButtonY = (int)(heightDiff * scrollRatio);

			int modButtonCounter = 0;
			int yCounter = 0;
			while (modButtonCounter < m_vpGameplayModButtons.size())
			{
				for (int x = 0; x < 3 && modButtonCounter < m_vpGameplayModButtons.size(); x++)
				{
					int yPos = m_modWindowHeight - buttonHeight - 17 - (buttonAndSpacer * yCounter);
					Point position = m_vpGameplayModButtons[modButtonCounter]->GetLocation();
					m_vpGameplayModButtons[modButtonCounter]->SetLocation(position.m_x, yPos + offsetButtonY);

					modButtonCounter++;
				}

				yCounter++;
			}
		}
		else
		{
			m_pModsScrollbar->SetScrollSize(0.0f);
			m_pModsScrollbar->SetDisabled(true);
		}
	}

	// Graphics
	if (m_pGraphicsMode->GetToggled())
	{
		int visibleSize = m_pModsScrollbar->GetScrollArea().m_height;
		int numYLines = (int)ceil((float)m_vpGraphicsModButtons.size() / 3.0f);
		int neededHeight = numYLines * buttonAndSpacer + 4;
		int heightDiff = neededHeight - visibleSize;
		if (heightDiff > 0)
		{
			m_pModsScrollbar->SetScrollSize((float)visibleSize / (float)neededHeight);
			m_pModsScrollbar->SetDisabled(false);

			float scrollRatio = m_pModsScrollbar->GetScrollRatio();
			int offsetButtonY = (int)(heightDiff * scrollRatio);

			int modButtonCounter = 0;
			int yCounter = 0;
			while (modButtonCounter < m_vpGraphicsModButtons.size())
			{
				for (int x = 0; x < 3 && modButtonCounter < m_vpGraphicsModButtons.size(); x++)
				{
					int yPos = m_modWindowHeight - buttonHeight - 17 - (buttonAndSpacer * yCounter);
					Point position = m_vpGraphicsModButtons[modButtonCounter]->GetLocation();
					m_vpGraphicsModButtons[modButtonCounter]->SetLocation(position.m_x, yPos + offsetButtonY);

					modButtonCounter++;
				}

				yCounter++;
			}
		}
		else
		{
			m_pModsScrollbar->SetScrollSize(0.0f);
			m_pModsScrollbar->SetDisabled(true);
		}
	}

	// Sound
	if (m_pSoundMode->GetToggled())
	{
		int visibleSize = m_pModsScrollbar->GetScrollArea().m_height;
		int numYLines = (int)ceil((float)m_vpSoundModButtons.size() / 3.0f);
		int neededHeight = numYLines * buttonAndSpacer + 4;
		int heightDiff = neededHeight - visibleSize;
		if (heightDiff > 0)
		{
			m_pModsScrollbar->SetScrollSize((float)visibleSize / (float)neededHeight);
			m_pModsScrollbar->SetDisabled(false);

			float scrollRatio = m_pModsScrollbar->GetScrollRatio();
			int offsetButtonY = (int)(heightDiff * scrollRatio);

			int modButtonCounter = 0;
			int yCounter = 0;
			while (modButtonCounter < m_vpSoundModButtons.size())
			{
				for (int x = 0; x < 3 && modButtonCounter < m_vpSoundModButtons.size(); x++)
				{
					int yPos = m_modWindowHeight - buttonHeight - 17 - (buttonAndSpacer * yCounter);
					Point position = m_vpSoundModButtons[modButtonCounter]->GetLocation();
					m_vpSoundModButtons[modButtonCounter]->SetLocation(position.m_x, yPos + offsetButtonY);

					modButtonCounter++;
				}

				yCounter++;
			}
		}
		else
		{
			m_pModsScrollbar->SetScrollSize(0.0f);
			m_pModsScrollbar->SetDisabled(true);
		}
	}

	// HUD
	if (m_pHUDMode->GetToggled())
	{
		int visibleSize = m_pModsScrollbar->GetScrollArea().m_height;
		int numYLines = (int)ceil((float)m_vpHUDModButtons.size() / 3.0f);
		int neededHeight = numYLines * buttonAndSpacer + 4;
		int heightDiff = neededHeight - visibleSize;
		if (heightDiff > 0)
		{
			m_pModsScrollbar->SetScrollSize((float)visibleSize / (float)neededHeight);
			m_pModsScrollbar->SetDisabled(false);

			float scrollRatio = m_pModsScrollbar->GetScrollRatio();
			int offsetButtonY = (int)(heightDiff * scrollRatio);

			int modButtonCounter = 0;
			int yCounter = 0;
			while (modButtonCounter < m_vpHUDModButtons.size())
			{
				for (int x = 0; x < 3 && modButtonCounter < m_vpHUDModButtons.size(); x++)
				{
					int yPos = m_modWindowHeight - buttonHeight - 17 - (buttonAndSpacer * yCounter);
					Point position = m_vpHUDModButtons[modButtonCounter]->GetLocation();
					m_vpHUDModButtons[modButtonCounter]->SetLocation(position.m_x, yPos + offsetButtonY);

					modButtonCounter++;
				}

				yCounter++;
			}
		}
		else
		{
			m_pModsScrollbar->SetScrollSize(0.0f);
			m_pModsScrollbar->SetDisabled(true);
		}
	}

	// Misc
	if (m_pMiscMode->GetToggled())
	{
		int visibleSize = m_pModsScrollbar->GetScrollArea().m_height;
		int numYLines = (int)ceil((float)m_vpMiscModButtons.size() / 3.0f);
		int neededHeight = numYLines * buttonAndSpacer + 4;
		int heightDiff = neededHeight - visibleSize;
		if (heightDiff > 0)
		{
			m_pModsScrollbar->SetScrollSize((float)visibleSize / (float)neededHeight);
			m_pModsScrollbar->SetDisabled(false);

			float scrollRatio = m_pModsScrollbar->GetScrollRatio();
			int offsetButtonY = (int)(heightDiff * scrollRatio);

			int modButtonCounter = 0;
			int yCounter = 0;
			while (modButtonCounter < m_vpMiscModButtons.size())
			{
				for (int x = 0; x < 3 && modButtonCounter < m_vpMiscModButtons.size(); x++)
				{
					int yPos = m_modWindowHeight - buttonHeight - 17 - (buttonAndSpacer * yCounter);
					Point position = m_vpMiscModButtons[modButtonCounter]->GetLocation();
					m_vpMiscModButtons[modButtonCounter]->SetLocation(position.m_x, yPos + offsetButtonY);

					modButtonCounter++;
				}

				yCounter++;
			}
		}
		else
		{
			m_pModsScrollbar->SetScrollSize(0.0f);
			m_pModsScrollbar->SetDisabled(true);
		}
	}
}

// Render
void ModMenu::Render()
{
	FrontendPage::Render();
}

void ModMenu::Render2D()
{
	FrontendPage::Render2D();
}

// Static functionality
void ModMenu::_CloseExitPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->CloseExitPressed();
}

void ModMenu::CloseExitPressed()
{
	if(m_returnToMainMenu)
	{
		m_pFrontendManager->SetFrontendScreen(FrontendScreen_MainMenu);
	}
	else
	{
		m_pFrontendManager->SetFrontendScreen(FrontendScreen_PauseMenu);
	}
}

void ModMenu::_GameplayTabPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->GameplayTabPressed();
}

void ModMenu::GameplayTabPressed()
{
	if (m_loaded)
	{
		m_pGUI->RemoveWindow(m_pModWindow);
	}

	ClearModButtonData();

	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	CreateGameplayModButtons();

	SelectLoadedMods();

	Update(0.0f);
	m_pModsScrollbar->SetScrollPosition(1.0f);

	m_pGUI->AddWindow(m_pModWindow);
	m_pModWindow->Show();
}

void ModMenu::_GraphicsTabPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->GraphicsTabPressed();
}

void ModMenu::GraphicsTabPressed()
{
	if (m_loaded)
	{
		m_pGUI->RemoveWindow(m_pModWindow);
	}

	ClearModButtonData();

	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	CreateGraphicsModButtons();

	SelectLoadedMods();

	Update(0.0f);
	m_pModsScrollbar->SetScrollPosition(1.0f);

	m_pGUI->AddWindow(m_pModWindow);
	m_pModWindow->Show();
}

void ModMenu::_SoundTabPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->SoundTabPressed();
}

void ModMenu::SoundTabPressed()
{
	if (m_loaded)
	{
		m_pGUI->RemoveWindow(m_pModWindow);
	}

	ClearModButtonData();

	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	CreateSoundModButtons();

	SelectLoadedMods();

	Update(0.0f);
	m_pModsScrollbar->SetScrollPosition(1.0f);

	m_pGUI->AddWindow(m_pModWindow);
	m_pModWindow->Show();
}

void ModMenu::_GUITabPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->GUITabPressed();
}

void ModMenu::GUITabPressed()
{
	if (m_loaded)
	{
		m_pGUI->RemoveWindow(m_pModWindow);
	}

	ClearModButtonData();

	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	CreateHUDModButtons();

	SelectLoadedMods();

	Update(0.0f);
	m_pModsScrollbar->SetScrollPosition(1.0f);

	m_pGUI->AddWindow(m_pModWindow);
	m_pModWindow->Show();
}

void ModMenu::_MiscTabPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->MiscTabPressed();
}

void ModMenu::MiscTabPressed()
{
	if (m_loaded)
	{
		m_pGUI->RemoveWindow(m_pModWindow);
	}

	ClearModButtonData();

	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	CreateMiscModButtons();

	SelectLoadedMods();

	Update(0.0f);
	m_pModsScrollbar->SetScrollPosition(1.0f);

	m_pGUI->AddWindow(m_pModWindow);
	m_pModWindow->Show();
}

void ModMenu::_ModButtonPressed(void *apData)
{
	ModButtonData* lpModButtonData = (ModButtonData*)apData;
	lpModButtonData->m_pModMenu->ModButtonPressed(lpModButtonData);
}

void ModMenu::ModButtonPressed(ModButtonData* pModButtonData)
{
	int buttonWidth = m_modButtonWidth;

	// Decide on the state of the toggle flag for each existing mod button and also the one we pressed
	bool addTick = false;
	for (unsigned int i = 0; i < m_vpModButtonData.size(); i++)
	{
		if (pModButtonData == m_vpModButtonData[i])
		{
			if (m_vpModButtonData[i]->m_toggled)
			{
				if (m_vpModButtonData[i]->m_allowToggleOff)
				{
					m_vpModButtonData[i]->m_toggled = false;
					string themeName = VoxGame::GetInstance()->GetModsManager()->GetHUDTextureTheme();
					string tickIcon = "media/textures/gui/" + themeName + "/common/tick.tga";
					m_vpModButtonData[i]->m_pModButton->RemoveIcon(tickIcon.c_str());
					VoxGame::GetInstance()->GetModsManager()->RemoveMod(m_vpModButtonData[i]->m_modName, m_vpModButtonData[i]->m_gameplayButton, m_vpModButtonData[i]->m_graphicsButton, m_vpModButtonData[i]->m_soundButton, m_vpModButtonData[i]->m_HUDButton, m_vpModButtonData[i]->m_miscButton);
				}
			}
			else
			{
				addTick = true;
				pModButtonData->m_toggled = true;
				VoxGame::GetInstance()->GetModsManager()->AddMod(m_vpModButtonData[i]->m_modName, m_vpModButtonData[i]->m_gameplayButton, m_vpModButtonData[i]->m_graphicsButton, m_vpModButtonData[i]->m_soundButton, m_vpModButtonData[i]->m_HUDButton, m_vpModButtonData[i]->m_miscButton);
			}
		}
		else
		{
			if (pModButtonData->m_allowMultipleSelection == false || m_vpModButtonData[i]->m_allowMultipleSelection == false)
			{
				m_vpModButtonData[i]->m_toggled = false;
				string themeName = VoxGame::GetInstance()->GetModsManager()->GetHUDTextureTheme();
				string tickIcon = "media/textures/gui/" + themeName + "/common/tick.tga";
				m_vpModButtonData[i]->m_pModButton->RemoveIcon(tickIcon.c_str());
				VoxGame::GetInstance()->GetModsManager()->RemoveMod(m_vpModButtonData[i]->m_modName, m_vpModButtonData[i]->m_gameplayButton, m_vpModButtonData[i]->m_graphicsButton, m_vpModButtonData[i]->m_soundButton, m_vpModButtonData[i]->m_HUDButton, m_vpModButtonData[i]->m_miscButton);
			}
		}
	}

	// Gameplay

	// Graphics

	// Sound

	// HUD
	if (pModButtonData->m_HUDButton)
	{
		m_pFrontendManager->LoadCommonGraphics(pModButtonData->m_modName);
		VoxGame::GetInstance()->SkinGUI();
	}

	// Misc

	// If we need to add the tick
	if (addTick)
	{
		string themeName = VoxGame::GetInstance()->GetModsManager()->GetHUDTextureTheme();
		string tickIcon = "media/textures/gui/" + themeName + "/common/tick.tga";
		pModButtonData->m_pModButton->AddIcon(m_pRenderer, tickIcon.c_str(), 32, 32, 32, 32, buttonWidth - 38, 4, 2.0f);
	}
}