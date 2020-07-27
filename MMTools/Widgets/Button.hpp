#include "../Widget.h"

#include <functional>

namespace MMT
{
	namespace Widgets
	{
		class Button : public IWidget
		{
		protected:
			TileColor ColorsDefault = { MMT::Colors::White, MMT::Colors::Black };
			TileColor ColorsMouseOver = { MMT::Colors::Yellow, MMT::Colors::Black}; 
			TileColor ColorsMouseDownOver = ColorsDefault.Flip(); //{ MMT::Colors::Yellow, MMT::Colors::Black };
			std::string Text;
			std::function<void(IWidget*)> Callback; // Parameter is pointer to self.
		public:
			Button(IWidget *Parent, const MMT::Point &Position, const std::string &Text, std::function<void(IWidget*)> Callback) :
				IWidget(Parent, {Position.X, Position.Y, static_cast<s32>(Text.length()), 1}),
				Text(Text),
				Callback(Callback)
			{
				bCanBeFocused = false;
			}

			virtual void Draw(MMT::TileSurface &DrawSurface) override
			{
				MMT::TileColor &Col = IsMouseDownOver(0) ? ColorsMouseDownOver : (IsMouseOver() ? ColorsMouseOver : ColorsDefault);
				DrawSurface.DrawText(BoundingBox.Position(), Col, Text.c_str());
			}

			virtual void OnMouseUp(const MMT::MouseData &Mouse, const u8 Button) override
			{
				if (Button == 0u)
				{
					Callback(this);
				}
			}
		};
	}
}
