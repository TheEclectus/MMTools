#include "Engine.h"

namespace MMT
{
	void Engine::CleanupStates()
	{
		if (!bRunning)
		{
			for (auto i : RegisteredStates)
			{
				i.second->Cleanup();
			}
		}
	}

	bool Engine::LoadStates()
	{
		for (auto i : RegisteredStates)
		{
			if (!i.second->Load()) return false;
		}
		return true;
	}

	void Engine::PollInput()
	{
		SDL_Event Event;

		//std::vector<u16> KeyPresses, KeyReleases;
		Keyboard.KeyPresses.clear();
		Keyboard.KeyReleases.clear();
		while (SDL_PollEvent(&Event))
		{
			// TODO: Maybe pass off the window close event to an event handler?
			switch (Event.type)
			{
			case SDL_QUIT:
				Stop();
				break;
			case SDL_KEYDOWN:
				Keyboard.KeyPresses.push_back(Event.key.keysym.scancode);
				break;
			case SDL_KEYUP:
				Keyboard.KeyReleases.push_back(Event.key.keysym.scancode);
				break;
			}

			// TODO: Keyboard events
		}

		// Get the most recent mouse position.
		s32 NewMousePosX = 0, NewMousePosY = 0;
		u32 Buttons = SDL_GetMouseState(&NewMousePosX, &NewMousePosY);
		
		/* 
			-1,-1 means the position hasn't been retrieved yet (startup),
			so delta is 0.
		*/
		if (Mouse.Position.X == -1 || Mouse.Position.Y == -1)
		{
			Mouse.Delta = { 0, 0 };
		}
		else
		{
			s32 DeltaX = NewMousePosX - Mouse.Position.X;
			s32 DeltaY = NewMousePosY - Mouse.Position.Y;
			Mouse.Delta = { DeltaX, DeltaY };
		}
		Mouse.Position = { NewMousePosX, NewMousePosY };

		MMT::Dimensions TileSize = this->MainWindow.Tileset->GetGlyphSize();
		
		MMT::Point NewTilePos = { Mouse.Position.X / TileSize.W, Mouse.Position.Y / TileSize.H };
		//Mouse.TilePosition = { Mouse.Position.X / TileSize.W, Mouse.Position.Y / TileSize.H };

		if (NewTilePos.X == -1 || NewTilePos.Y == -1)
		{
			Mouse.TileDelta = { 0, 0 };
		}
		else
		{
			s32 TileDeltaX = NewTilePos.X - Mouse.TilePosition.X;
			s32 TileDeltaY = NewTilePos.Y - Mouse.TilePosition.Y;
			Mouse.TileDelta = { TileDeltaX, TileDeltaY };
			if (Mouse.TileDelta != MMT::Point(0, 0))
			{
				printf("S");
			}
		}

		Mouse.TilePosition = NewTilePos;

		// Tile position and delta are both based on the window-space position.
		
		
		Mouse.TileDelta = { Mouse.Delta.X / TileSize.W, Mouse.Delta.Y / TileSize.H };

		memcpy_s(Mouse.PrevButtons, sizeof(MouseData::Button) * 3, Mouse.Buttons, sizeof(MouseData::Button) * 3);

		// Buttons and dragging
		u8 NewButtons[3] = { 0, 0, 0 };
		NewButtons[0] = Buttons & SDL_BUTTON_LMASK;
		NewButtons[1] = Buttons & SDL_BUTTON_MMASK;
		NewButtons[2] = Buttons & SDL_BUTTON_RMASK;

		for (u8 i = 0; i < 3; i++)
		{
			MouseData::Button &CurBtn = Mouse.Buttons[i];
			// Up -> Down
			if (!CurBtn.bIsDown && NewButtons[i])
			{
				CurBtn.DownPosition = Mouse.Position;
				CurBtn.DownTilePosition = Mouse.TilePosition;
			}

			// Down -> Up
			if (CurBtn.bIsDown && !NewButtons[i])
			{
				CurBtn.DownPosition = { -1, -1 };
				CurBtn.DownTilePosition = { -1, -1 };
			}

			CurBtn.bIsDown = NewButtons[i];

			// Only the initial condition need be satisfied; dragging won't end until button is release
			if (CurBtn.bIsDown && Mouse.Buttons[i].DownTilePosition != Mouse.TilePosition)
			{
				CurBtn.bIsDragging = true;
			}
			else if (!CurBtn.bIsDown)
			{
				CurBtn.bIsDragging = false;
			}
		}
		
		memcpy_s(Keyboard.PreviousState, sizeof(key_t) * SDL_Scancode::SDL_NUM_SCANCODES, Keyboard.CurrentState, sizeof(key_t) * SDL_Scancode::SDL_NUM_SCANCODES);
		memcpy_s(Keyboard.CurrentState, sizeof(key_t) * SDL_Scancode::SDL_NUM_SCANCODES, SDL_GetKeyboardState(nullptr), sizeof(key_t) * SDL_Scancode::SDL_NUM_SCANCODES);
		
		SDL_Keymod Mod = SDL_GetModState();
		Keyboard.bLeftShiftDown = Mod & SDL_Keymod::KMOD_LSHIFT;
		Keyboard.bRightShiftDown = Mod & SDL_Keymod::KMOD_RSHIFT;
		Keyboard.bShiftDown = Keyboard.bLeftShiftDown || Keyboard.bRightShiftDown;

		Keyboard.bLeftCtrlDown = Mod & SDL_Keymod::KMOD_LCTRL;
		Keyboard.bRightCtrlDown = Mod & SDL_Keymod::KMOD_RCTRL;
		Keyboard.bCtrlDown = Keyboard.bLeftCtrlDown || Keyboard.bRightCtrlDown;

		SDL_SetWindowTitle(MainWindow.SDLWindow, 
			fmt::sprintf("%d, %d | D: %d, %d | L[%s|%s] M[%s|%s] R[%s|%s]", 
				Mouse.TilePosition.X, Mouse.TilePosition.Y, 
				Mouse.TileDelta.X, Mouse.TileDelta.Y,
				Mouse.Buttons[0].bIsDown ? "Dn" : "  ", Mouse.Buttons[0].bIsDragging ? "Dr" : "  ",
				Mouse.Buttons[1].bIsDown ? "Dn" : "  ", Mouse.Buttons[1].bIsDragging ? "Dr" : "  ",
				Mouse.Buttons[2].bIsDown ? "Dn" : "  ", Mouse.Buttons[2].bIsDragging ? "Dr" : "  ").c_str());
	}

