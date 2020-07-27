#include "Window.h"

#include "Engine.h"
//#include "Error.h"
#include "Widget.h"

#include <string>

#undef DrawText

namespace MMT
{
	Window::Window(const std::string &Title, Dimensions SizeInTiles, const Dimensions &FontGlyphSize, const std::string &FontPath) :
		SDLWindow(nullptr),
		SDLRenderer(nullptr),
		SizeInTiles(SizeInTiles)
	{
		Tileset = new Font(FontGlyphSize, FontPath);

		SDLWindow = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Tileset->GetGlyphSize().W * SizeInTiles.W, Tileset->GetGlyphSize().H * SizeInTiles.W, SDL_WINDOW_SHOWN);
		if (SDLWindow)
		{
			SDLRenderer = SDL_CreateRenderer(SDLWindow, -1, SDL_RENDERER_ACCELERATED);
			if (SDLRenderer)
			{
				Tileset->Cache(SDLRenderer);
				Surface = new TileSurface(SizeInTiles);
			}
			else
			{
				SDL_DestroyWindow(SDLWindow);
			}
		}
	}

	Window::~Window()
	{
		delete Surface;
		if (SDLWindow) SDL_DestroyWindow(SDLWindow);
		if (SDLRenderer) SDL_DestroyRenderer(SDLRenderer);
	}

	const Dimensions Window::GetSize() const
	{
		return SizeInTiles;
	}

	void Window::SetSize(const Dimensions &Size)
	{
		SDL_SetWindowSize(SDLWindow, Size.W * Tileset->GetGlyphSize().W, Size.H * Tileset->GetGlyphSize().H);
		
		TileSurface *Temp = new TileSurface(Size);
		Temp->CopyToPosition(*Surface, { 0, 0 });
		delete Surface;
		Surface = Temp;
	}

	void Window::Render()
	{
		//ErrorContext EC("While rendering MMT::Window.");

		Font &CurrentFont = *Tileset;
		Dimensions CurFontSize = CurrentFont.GetGlyphSize();
		SDL_Texture *FontTexture = (SDL_Texture*)CurrentFont.GetFontTexture();

		for (u32 y = 0u; y < Surface->GetHeight(); y++)
		{
			for (u32 x = 0u; x < Surface->GetWidth(); x++)
			{
				Tile &CurTile = (*Surface)[y][x]; //Surface[y][x];
				SDL_Rect SourcePos = CurrentFont.GetGlyphPositionByIndex(CurTile.Character);
				SDL_Rect DestPos = { static_cast<s32>(x * CurFontSize.W), static_cast<s32>(y * CurFontSize.H), static_cast<s32>(CurFontSize.W), static_cast<s32>(CurFontSize.H) };

				if (DestPos.x >= 0 && DestPos.x < SizeInTiles.W * CurFontSize.W && DestPos.y >= 0 && DestPos.y < SizeInTiles.H * CurFontSize.W)
				{
					SDL_SetTextureColorMod(FontTexture, CurTile.Color.FG.R, CurTile.Color.FG.G, CurTile.Color.FG.B);
					SDL_SetRenderDrawColor(SDLRenderer, CurTile.Color.BG.R, CurTile.Color.BG.G, CurTile.Color.BG.B, 255);
					SDL_RenderFillRect(SDLRenderer, &DestPos);
					SDL_RenderCopy(SDLRenderer, FontTexture, &SourcePos, &DestPos);
				}
			}
		}
	}
}