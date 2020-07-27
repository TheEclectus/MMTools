// <summary>, <param>

#pragma once

#include <stack>
#include <string>
#include <unordered_map>

#include "Font.h"
#include "GameState.h"
#include "Typedefs.h"
#include "Window.h"

namespace MMT
{
	/// <summary>The main engine.</summary>
	class Engine
	{
	private:
		std::unordered_map<u32, MMT::GameState*>	RegisteredStates;
		std::stack<u32>	StateStack;
		bool			bRunning = false;
		bool			bStopRequested = false;
		KeyboardData	Keyboard;
		MouseData		Mouse;

		/// <summary>Calls Cleanup() on every GameState registered to the Engine.
		/// Functionally the opposite of LoadStates().</summary>
		void CleanupStates();	// When the engine is stopped (opposite of LoadStates)
		
		/// <summary>Calls Load() on every GameState registered to the Engine.</summary>
		bool LoadStates();	// When the engine is run
		
		/// <summary>Populates Keyboard and Mouse with up-to-date information, and
		/// calls events where appropriate.</summary>
		void PollInput();
	public:
		MMT::Window		MainWindow;

		/// <param name="WindowTitle">Title of the engine's main window.</param>
		/// <param name="SizeInTiles">Dimensions of the display window in tiles.</param>
		Engine(const std::string &WindowTitle, const Dimensions &SizeInTiles);
		~Engine();

		/// <summary>Registers a GameState to be used by the engine. States 
		/// can only be registered while the engine isn't running. Returns
		/// true on success.</summary>
		/// <param name="StateID">Unique identifier that the new GameState will be mapped to.</param>
		/// <param name="State">Pointer to the GameState to be added.</param>
		bool RegisterState(u32 StateID, MMT::GameState *State);
		
		/// <summary>Pushes a GameState to the top of the stack, using the 
		/// unique identifier specified in RegisterState().</summary>
		/// <param name="StateID">Unique identifier of the GameState to push.</param>
		void PushState(u32 StateID);

		/// <summary>Pops the topmost GameState from the stack.</summary>
		void PopState();

		/// <summary>Starts the engine's main loop.</summary>
		void Run();

		/// <summary>Requests that the engine stops after completing the current loop.</summary>
		void Stop();
	};
}