	Engine::Engine(const std::string &WindowTitle, const Dimensions &SizeInTiles) :
		MainWindow(WindowTitle, SizeInTiles, { 12, 12 }, "DefaultFont.png")
	{
		//SDL_Init(SDL_INIT_VIDEO);
	}

	Engine::~Engine()
	{
		Stop();	// Won't do anything if it's not running anyways. Clean up everything if it is.
		for (auto i : RegisteredStates)
		{
			delete i.second;
		}
	}

	bool Engine::RegisterState(u32 StateID, MMT::GameState *State)
	{
		if (!bRunning && (RegisteredStates.find(StateID) == RegisteredStates.end()))
		{
			RegisteredStates[StateID] = State;
			return true;
		}
		return false;
	}

	void Engine::PushState(u32 StateID)
	{
		if (RegisteredStates.find(StateID) != RegisteredStates.end())
		{
			StateStack.push(StateID);
			RegisteredStates[StateID]->Init();
		}
	}

	void Engine::PopState()
	{
		if (StateStack.size() > 0)
		{
			StateStack.pop();
		}
	}

	void Engine::Run()
	{
		bool bStatesLoaded = LoadStates();
		xassert(bStatesLoaded, "Failed to load GameStates.");
		
		bRunning = true;
		while (!bStopRequested)
		{
			SDL_RenderClear(MainWindow.SDLRenderer);
			PollInput();

			if (StateStack.size() > 0)
			{
				// ID validity is checked when pushed to the stack.
				MMT::GameState *Top = RegisteredStates[StateStack.top()];
				Top->Input(Keyboard, Mouse);
				Top->Logic();
				Top->Render(*MainWindow.Surface);
				//MainWindow.Surface->PrintPreview();
				//Top->Input

				MainWindow.Render();
			}

			SDL_RenderPresent(MainWindow.SDLRenderer);
		}
		bRunning = false;
		CleanupStates();

		// Reset bStopRequested
		bStopRequested = false;
	}

	void Engine::Stop()
	{
		if (bRunning)
		{
			bStopRequested = true;
		}
	}
}