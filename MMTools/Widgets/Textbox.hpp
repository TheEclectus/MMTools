#include "../Widget.h"

#include "../Utility.h"

namespace MMT
{
	namespace Widgets
	{
		class Textbox : public IWidget
		{
		protected:
			TileColor ColorsNothingEntered = { MMT::Colors::Grey, MMT::Colors::DarkGrey };
			TileColor ColorsTextEntered = { MMT::Colors::LightGrey, MMT::Colors::DarkGrey };
			std::string EnteredText, Message;
			u32 CaratPosition, VisibleFieldOffset;
			key_t ActionKey;	// An SDL_Scancode value; 0 means no action key.
			std::function<void(IWidget*)> ActionCallback;	// Parameter is pointer to self
		public:
			Textbox(IWidget *Parent, const Point &Position, s32 Width, const std::string &Message, key_t ActionKey = 0u, std::function<void(IWidget*)> Callback = std::function<void(IWidget*)>()) :
				IWidget(Parent, {Position.X, Position.Y, Width, 1}),
				Message(Message),
				ActionKey(ActionKey),
				ActionCallback(Callback),
				CaratPosition(0u),
				VisibleFieldOffset(0u)
			{
				bRepeatKeyboardInput = true;
			}

			const std::string &GetText() const
			{
				return EnteredText;
			}

			void SetText(const std::string &Text, bool bResetCaratToEnd = true)
			{
				EnteredText = Text;
				if (bResetCaratToEnd)
				{
					CaratPosition = EnteredText.length();
				}
			}

			// At current carat position
			void InsertText(const std::string &Text)
			{
				for (char i : Text)
				{
					InsertCharacter(i);
				}
			}

			void InsertCharacter(char Character)
			{
				if (Character != '\n' && Character != '\r' && Util::bIsKeyboardChar(Character))
				{
					if (Character == '\t') InsertText("    ");
					else
					{
						EnteredText.insert(CaratPosition, 1, Character);
						CaratPosition++;
					}
				}
			}

			void ClearText()
			{
				EnteredText.clear();
				CaratPosition = 0u;
				VisibleFieldOffset = 0u;
			}

			virtual void Draw(MMT::TileSurface &DrawSurface) override
			{
				if (!this->IsFocused())
				{
					DrawSurface.DrawLine(this->BoundingBox.Position(), MMT::Point(this->BoundingBox.X + this->BoundingBox.W, this->BoundingBox.Y ), ColorsNothingEntered, ' ');
					if(!EnteredText.length()) 
						DrawSurface.DrawText(BoundingBox.Position(), ColorsNothingEntered, Message.c_str());
					else
						DrawSurface.DrawText(BoundingBox.Position(), ColorsTextEntered, EnteredText.c_str());
				}
				else
				{
					DrawSurface.DrawLine(this->BoundingBox.Position(), MMT::Point(this->BoundingBox.X + this->BoundingBox.W, this->BoundingBox.Y), ColorsTextEntered, ' ');
					//std::string TextToDraw = 
					DrawSurface.DrawText(BoundingBox.Position(), ColorsTextEntered, EnteredText.substr(VisibleFieldOffset, this->BoundingBox.W).c_str());

					if (SDL_GetTicks() % 1100 <= 550)
					{
						s32 CaratPos = (BoundingBox.X + CaratPosition) - VisibleFieldOffset;

						if (CaratPosition == EnteredText.length())
							DrawSurface.DrawTile(MMT::Point(CaratPos, BoundingBox.Y), '|', ColorsTextEntered);
						else
							DrawSurface.DrawTile(MMT::Point(CaratPos /*BoundingBox.X + CaratPosition*/, BoundingBox.Y), EnteredText[CaratPosition], ColorsTextEntered.Flip());
					}
				}
			}

			// TODO: Implement text selection
			virtual void OnKeyPress(const MMT::KeyboardData &Keyboard, const key_t Key) override
			{
				if (ActionKey != 0u && Key == ActionKey && ActionCallback)
				{
					ActionCallback(this);
				}
				else
				{
					switch (Key)
					{
					case SDL_SCANCODE_BACKSPACE:
						if ((EnteredText.length() != 0) && (CaratPosition > 0))
						{
							EnteredText.erase(CaratPosition - 1, 1u);
							CaratPosition--;

							//EnteredText.pop_back();
						}
						break;
					case SDL_SCANCODE_DELETE:
						if ((EnteredText.length() != 0) && (CaratPosition < EnteredText.length()))
						{
							EnteredText.erase(CaratPosition, 1u);
						}
						break;
					case SDL_SCANCODE_LEFT:
						if (CaratPosition > 0)
						{
							if (Keyboard.bCtrlDown)
							{
								size_t Loc = EnteredText.rfind(' ', CaratPosition - 1);
								if(Loc == CaratPosition - 1)
									Loc = EnteredText.rfind(' ', CaratPosition - 2);

								if (Loc == EnteredText.npos) Loc = 0u;
								else Loc += 1;
								CaratPosition = Loc;
							}
							else
								CaratPosition--;
						}
						break;
					case SDL_SCANCODE_RIGHT:
						if (CaratPosition < EnteredText.length())
							if (Keyboard.bCtrlDown)
							{
								size_t Loc = EnteredText.find(' ', CaratPosition + 1);

								if (Loc == CaratPosition + 1)
									Loc = EnteredText.find(' ', CaratPosition + 2);

								if (Loc == EnteredText.npos) Loc = EnteredText.length();
								else Loc += 1;
								
								CaratPosition = Loc;
							}
							else
								CaratPosition++;
						break;
					default:
						if (Keyboard.bCtrlDown)
						{
							if (Key == SDL_SCANCODE_V)
							{
								char *ClipboardTextBuf = SDL_GetClipboardText();
								//std::string ClipboardText(ClipboardTextBuf);
								InsertText(ClipboardTextBuf);
								SDL_free(ClipboardTextBuf);
							}
							else if (Key == SDL_SCANCODE_C)
							{
								std::string SelectedText = "";

								if (CaratPosition < EnteredText.length())
								{
									SelectedText += EnteredText[CaratPosition];
								}
								SDL_SetClipboardText(SelectedText.c_str());
								fmt::print("Copied '{}'\n", SelectedText);
							}
						}

						//	A-Z, 0-9				   space -=[]\					   ;'`,./
						else if ((Key >= 4 && Key <= 39) || (Key >= 44 && Key <= 49) || (Key >= 51 && Key <= 56))
						{
							InsertCharacter(MMT::Util::SDLScancodeToChar(Key, SDL_GetModState()));
							
							// EnteredText.insert(CaratPosition, 1, MMT::Util::SDLScancodeToChar(Key, SDL_GetModState()));
							// CaratPosition++;
						}
						break;
					}

					if (CaratPosition > VisibleFieldOffset + this->BoundingBox.W - 1)
					{
						VisibleFieldOffset = CaratPosition - this->BoundingBox.W + 1;
					}
					else if (CaratPosition < VisibleFieldOffset)
					{
						VisibleFieldOffset = CaratPosition;
					}

					if (VisibleFieldOffset != 0 && VisibleFieldOffset + this->BoundingBox.W > EnteredText.length())
					{
						VisibleFieldOffset = (this->BoundingBox.W > EnteredText.length()) ? 0 : EnteredText.length() - this->BoundingBox.W + 1;
						//VisibleFieldOffset = std::max(0u, EnteredText.length() - this->BoundingBox.X);
					}
				}
			}
		};
	}
}