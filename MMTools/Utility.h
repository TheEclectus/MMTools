#pragma once

#include <string>
#include <type_traits>
#include <vector>

//#include "Utility.h"
//#include "Compression.h" // TODO: This include could be dangerous, it's in a header file that could be used anywhere.

namespace MMT
{
	namespace Util
	{
		//inline bool LoadXPFile(const std::string &Path, Array3d<Tile> *&Destination)
		//{
		//	auto Result = DecompressFile(Path);
		//	if (!Result.bSuccessful)
		//		return false;
		//	else
		//	{
		//		u32 DataSize = Result.Size;
		//		uptr Data = reinterpret_cast<uptr>(Result.Output);

		//		// Skip the XP file format section that's been causing so much grief.
		//		Data += 4;

		//		// Top-level data
		//		u32 Layers = *reinterpret_cast<u32*>(reinterpret_cast<void*>(Data));
		//		Data += 4;
		//		u32 Width = *reinterpret_cast<u32*>(reinterpret_cast<void*>(Data));
		//		u32 Height = *reinterpret_cast<u32*>(reinterpret_cast<void*>(Data + 4));

		//		//Array3d<MTile> SpriteSheet(Layers, Height, Width);
		//		Destination = new Array3d<Tile>(Layers, Height, Width);

		//		for (u32 i = 0; i < Layers; i++)
		//		{
		//			Data += 8;	// Skip past the unnecessarily-placed width/height
		//			Array2dRef<Tile> &CurLayer = (*Destination)[i];

		//			for (int i = 0; i < Height * Width; i++)
		//			{
		//				Tile &Tile = CurLayer[i % Height][i / Height];
		//				Tile *Loc = &CurLayer[i % Height][i / Height];
		//				Tile.Character = *reinterpret_cast<u32*>(reinterpret_cast<void*>(Data));
		//				Tile.Color.FG.R = *reinterpret_cast<u8*>(reinterpret_cast<void*>(Data + 4));
		//				Tile.Color.FG.G = *reinterpret_cast<u8*>(reinterpret_cast<void*>(Data + 5));
		//				Tile.Color.FG.B = *reinterpret_cast<u8*>(reinterpret_cast<void*>(Data + 6));
		//				//Tile.Color.fg.a = 255;
		//				Tile.Color.BG.R = *reinterpret_cast<u8*>(reinterpret_cast<void*>(Data + 7));
		//				Tile.Color.BG.G = *reinterpret_cast<u8*>(reinterpret_cast<void*>(Data + 8));
		//				Tile.Color.BG.B = *reinterpret_cast<u8*>(reinterpret_cast<void*>(Data + 9));
		//				//Tile.Color.bg.a = 255;

		//				Data += 10;
		//			}

		//			//CurLayer.PrintPreview();

		//			/*for (int w = 0; w < Width; w++)
		//			{
		//				for (int h = 0; h < Height; h++)
		//				{
		//					auto &Tile = CurLayer[h][w];

		//					Tile.character = *reinterpret_cast<uint32*>(reinterpret_cast<void*>(Data));
		//					Tile.color.fg.r = *reinterpret_cast<uint8*>(reinterpret_cast<void*>(Data + 4));
		//					Tile.color.fg.g = *reinterpret_cast<uint8*>(reinterpret_cast<void*>(Data + 5));
		//					Tile.color.fg.b = *reinterpret_cast<uint8*>(reinterpret_cast<void*>(Data + 6));
		//					Tile.color.fg.a = 255;
		//					Tile.color.bg.r = *reinterpret_cast<uint8*>(reinterpret_cast<void*>(Data + 7));
		//					Tile.color.bg.g = *reinterpret_cast<uint8*>(reinterpret_cast<void*>(Data + 8));
		//					Tile.color.bg.b = *reinterpret_cast<uint8*>(reinterpret_cast<void*>(Data + 9));
		//					Tile.color.bg.a = 255;

		//					Data += 10;

		//					fmt::printf((Tile.character != 32) ? ("+") : (" "));
		//				}
		//				fmt::printf("\n");
		//			}
		//			Data += sizeof(MTile);*/
		//		}

		//		delete[] Result.Output;

		//		//Destination = SpriteSheet;
		//		//Destination.Claim(SpriteSheet);

		//		return true;
		//	}
		//}

		template<class T>
		inline void Latch(T &Variable, const T &Lower, const T &Upper)
		{
			if (!std::is_const<T>() && (std::is_integral<T>() || std::is_floating_point<T>()))
			{
				if (Variable < Lower) Variable = Lower;
				else if (Variable > Upper) Variable = Upper;
			}
		}

		template<class T>
		inline T Latch(const T &Value, const T &Lower, const T &Upper)
		{
			if (!std::is_const<T>() && (std::is_integral<T>() || std::is_floating_point<T>()))
			{
				if (Variable < Lower) return Lower;
				else if (Variable > Upper) return Upper;
				else return Value;
			}
		}

