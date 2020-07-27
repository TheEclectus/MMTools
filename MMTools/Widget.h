#pragma once

#include <vector>

#include "TileSurface.h"
#include "Typedefs.h"

namespace MMT
{
	class IWidget;
	class WidgetList;

	class WidgetList : public std::vector<IWidget*>
	{
	public:
		// TODO: Potentially implement a picking algorithm based on a bitmap instead of just the bounding boxes
		IWidget *PickTop(const Point &Position);
	};

	enum class IWidgetStateFlags : u8
	{
		MouseDown		= 1u,
		MouseDownOver	= 2u,
		IsFocused		= 4u
	};

	class IWidget
	{
		friend class MMT::GameState;
	private:
		//u8			StateFlags;	// GameState is responsible for setting these
		// TODO: Use StateFlags instead of bools.
		bool bMouseOver			= false;
		bool bMouseDownOver[3]	= { false, false, false };
		bool bIsFocused			= false;

	protected:
		IWidget		*ParentWidget	= nullptr;
		GameState	*ParentState;
		Rect		BoundingBox;
		WidgetList	Children;
		
	public:
		bool bIsVisible				= true;
		bool bRepeatKeyboardInput	= false;
		bool bCanBeFocused			= true;

		IWidget(IWidget *Parent, const Rect &BoundingBox);
		~IWidget();
		virtual void Draw(MMT::TileSurface &DrawSurface);
		// TODO: Implement events after boolean state flags
		/*virtual void OnFocus();
		virtual void OnUnfocus();
		virtual void OnMouseEnter(const MouseData &Mouse);
		virtual void OnMouseLeave(const MouseData &Mouse);
		virtual void OnMouseDown(const MouseData &Mouse, u8 Button);
		virtual void OnMouseUp(const MouseData &Mouse, u8 Button);*/

		/// <summary>Returns true if the mouse cursor is over the IWidget.</summary>
		bool IsMouseOver() const;

		/// <summary>Returns true if the mouse is both over the IWidget and the
		/// specified button is down.</summary>
		/// <param name="Button">The mouse button to check.</param>
		bool IsMouseDownOver(const u8 Button) const;

		/// <summary>Returns true if the IWidget is currently the focus.</summary>
		bool IsFocused() const;

		// Position is widget-relative
		IWidget *PickTop(const Point &Position);
		void Focus();
		void Unfocus();
		const u8 GetStateFlags() const;
		const Rect GetBoundingBox() const;
		bool HasChildren() const;
		void AddChild(MMT::IWidget *Widget);
		void RemoveChild(MMT::IWidget *Widget);

		// Events
		virtual void OnFocus();
		virtual void OnUnfocus();
		virtual void OnMouseOver(const MouseData &Mouse);
		virtual void OnMouseLeave(const MouseData &Mouse);
		virtual void OnMouseDown(const MouseData &Mouse, const u8 Button);
		// Just releasing the mouse over the widget.
		virtual void OnMouseUp(const MouseData &Mouse, const u8 Button);
		virtual void OnMouseMove(const MouseData &Mouse);

		virtual void OnKeyPress(const KeyboardData &Keyboard, const key_t Key);
		virtual void OnKeyRelease(const KeyboardData &Keyboard, const key_t Key);

		// TODO: Potentially implement OnMouseClick()
		// Down and then up, both over the widget.
		//virtual void OnMouseClick(const MouseData &Mouse, const u8 Button);
	};
}