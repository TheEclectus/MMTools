#pragma once

#include <stack>

#include "Array.hpp"
#include "Typedefs.h"

namespace MMT
{
	class TileSurface : public Array2d<Tile>
	{
		friend class GameState;
	protected:
		//std::stack<MRect> _clippingAreaStack;
		//std::stack<Point> TransformStack;	// The *STORED* transform values.
											
		//Point CurrentTransform = { 0, 0 };	// THIS IS JUST THE CURRENT ONE.

		//void _pushClip(const MRect &Area);
		//void _popClip();
	public:
		//MRect *ClippingArea;
		TileColor DefaultColors;

		TileSurface();
		TileSurface(const Dimensions &Size);

		//void SetViewport()

		//void PushTransform();	// Call before modifying _currentTransform
		//void PopTransform();

		void SetDefaultColors(const Color &FG, const Color &BG);
		void SetDefaultColors(const TileColor &Color);

		void SetTileColor(const Point &Position, const TileColor &Color);
		void SetTileColor(const Rect &Position, const TileColor &Color);

		void SetTileCharacter(const Point &Position, const u16 Character);

		void DrawTile(const Point &Position, const u16 Character);
		void DrawTile(const Point &Position, const u16 Character, const Color &FG, const Color &BG);
		void DrawTile(const Point &Position, const u16 Character, const TileColor &Color);

		void DrawLine(const Point &Point1, const Point &Point2, const TileColor &Color1, const TileColor &Color2, const u16 Char);
		void DrawLine(const Point &Point1, const Point &Point2, const TileColor &Color, const u16 Char);

		void DrawText(const Point &Point, const TileColor &Color, const char* String);
		void DrawText(const Point &Point, const Dimensions &Bounds, const TileColor &Color, const char* String);

		void DrawRect(const Rect &Rect, const TileColor &Color, const u16 Character, const bool &Fill);
		void DrawRectEx(const Rect &Rect, const TileColor &Color, const u16 HorizontalChar, const u16 VerticalChar, const u16 TopLeftChar, const u16 TopRightChar, const u16 BotLeftChar, const u16 BotRightChar, const bool Fill);

		void ClearSurface(const Color &ClearColor);

		// Similar to CopyToPosition, but only copies non-null tiles. 
		void Blit(const Array2dRef<Tile> &From, const Point &Position) const;

		// Array2d<T>
		//virtual void CopyToPosition(Array2d<MTile> &From, const MPoint &Location) override;
	};
}