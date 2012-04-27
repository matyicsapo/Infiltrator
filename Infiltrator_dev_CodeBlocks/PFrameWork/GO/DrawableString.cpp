#include "DrawableString.hpp"

#include "../ResourceManager.hpp"
#include "../PFWConstants.hpp"

DrawableString::DrawableString (std::string text, std::string fontFile, float size, int layerDepth)
		: DrawableBase(layerDepth) {
	sf::Font* fontPtr = Resources->GetFont(fontFile);

	//sf::Uint8* ptr_unicode = (sf::Uint8*)text.c_str();
	//sf::Unicode::Text text_unicode(ptr_unicode);
	sfString = new sf::String((sf::Uint8*)text.c_str(), fontPtr == 0 ? sf::Font::GetDefaultFont() : *fontPtr, size);

	CalcHeight();

	sfString->SetCenter(
		(sfString->GetRect().Right - sfString->GetRect().Left) / sfString->GetScale().x / 2,
		height / sfString->GetScale().y / 2 * windowFakeScale);
}

DrawableString::DrawableString (std::string text, float size, int layerDepth)
		: DrawableBase(layerDepth) {
	sfString = new sf::String((sf::Uint8*)text.c_str(), sf::Font::GetDefaultFont(), size);

	CalcHeight();

	sfString->SetCenter(
		(sfString->GetRect().Right - sfString->GetRect().Left) / sfString->GetScale().x / 2,
		height / sfString->GetScale().y / 2 * windowFakeScale);
}

DrawableString::DrawableString (DrawableString const& xDrawableString) : DrawableBase(xDrawableString) {
	sfString = new sf::String( *(xDrawableString.sfString) );
	height = xDrawableString.height;
}

DrawableString::~DrawableString () {
	delete sfString;
}

void DrawableString::ReleaseResources () {
	Resources->ReleaseFont(&sfString->GetFont());
	delete sfString;
	sfString = 0;
}

void DrawableString::SetText (std::string text) {
	sfString->SetText((sf::Uint8*)text.c_str());

	sfString->SetCenter(
		(sfString->GetRect().Right - sfString->GetRect().Left) / sfString->GetScale().x / 2,
		height / sfString->GetScale().y / 2 * windowFakeScale);

	SetFakePos(CalcFakePos());
}

sf::Vector2f DrawableString::GetScreenSize () {
	CalcHeight();

	return sf::Vector2f((sfString->GetRect().Right - sfString->GetRect().Left), height);
}

void DrawableString::CalcHeight () {
	sf::Unicode::Text text = sfString->GetText();
	sfString->SetText(PFWConstants::stringMaxHeight);

	height = sfString->GetRect().GetHeight();

	sfString->SetText(text);
}
