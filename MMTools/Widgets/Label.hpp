#include "../Widget.h"

namespace MMT
{
	namespace Widgets
	{
		class Label : public IWidget
		{
		protected:
			TileColor ColorsDefault = { MMT::Colors::LightGrey, MMT::Colors::Black };
			std::string Text;
		public:
			Label(IWidget *Parent, const Point Position, const std::string &Text) :
				IWidget(Parent, { Position.X, Position.Y, static_cast<s32>(Text.length()), 1 }),
				Text(Text)
			{
				bCanBeFocused = false;
			}

			virtual void Draw(MMT::TileSurface &DrawSurface) override
			{
				DrawSurface.DrawText(BoundingBox.Position(), ColorsDefault, Text.c_str());
			}
		};
	}
}