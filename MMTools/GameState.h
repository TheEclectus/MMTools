#pragma once

#include <string>

#include "Engine.h"
#include "Widget.h"

namespace MMT
{
	class GameState
	{
		friend class MMT::Engine;
	private:
		MMT::IWidget *MouseOverWidget = nullptr;
	protected:
		MMT::Engine &Parent;
		MMT::WidgetList Widgets;
		// The widget that will receive all events while nothing else is focused.
		// If Focus is called on the CoreWidget, it's the same as calling Unfocus().
		MMT::IWidget *CoreWidget = nullptr;
		MMT::IWidget *FocusedWidget = nullptr;
		const std::string Name;

		/// <summary>Loads the resources associated with the GameState; called
		/// when Run() is called. Returns true on success.</summary>
		virtual bool Load();	// Initial loading. True on success.
		
		/// <summary>Sets GameState to its initial state; called whenever 
		/// the GameState is brought to the top of the stack.</summary>
		virtual void Init();	// Whenever it's brought to the top of the stack
		
		/// <summary>Frees the resources associated with the GameState; functionally 
		/// the opposite of Load(). Called when the engine is stopped. Only works 
		/// if Engine.bRunning == false.</summary>
		virtual void Cleanup();	// When resources are freed (opposite of Load)
		
		/// <summary>Exposes most recent keyboard and mouse input data to the 
		/// GameState. Responsible for caling child IWidget IO events.</summary>
		/// <param name="Keyboard">Keyboard input data.</param>
		/// <param name="Mouse">Mouse input data.</param>
		virtual void Input(const KeyboardData &Keyboard, const MouseData &Mouse);
		
		/// <summary>Runs non-IO related logic pertaining to the GameState.</summary>
		virtual void Logic();

		/// <summary>Renders the GameState to a TileSurface (unless specifically
		/// called otherwise, to the Window's TileSurface).</summary>
		/// <param name="DrawScene">The destination TileSurface to render to.</param>
		virtual void Render(TileSurface &DrawScene);
	public:
		/// <param name="Parent">The Engine that the GameState will run under.</param>
		/// <param name="Name">Referential name of the GameState.</param>
		GameState(MMT::Engine &Parent, const std::string &Name);
		~GameState();
		
		/// <summary>Sets the focused IWidget.</summary>
		/// <param name="Widget">The IWidget to focus.</param>
		void Focus(MMT::IWidget *Widget);

		/// <summary>Unfocuses currently focused IWidget.</summary>
		void Unfocus();

		/// <summary>Adds an IWidget to the GameState and sets it as its parent. Won't add any IWidgets that are already a child.</summary>
		/// <param name="Widget">The IWidget to add.</param>
		void AddWidget(MMT::IWidget *Widget);

		/// <summary>Sets an IWidget that has already been added to the GameState's core widget.</summary>
		/// <param name="Widget">The IWidget to set as the core widget.</param>
		void SetCoreWidget(MMT::IWidget *Widget);

		/// <summary>Removes an IWidget from the GameState and nullifies its parent.</summary>
		/// <param name="Widget">The IWidget to remove.</param>
		void RemoveWidget(MMT::IWidget *Widget);

		/// <summary>Returns the topmost IWidget from the GameState's children, based on 
		/// its screen position; nullptr if there are no valid IWidgets at the specified 
		/// position.</summary>
		/// <param name="Position">Screen position to pick topmost IWidget from.</param>
		MMT::IWidget *PickTop(const MMT::Point &Position);

		/// <summary>Returns the currently-focused IWidget; nullptr if no IWidget
		/// is focused.</summary>
		MMT::IWidget *GetFocusedWidget() const;
	};
}