#include "Widget.h"

#include "GameState.h"
#include "Utility.h"

namespace MMT
{
	IWidget *WidgetList::PickTop(const Point &Position)
	{
		IWidget *MostFitWidget = nullptr;

		// Check if any of the children are more fit that the parent
		auto i = rbegin();
		while (i != rend())
		{
			if ((*i)->GetBoundingBox().IsPointInside(Position) && (*i)->bIsVisible)
			{
				// Position inside, Has children
				if ((*i)->HasChildren())
				{
					// If there are no fit children, then the parent is fit
					IWidget *Candidate = (*i)->PickTop(Position);
					if (Candidate != nullptr)
					{
						MostFitWidget = Candidate;
					}
				}
				if (MostFitWidget == nullptr)
				{
					MostFitWidget = *i;
				}
				break;
			}
			i++;
		}

		return MostFitWidget;
	}

	IWidget::IWidget(IWidget *Parent, const Rect &BoundingBox) :
		ParentWidget(Parent),
		BoundingBox(BoundingBox)
	{
		xassert(BoundingBox.W > 0, "BoundingBox width cannot be less than 1.");
		xassert(BoundingBox.H > 0, "BoundingBox height cannot be less than 1.");
	}

	IWidget::~IWidget()
	{
		if (ParentWidget)
		{
			auto &ParentChildren = ParentWidget->Children;
			auto ResIter = std::find(ParentChildren.begin(), ParentChildren.end(), this);
			
			if (ResIter != ParentChildren.end())
			{
				ParentWidget->Children.erase(ResIter);
			}
			else
			{
				// This means the parent didn't have the widget as a child... whoops!
			}
		}
	}

	void IWidget::Draw(MMT::TileSurface &DrawSurface)
	{
		
	}

	bool IWidget::IsMouseOver() const
	{
		return bMouseOver;
	}

	bool IWidget::IsMouseDownOver(const u8 Button) const
	{
		xassert(Button <= 2, "Index must be between 0 and 2.");
		return bMouseDownOver[Button];
	}

	bool IWidget::IsFocused() const
	{
		return bIsFocused;
	}

	MMT::IWidget *IWidget::PickTop(const MMT::Point &Position)
	{
		return Children.PickTop(Position);
	}

	void IWidget::Focus()
	{
		if (ParentState) ParentState->Focus(this);
	}

	void IWidget::Unfocus()
	{
		if (ParentState && (ParentState->GetFocusedWidget() == this)) ParentState->Unfocus();
	}

	const u8 IWidget::GetStateFlags() const
	{
		//return StateFlags;
		return 0u;
	}

	const MMT::Rect IWidget::GetBoundingBox() const
	{
		return BoundingBox;
	}

	bool IWidget::HasChildren() const
	{
		return (Children.size() != 0);
	}

	void IWidget::AddChild(MMT::IWidget *Widget)
	{
		xassert(Widget->ParentState == this->ParentState, "Cannot add child Widget from different state.");
		xassert(Widget->ParentWidget == nullptr, "Cannot add child Widget that already has a parent.");

		Widget->ParentWidget = this;
		Children.push_back(Widget);
	}

	void IWidget::RemoveChild(MMT::IWidget *Widget)
	{
		auto Iter = std::find(Children.begin(), Children.end(), Widget);
		if (Iter != Children.end())
		{
			Children.erase(Iter);
			Widget->ParentWidget = nullptr;
		}
	}

	void IWidget::OnFocus()
	{
		fmt::printf("Focused.\n");
	}

	void IWidget::OnUnfocus()
	{
		fmt::printf("Unfocused.\n");
	}

	void IWidget::OnMouseOver(const MouseData &Mouse)
	{
		//fmt::printf("Mouse over.\n");
	}

	void IWidget::OnMouseLeave(const MouseData &Mouse)
	{
		//fmt::printf("Mouse leave.\n");
	}

	void IWidget::OnMouseDown(const MouseData &Mouse, const u8 Button)
	{
		//fmt::printf("Mouse down.\n");
	}

	void IWidget::OnMouseUp(const MouseData &Mouse, const u8 Button)
	{
		//fmt::printf("Mouse up.\n");
	}

	void IWidget::OnMouseMove(const MouseData &Mouse)
	{
		fmt::printf("Mouse moved.\n");
	}

	void IWidget::OnKeyPress(const KeyboardData &Keyboard, const key_t Key)
	{
		char InKey = MMT::Util::SDLScancodeToChar(Key, SDL_GetModState());
		fmt::printf("[%c]\n", InKey);
		//fmt::printf("Pressed '%s'.\n", SDL_GetScancodeName(static_cast<SDL_Scancode>(Key)));
	}

	void IWidget::OnKeyRelease(const KeyboardData &Keyboard, const key_t Key)
	{
		//fmt::printf("Released '%c'.\n", Key);
	}

	/*void IWidget::OnMouseClick(const MouseData &Mouse, const u8 Button)
	{

	}*/
}