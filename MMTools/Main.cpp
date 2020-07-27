#include "Engine.h"

#include "Widgets\Button.hpp"
#include "Widgets\Label.hpp"
#include "Widgets\Textbox.hpp"

class EchoWidget : public MMT::IWidget
{
public:
	EchoWidget(MMT::IWidget *Parent, const MMT::Rect &BoundingBox) :
		MMT::IWidget(Parent, BoundingBox)
	{

	}

	virtual void OnKeyPress(const MMT::KeyboardData &Keyboard, const key_t Key) override
	{
		char InKey = MMT::Util::SDLScancodeToChar(Key, SDL_GetModState());
		fmt::printf("[%c]\n", InKey);
		//fmt::printf("Pressed '%s'.\n", SDL_GetScancodeName(static_cast<SDL_Scancode>(Key)));
	}
};

class TestGameState : public MMT::GameState
{
private:
	MMT::Widgets::Button *Btn1, *Btn2;
	MMT::Widgets::Label *Lbl1;
	MMT::Widgets::Textbox *Txt1;

	EchoWidget *Echo1;
public:
	TestGameState(MMT::Engine &Parent) :
		MMT::GameState(Parent, "Test Game State")
	{

	}

	virtual bool Load() override
	{
		Btn1 = new MMT::Widgets::Button(nullptr, { 2, 2 }, "BUTTON!", [](MMT::IWidget*) {
			fmt::printf("BEEP\n");
		});
		Btn1->bCanBeFocused = false;
		AddWidget(Btn1);

		Lbl1 = new MMT::Widgets::Label(nullptr, { 2, 4 }, "Label:");
		AddWidget(Lbl1);

		Btn2 = new MMT::Widgets::Button(nullptr, { 2, 5 }, "OTHERBUTTON!", [](MMT::IWidget*) {
			fmt::printf("BEWP\n");
		});
		AddWidget(Btn2);

		Txt1 = new MMT::Widgets::Textbox(nullptr, { 2,7 }, 20, "Type to chat...", SDL_SCANCODE_RETURN, [](MMT::IWidget *Self)
		{
			MMT::Widgets::Textbox *SelfCast = static_cast<MMT::Widgets::Textbox*>(Self);
			const std::string &Text = SelfCast->GetText();
			if (Text.length() > 0)
			{
				fmt::print("'{}'\n", Text);
				SelfCast->ClearText();
			}
			Self->Unfocus();
		});
		AddWidget(Txt1);

		Echo1 = new EchoWidget(nullptr, { 1, 1, 1, 1 });
		AddWidget(Echo1);
		SetCoreWidget(Echo1);

		return true;
	}

	virtual void Cleanup() override
	{
		delete Btn1;
	}

	virtual void Render(MMT::TileSurface &DrawSurface) override
	{
		MMT::GameState::Render(DrawSurface);
		//Btn1->Draw(DrawSurface);
		//DrawSurface.DrawText({ 5, 5 }, { MMT::Colors::LightGrey, MMT::Colors::Black }, "eY bB u WaN sUm FuK");
	}
};

int main(int argc, char *argv[])
{
	MMT::Engine Engine = MMT::Engine("Test", { 50, 50 });
	Engine.RegisterState(1, new TestGameState(Engine));
	Engine.PushState(1);
	Engine.Run();
	return 0;
}