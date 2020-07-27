#include "GameState.h"

namespace MMT
{
	bool GameState::Load()
	{
		return true;
	}

	void GameState::Init()
	{
		
	}

	void GameState::Cleanup()
	{

	}

	void GameState::Input(const KeyboardData &Keyboard, const MouseData &Mouse)
	{
		MMT::IWidget *NextMouseOverWidget = PickTop(Mouse.TilePosition);

		// Different widget than before
		if (NextMouseOverWidget != MouseOverWidget)
		{
			// Old widget is having the mouse leave it
			if (MouseOverWidget)
			{
				MouseOverWidget->bMouseOver = false;
				memset(MouseOverWidget->bMouseDownOver, 0, sizeof(bool) * 3);
				MouseOverWidget->OnMouseLeave(Mouse);
			}

			// New widget is being moused over
			if (NextMouseOverWidget)
			{
				NextMouseOverWidget->bMouseOver = true;
				NextMouseOverWidget->OnMouseOver(Mouse);
			}
		}

		if (MouseOverWidget != nullptr && Mouse.TileDelta != MMT::Point(0, 0))
		{
			MouseOverWidget->OnMouseMove(Mouse);
		}

		for (u8 i = 0u; i < 3u; i++)
		{
			if (NextMouseOverWidget && NextMouseOverWidget->BoundingBox.IsPointInside(Mouse.Buttons[i].DownTilePosition) && NextMouseOverWidget->bMouseOver)
			{
				bool bCurMouseDownOverOld = NextMouseOverWidget->bMouseDownOver[i];
				NextMouseOverWidget->bMouseDownOver[i] = true;
				if (!bCurMouseDownOverOld)
				{
					NextMouseOverWidget->OnMouseDown(Mouse, i);
					if (NextMouseOverWidget->bCanBeFocused && !NextMouseOverWidget->bIsFocused)
					{
						Focus(NextMouseOverWidget);
					}
				}
			}
			else if (NextMouseOverWidget && NextMouseOverWidget->bMouseDownOver[i])
			{
				NextMouseOverWidget->bMouseDownOver[i] = false;
				NextMouseOverWidget->OnMouseUp(Mouse, i);
			}
		}

		MouseOverWidget = NextMouseOverWidget;

		// Key events fire for focused widget only, so...
		IWidget *EventWidget = (FocusedWidget != nullptr) ? FocusedWidget : CoreWidget;
		if (EventWidget)
		{
			if (EventWidget->bRepeatKeyboardInput)
			{
				for (key_t i : Keyboard.KeyPresses)
				{
					EventWidget->OnKeyPress(Keyboard, i);
				}
				for (key_t i : Keyboard.KeyReleases)
				{
					EventWidget->OnKeyRelease(Keyboard, i);
				}
			}
			else
			{
				for (u16 i = 0u; i < 512; i++)
				{
					key_t CurKeyPrev = Keyboard.PreviousState[i];
					key_t CurKey = Keyboard.CurrentState[i];

					// Up -> Down = KeyPress
					if (!CurKeyPrev && CurKey)
					{
						EventWidget->OnKeyPress(Keyboard, i);
					}
					// Down -> Up = KeyRelease
					else if (CurKeyPrev && !CurKey)
					{
						EventWidget->OnKeyRelease(Keyboard, i);
					}
				}
			}
		}
	}

	void GameState::Logic()
	{

	}

	void GameState::Render(TileSurface &DrawScene)
	{
		DrawScene.ClearSurface(MMT::Colors::Black);
		for (auto i : Widgets)
		{
			i->Draw(DrawScene);
		}
	}

	GameState::GameState(MMT::Engine &Parent, const std::string &Name) :
		Parent(Parent),
		Name(Name)
	{

	}

	GameState::~GameState()
	{
		
	}
	
	void GameState::Focus(MMT::IWidget *Widget)
	{
		if (Widget == CoreWidget)
		{
			Unfocus();
		}
		else if (Widget != FocusedWidget && std::find(Widgets.begin(), Widgets.end(), Widget) != Widgets.end())
		{
			if (FocusedWidget) Unfocus();
			FocusedWidget = Widget;
			FocusedWidget->bIsFocused = true;
			FocusedWidget->OnFocus();
		}
	}

	void GameState::Unfocus()
	{
		if (FocusedWidget)
		{
			FocusedWidget->bIsFocused = false;
			FocusedWidget->OnUnfocus();
			FocusedWidget = nullptr;
		}
	}

	void GameState::AddWidget(MMT::IWidget *Widget)
	{
		if (!Widget->ParentState && std::find(Widgets.begin(), Widgets.end(), Widget) == Widgets.end())
		{
			Widget->ParentState = this;
			Widgets.push_back(Widget);
		}
	}

	void GameState::SetCoreWidget(MMT::IWidget *Widget)
	{
		if (Widget == nullptr || std::find(Widgets.begin(), Widgets.end(), Widget) != Widgets.end())
		{
			CoreWidget = Widget;
		}
	}

	void GameState::RemoveWidget(MMT::IWidget *Widget)
	{
		auto PosIter = std::find(Widgets.begin(), Widgets.end(), Widget);
		if (PosIter != Widgets.end())
		{
			Widget->ParentState = nullptr;
			Widgets.erase(PosIter);
		}
	}

	MMT::IWidget *GameState::PickTop(const MMT::Point &Position)
	{
		return Widgets.PickTop(Position);
	}

	MMT::IWidget *GameState::GetFocusedWidget() const
	{
		return FocusedWidget;
	}
}