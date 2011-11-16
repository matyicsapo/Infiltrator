#include "DrawableString.hpp"

#include "../ResourceManager.hpp"

DrawableString::DrawableString (std::string text, std::string fontFile, float size, int layerDepth)
		: DrawableBase(layerDepth) {
	sf::Font* fontPtr = Resources->GetFont(fontFile);

	sfString = new sf::String(text, fontPtr == 0 ? sf::Font::GetDefaultFont() : *fontPtr, size);

	sfString->SetCenter( (sfString->GetRect().Right - sfString->GetRect().Left) / sfString->GetScale().x / 2,
						(sfString->GetRect().Bottom - sfString->GetRect().Top) / sfString->GetScale().y / 2 );
}

DrawableString::DrawableString (std::string text, float size, int layerDepth)
		: DrawableBase(layerDepth) {
	sfString = new sf::String(text, sf::Font::GetDefaultFont(), size);

	sfString->SetCenter( (sfString->GetRect().Right - sfString->GetRect().Left) / sfString->GetScale().x / 2,
						(sfString->GetRect().Bottom - sfString->GetRect().Top) / sfString->GetScale().y / 2 );
}

void DrawableString::SetText (std::string text) {
	sfString->SetText(text);
	sfString->SetCenter( (sfString->GetRect().Right - sfString->GetRect().Left) / sfString->GetScale().x / 2,
						(sfString->GetRect().Bottom - sfString->GetRect().Top) / sfString->GetScale().y / 2 );
	SetFakePos(CalcFakePos());
}
