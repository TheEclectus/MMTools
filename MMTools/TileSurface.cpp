#include "TileSurface.h"

#include "Utility.h"

namespace MMT
{
	//void TileSurface::_pushClip(const MRect &Area)
	//{
	//	// TODO: Determine if a clipping area is really needed, it can be manually set if it's needed (add functions to do that)
	//	_clippingAreaStack.push(Area);
	//	ClippingArea = &_clippingAreaStack.top();
	//}

	//void TileSurface::_popClip()
	//{
	//	_clippingAreaStack.pop();
	//	if (_clippingAreaStack.size() == 0)
	//	{
	//		ClippingArea = nullptr;
	//	}
	//	else
	//	{
	//		ClippingArea = &_clippingAreaStack.top();
	//	}
	//}

	TileSurface::TileSurface() :
		Array2d<Tile>(),
		DefaultColors({Colors::DarkGrey, Colors::Black})
	{

	}

	TileSurface::TileSurface(const Dimensions &Size) :
		Array2d<Tile>(Size.H, Size.W),
		DefaultColors({Colors::DarkGrey, Colors::Black})
	{

	}

	void TileSurface::SetDefaultColors(const Color &FG, const Color &BG)
	{
		DefaultColors = { FG, BG };
	}

	void TileSurface::SetDefaultColors(const TileColor &Color)
	{
		DefaultColors = Color;
	}

	void TileSurface::SetTileColor(const Point &Position, const TileColor &Color)
	{
		if (Position.X >= 0 && Position.X < _width && Position.Y >= 0 && Position.Y < _height)
		{
			_data[Position.Y][Position.Y].Color = Color;
		}
	}

	void TileSurface::SetTileColor(const Rect &Position, const TileColor &Color)
	{
		for (int y = 0; y < Position.H; y++)
		{
			for (int x = 0; x < Position.W; x++)
			{
				Point Pos = { Position.X + x, Position.Y + y };
				SetTileColor(Pos, Color);
			}
		}
	}

	void TileSurface::SetTileCharacter(const Point &Position, const u16 Character)
	{
		if (Position.X >= 0 && Position.X < _width && Position.Y >= 0 && Position.Y < _height)
		{
			_data[Position.Y][Position.X].Character = Character;
		}
	}

	void TileSurface::DrawTile(const Point &Position, const u16 Character)
	{
		DrawTile(Position, Character, DefaultColors);
	}

	void TileSurface::DrawTile(const Point &Position, const u16 Character, const Color &FG, const Color &BG)
	{
		DrawTile(Position, Character, { FG, BG });
	}

	void TileSurface::DrawTile(const Point &Position, const u16 Character, const TileColor &Color)
	{
		u16 FinalChar = Character;
		TileColor FinalColor = Color;
		Point NewPos = { Position.X, Position.Y };

		if (NewPos.X >= 0 && NewPos.X < _width && NewPos.Y >= 0 && NewPos.Y < _height)
		{
			Tile &CurrentTile = _data[NewPos.Y][NewPos.X];

			// TODO: Accomodate for cases like transparent FG and opaque BG on a similar surface
			/*if (Color.bg.a != 255)
			{
				if (Color.bg == Colors::Transparent)
				{
					FinalColor.bg = CurrentTile.color.bg;
				}
				else
				{
					FinalColor.bg = MMT::Lerp(CurrentTile.color.bg, FinalColor.bg, static_cast<float>(FinalColor.bg.a) / 255.f);
				}
			}

			if (Color.fg.a != 255)
			{
				if (Color.fg == Colors::Transparent)
				{
					FinalColor.fg = FinalColor.bg;
				}
				else
				{
					FinalColor.fg = MMT::Lerp(FinalColor.bg, FinalColor.fg, static_cast<float>(FinalColor.fg.a) / 255.f);
				}
			}*/

			CurrentTile = { FinalChar, FinalColor };
			/*if (NewPos.x >= ClippingArea->x && NewPos.x < ClippingArea->x + ClippingArea->w && NewPos.y >= ClippingArea->y && NewPos.y < ClippingArea->y + ClippingArea->h)
			{
				CurrentTile = { FinalChar, FinalColor };
			}*/
		}
	}

	void TileSurface::DrawLine(const Point &Point1, const Point &Point2, const TileColor &Color1, const TileColor &Color2, const u16 Char)
	{
		TileColor StartColor = (Point1.X < Point2.X) ? (Color1) : (Color2);
		TileColor EndColor = (Point1.X > Point2.X) ? (Color1) : (Color2);
		u32 Width = (Point1.X > Point2.X) ? (Point1.X - Point2.X) : (Point2.X - Point1.X);
		u32 Height = (Point1.Y > Point2.Y) ? (Point1.Y - Point2.Y) : (Point2.Y - Point1.Y);
		for (u32 x = 0u; x < Width; x++)
		{
			int hoff = Height * (static_cast<float>(x) / static_cast<float>(Width));
			TileColor col = Lerp(StartColor, EndColor, (static_cast<float>(x) / static_cast<float>(Width)));
			DrawTile({ static_cast<s32>(x + Point1.X), static_cast<s32>(hoff + Point1.Y) }, Char, col);
		}
	}

