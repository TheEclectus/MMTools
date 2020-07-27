#pragma once

#include <cstdint>
#include <vector>

#include <SDL.h>

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;
typedef uintptr_t	uptr;
typedef int8_t		s8;
typedef int16_t		s16;
typedef int32_t		s32;
typedef int64_t		s64;

typedef u8			key_t;

namespace MMT
{
	struct Dimensions
	{
		s32 W, H;
	};

	struct Point
	{
		s32 X, Y;

		Point(const s32 X = 0, const s32 Y = 0) :
			X(X),
			Y(Y)
		{

		}

		inline bool operator==(const Point &RHS) const
		{
			return ((X == RHS.X) && (Y == RHS.Y));
		}

		inline bool operator!=(const Point &RHS) const
		{
			return !operator==(RHS);
		}
	};

	struct Rect
	{
		s32 X, Y, W, H;

		inline Point Position() const
		{
			return { X, Y };
		}

		inline Dimensions Size() const
		{
			return { W, H };
		}

		inline operator SDL_Rect() const
		{
			return { X, Y, W, H };
		}

		inline bool IsPointInside(const Point &Position) const
		{
			return ((Position.X >= X) && (Position.X <= (X + W) - 1) && (Position.Y >= Y) && (Position.Y <= (Y + H) - 1));
		}
	};

	struct MouseData
	{
		struct Button
		{
			u8 bIsDown, bIsDragging;
			Point DownPosition = { -1, -1 }, DownTilePosition = { -1, -1 };
		} Buttons[3], PrevButtons[3];

		Point Position = { -1, -1 };	// -1,-1 means the position hasn't been retrieved yet.
		Point Delta;

		Point TilePosition = { -1, -1 };
		Point TileDelta;
	};

	struct KeyboardData
	{
		/*u8*/ key_t CurrentState[SDL_Scancode::SDL_NUM_SCANCODES];
		/*u8*/ key_t PreviousState[SDL_Scancode::SDL_NUM_SCANCODES];
		bool bLeftShiftDown, bRightShiftDown, bShiftDown;
		bool bLeftCtrlDown, bRightCtrlDown, bCtrlDown;
		std::vector<key_t> KeyPresses, KeyReleases;
	};

	struct Color
	{
		u8 R, G, B;
		inline Color Invert() const
		{
			return{ 255u - R, 255u - G, 255u - B };
		}
		inline bool operator==(const Color &RHS) const
		{
			return (R == RHS.R && G == RHS.G && B == RHS.B);
		}
	};

	struct TileColor
	{
		Color FG, BG;
		inline TileColor Invert() const
		{
			return{ FG.Invert(), BG.Invert() };
		}
		inline TileColor Flip() const
		{
			return{ BG, FG };
		}
		inline bool operator==(const TileColor &RHS) const
		{
			return (FG == RHS.FG && BG == RHS.BG);
		}
	};

	struct Tile
	{
		u16 Character;
		TileColor Color;
	};

	inline Color Lerp(const Color &Color1, const Color &Color2, float Coef)
	{
		u8 r, g, b;
		r = Color1.R + static_cast<u8>(static_cast<float>(Color2.R - Color1.R) * Coef);
		g = Color1.G + static_cast<u8>(static_cast<float>(Color2.G - Color1.G) * Coef);
		b = Color1.B + static_cast<u8>(static_cast<float>(Color2.B - Color1.B) * Coef);

		return{ r, g, b };
	}

	inline TileColor Lerp(const TileColor &Color1, const TileColor &Color2, float Coef)
	{
		return{
			Lerp(Color1.FG, Color2.FG, Coef),
			Lerp(Color1.BG, Color2.BG, Coef)
		};
	}

	namespace Colors
	{
		// Technical
		const Color Transparent = { 0, 0, 0 };

		// Greyscale
		const Color Black = { 0, 0, 0 };
		const Color DarkGrey = { 63, 63, 63 };
		const Color Grey = { 127, 127, 127 };
		const Color LightGrey = { 190, 190, 190 };
		const Color White = { 255, 255, 255 };

		const Color Yellow = { 255, 255, 0 };
		const Color Green = { 0, 255, 0 };

		//constexpr MTileColor Double(const MColor &Color);
	};
}