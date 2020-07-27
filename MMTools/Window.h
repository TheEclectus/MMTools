#pragma once

#include <format.h>
#include <SDL.h>
#include <vector>

//#include "Event.h"
//#include "Font.h"
#include "TileSurface.h"
#include "Typedefs.h"

namespace MMT
{	
	class Font;

	// TODO: (?) Potentially make Surface a SpriteView?
	// -TODO:- (X) Make the user provide an ArrayXd<MTile>-castable variable to use? || Make GameState have a TileSurface/SpriteViewer by default, which is passed to the Window after being rendered (Surface may not even be needed anymore)?
	///<summary>Stores the information of a single window.</summary>
	class Window
	{
		friend class Engine;
	private:
		SDL_Window *SDLWindow;
		SDL_Renderer *SDLRenderer;
		Dimensions SizeInTiles;
		Font *Tileset = nullptr;
	public:
		//Scene *Surface;
		TileSurface *Surface;
		//EventSubject EventQueue;

		Window(const std::string &Title, Dimensions SizeInTiles, const Dimensions &FontGlyphSize, const std::string &FontPath);
		~Window();
		//bool Initialize(const char *Title, Font *Tileset, Dimensions SizeInTiles);
		//void Cleanup();

		// Gets the size in tiles
		const Dimensions GetSize() const;
		// Sets the size in tiles
		void SetSize(const Dimensions &Size);

		void Render();
	};
}