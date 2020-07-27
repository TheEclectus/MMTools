#include "Font.h"

#include "Error.h"
#include "Window.h"

namespace MMT
{
	Font::Font(const Dimensions &GlyphSize, const std::string &Path) :
		GlyphSize(GlyphSize),
		FontMapSize({ 0, 0 }),
		FontMapTexture(nullptr)
	{
		FontMapSurface = IMG_Load(Path.c_str());

		// TODO: Perhaps embed the default font, so a crash isn't needed?
		xassert(FontMapSurface != nullptr, "Failed to load file '%s'.", Path);

		s32 ColorKeyResult = SDL_SetColorKey(FontMapSurface, 1, SDL_MapRGB(FontMapSurface->format, 0, 0, 0));
		xassert(ColorKeyResult == 0, "Failed to apply color key to font surface ('%s').", Path);

		FontMapSize = { FontMapSurface->w, FontMapSurface->h };
	}

	void Font::Cache(SDL_Renderer *Renderer)
	{
		if (FontMapSurface)
		{
			FontMapTexture = SDL_CreateTextureFromSurface(Renderer, FontMapSurface);
			SDL_FreeSurface(FontMapSurface);
			FontMapSurface = nullptr;
		}
	}

	Font::~Font()
	{
		if (FontMapSurface) SDL_FreeSurface(FontMapSurface);
		if (FontMapTexture) SDL_DestroyTexture(FontMapTexture);
	}

	const Dimensions Font::GetFontMapSize() const
	{
		return FontMapSize;
	}

	const Dimensions Font::GetGlyphSize() const
	{
		return GlyphSize;
	}

	const SDL_Texture *Font::GetFontTexture() const
	{
		return FontMapTexture;
	}

	Rect Font::GetGlyphPositionByIndex(const u16 Index)
	{
		u16 Row = (Index * GlyphSize.W) / FontMapSize.W;
		u16 Column = (Index * GlyphSize.W) % FontMapSize.W;
		return {static_cast<s32>(Column), static_cast<s32>(Row * GlyphSize.H), static_cast<s32>(GlyphSize.W), static_cast<s32>(GlyphSize.H) };
	}
}