	void TileSurface::DrawLine(const Point &Point1, const Point &Point2, const TileColor &Color, const u16 Char)
	{
		Point LeftMostPoint = (Point1.X > Point2.X) ? (Point2) : (Point1);
		u32 Width = (Point1.X > Point2.X) ? (Point1.X - Point2.X) : (Point2.X - Point1.X);
		u32 Height = (Point1.Y > Point2.Y) ? (Point1.Y - Point2.Y) : (Point2.Y - Point1.Y);
		for (u32 x = 0u; x < Width; x++)
		{
			DrawTile({ static_cast<s32>(x) + LeftMostPoint.X, static_cast<s32>((x / Width) * Height) + LeftMostPoint.Y }, Char, Color);
		}
	}

	void TileSurface::DrawText(const Point &Point, const TileColor &Color, const char* String)
	{
		u32 Length = strlen(String);//= strlen(FullString);
		for (u32 i = 0u; i < Length; i++)
		{
			DrawTile({ static_cast<s32>(Point.X + i), static_cast<s32>(Point.Y) }, String[i], Color);
		}
	}

	void TileSurface::DrawText(const Point &Point, const Dimensions &Bounds, const TileColor &Color, const char* String)
	{
		std::string Str = String;
		Util::StringWrap(Bounds, Str);
	}

	void TileSurface::DrawRect(const Rect &Rect, const TileColor &Color, const u16 Character, const bool &Fill)
	{
		if (Fill)
		{
			for (int y = 0; y < Rect.H; y++)
			{
				for (int x = 0; x < Rect.W; x++)
				{
					DrawTile({ x + Rect.X, y + Rect.Y }, Character, Color);
				}
			}
		}
		else
		{
			for (int w = 0; w < Rect.W; w++)
			{
				DrawTile({ Rect.X + w, Rect.Y }, Character, Color);
				DrawTile({ Rect.X + w, Rect.Y + Rect.H - 1}, Character, Color);
			}
			for (int h = 1; h < Rect.H - 1; h++)
			{
				DrawTile({ Rect.X, Rect.Y + h }, Character, Color);
				DrawTile({ Rect.X + Rect.W - 1, Rect.Y + h }, Character, Color);
			}
		}
	}

	// TODO: Implement fill option
	void TileSurface::DrawRectEx(const Rect &Rect, const TileColor &Color, const u16 HorizontalChar, const u16 VerticalChar, const u16 TopLeftChar, const u16 TopRightChar, const u16 BotLeftChar, const u16 BotRightChar, const bool Fill)
	{
		for (u32 x = 1u; x < Rect.W - 1u; x++)
		{
			DrawTile({ static_cast<s32>(Rect.X + x), static_cast<s32>(Rect.Y) }, HorizontalChar, Color);
			DrawTile({ static_cast<s32>(Rect.X + x), static_cast<s32>(Rect.Y + Rect.H - 1) }, HorizontalChar, Color);
		}

		for (u32 y = 1u; y < Rect.H - 1u; y++)
		{
			DrawTile({ static_cast<s32>(Rect.X), static_cast<s32>(Rect.Y + y) }, VerticalChar, Color);
			DrawTile({ static_cast<s32>(Rect.X + Rect.W - 1), static_cast<s32>(Rect.Y + y) }, VerticalChar, Color);
		}

		DrawTile({ Rect.X, Rect.Y }, TopLeftChar, Color);
		DrawTile({ static_cast<s32>(Rect.X + Rect.W - 1), static_cast<s32>(Rect.Y) }, TopRightChar, Color);
		DrawTile({ static_cast<s32>(Rect.X), static_cast<s32>(Rect.Y + Rect.H - 1) }, BotLeftChar, Color);
		DrawTile({ static_cast<s32>(Rect.X + Rect.W - 1), static_cast<s32>(Rect.Y + Rect.H - 1) }, BotRightChar, Color);
	}

	void TileSurface::ClearSurface(const Color &ClearColor)
	{
		for (int y = 0u; y < _height; y++)
		{
			for (int x = 0u; x < _width; x++)
			{
				_data[y][x] = { 0, { { 0, 0, 0 },{ 0, 0, 0 } } };
			}
		}

		/*SDL_SetRenderDrawColor(_SDLRenderer, ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
		SDL_RenderClear(_SDLRenderer);*/
	}

	void TileSurface::Blit(const Array2dRef<Tile> &From, const Point &Position) const
	{
		for (int y = 0; y < From.GetHeight(); y++)
		{
			for (int x = 0; x < From.GetWidth(); x++)
			{
				Tile &CurTile = From[y][x];
				if ((Position.X + x >= 0) && (Position.X + x < this->_width) && (Position.Y + y >= 0) && (Position.Y + y < this->_height))
				{
					_data[Position.Y + y][Position.X + x] = CurTile;
				}
			}
		}
		
		//Colors::Double()
	}
}