		inline char SDLKeyToChar(const u16 Keycode, const u16 Keymods)
		{
			// Fuck you for forcing me to do this, SDL.
			// You're so wonderfully imperfect.

			if (Keycode != SDLK_UNKNOWN)
			{
				if (Keycode >= 32 && Keycode <= 126)
				{
					if (Keymods & SDL_Keymod::KMOD_LSHIFT || Keymods & SDL_Keymod::KMOD_RSHIFT)
					{
						if (Keycode >= SDLK_a && Keycode <= SDLK_z)
						{
							return static_cast<char>(Keycode - 32);
						}

						switch (Keycode)
						{
						case SDLK_0: return ')';
						case SDLK_1: return '!';
						case SDLK_2: return '@';
						case SDLK_3: return '#';
						case SDLK_4: return '$';
						case SDLK_5: return '%';
						case SDLK_6: return '^';
						case SDLK_7: return '&';
						case SDLK_8: return '*';
						case SDLK_9: return '(';
						case SDLK_LEFTBRACKET: return '{';
						case SDLK_RIGHTBRACKET: return '}';
						case SDLK_SEMICOLON: return ':';
						case SDLK_QUOTE: return '\"';
						case SDLK_BACKSLASH: return '|';
						case SDLK_SLASH: return '?';
						case SDLK_COMMA: return '<';
						case SDLK_PERIOD: return '>';
						case SDLK_BACKQUOTE: return '~';
						case SDLK_MINUS: return '_';
						case SDLK_EQUALS: return '+';
						case SDLK_SPACE: return ' ';
						default: return '\0';
						}
					}

					return static_cast<char>(Keycode);
				}
				switch (Keycode)
				{
				case SDLK_BACKSPACE:	return '\b';
				case SDLK_TAB:			return '\t';
				case SDLK_RETURN:		return '\r';	// Enter
				case SDLK_ESCAPE:		return '\033';
				default:				return '\0';
				}
			}
			return 0;

			/*switch (Keycode)
			{
			case SDLK_BACKSPACE:	return '\b';
			case SDLK_TAB:			return '\t';
			case SDLK_RETURN:		return '\r';	// Enter
			case SDLK_ESCAPE:		return '\033';
			case SDLK_SPACE:		return ' ';
			case SDLK_EXCLAIM:		return '!';
			case SDLK_UNKNOWN:
			default:
				if (Keycode >= 33 && Keycode <= 126)
				{
					if (Keymods & SDL_Keymod::KMOD_LSHIFT || Keymods & SDL_Keymod::KMOD_RSHIFT)
					{
						if (Keycode >= SDLK_a && Keycode <= SDLK_z)
						{
							return static_cast<char>(Keycode - 32);
						}
						switch (Keycode)
						{

						}
					}
					
					return static_cast<char>(Keycode);
				}
				return '\0';
			}*/
		}

		inline char SDLScancodeToChar(const u16 Scancode, const u16 Keymods)
		{
			return SDLKeyToChar(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(Scancode)), Keymods);
		}

		inline bool bIsKeyboardCharScancode(key_t Key)
		{
			return ((Key >= 4 && Key <= 39) || (Key >= 44 && Key <= 49) || (Key >= 51 && Key <= 56));
		}

		inline bool bIsKeyboardChar(char Character)
		{
			static std::string ValidChars = " 1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM,.?-=[]\\;'/\t\n\r~!@#$%^&*()_+{}|:\"<>`";
			return ValidChars.find(Character) != ValidChars.npos;
		}

		// TODO: Implement bKeepWordsWhole functionality; currently attempts to keep words whole by default
		inline Rect StringWrap(const Dimensions& Bounds, std::string &String, bool bKeepWordsWhole = true)
		{
			if (String.length() <= Bounds.W)
				return { static_cast<s32>(String.length()), 1 };

			std::string NewString;
			int CurrentLineLength = 0, CurrentLines = 1;
			std::vector<std::string> WordList;
			{
				char *Next = strtok((char*)String.c_str(), " ");
				while (Next)
				{
					WordList.push_back(Next);
					Next = strtok((char*)String.c_str(), " ");
				}
			}

			for (int i = 0; i < WordList.size(); i++)
			{
				if (i != 0)
				{
					NewString += ' ';
				}

				if (CurrentLineLength + WordList[i].length() > Bounds.W)
				{
					if (Bounds.H != 0 && CurrentLines + 1 > Bounds.H)
						break;
					NewString += '\n';
					CurrentLines++;
					CurrentLineLength = 0;
				}

				NewString += WordList[i];
				CurrentLineLength += WordList[i].length();
			}

			return { static_cast<s32>(CurrentLines == 1 ? CurrentLineLength : Bounds.W), CurrentLines };
		}
	}
}