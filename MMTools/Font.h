#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Typedefs.h"
#include "Window.h"

namespace MMT
{
	// TODO: Make LoadFromFile() deduce tile size from image size instead of vice versa
	class Font
	{
		friend class MMT::Window;
	private:
		Dimensions GlyphSize;
		Dimensions FontMapSize;
		SDL_Surface *FontMapSurface;
		SDL_Texture *FontMapTexture;

		/// <param name="GlyphSize">Size (in pixels) of an individual glyph.</param>
		/// <param name="Path">Path to the bitmap font image.</param>
		Font(const Dimensions &GlyphSize, const std::string &Path);

		/// <summary>Loads the bitmap font to FontMapTexture and frees FontMapSurface.</summary>
		/// <param name="Renderer">SDL_Renderer associated with the Window that the Font will be used in.</param>
		void Cache(SDL_Renderer *Renderer);
	public:
		~Font();

		/// <summary>Returns the size (in pixels) of the font map texture.</summary>
		const Dimensions GetFontMapSize() const;

		/// <summary>Returns the size (in pixels) of a single glyph of the font map.</summary>
		const Dimensions GetGlyphSize() const;

		/// <summary>Returns the SDL_Texture containing the font map.</summary>
		const SDL_Texture *GetFontTexture() const;

		// Gets the pixel position of a glyph by it's index.
		/// <summary>Returns the position of a glyph specified by its index (in most
		/// cases, said index is also the decimal equivalent of a character).</summary>
		/// <param name="Index">Index of the glyph whose position will be retrieved.</param>
		Rect GetGlyphPositionByIndex(const u16 Index);

		/*inline bool IsLoaded() const
		{
			return (FontMapSurface || FontMapTexture);
		}

		inline bool IsCached() const
		{
			return (FontMapTexture == nullptr);
		}*/
	};
}