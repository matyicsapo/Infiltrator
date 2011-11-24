#include "GUITextfield.hpp"

#include "../../GameObjectManager.hpp"
#include "../../PFWConstants.hpp"
#include "../../SFMLGameManager.hpp"



		#include <iostream>


void GUITextfield::HandleSFEvents (std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {

		switch (itSfEvent->Type) {
			case sf::Event::MouseMoved: {
				if (!active && txNormal != "" && txHover != "") {
					sf::Rect<float> btnBounds(GetScreenBounds());

					if (btnBounds.Contains(itSfEvent->MouseMove.X, itSfEvent->MouseMove.Y)) {
						if (!hover) {
							// if got in/on
							hover = true;
							SetImage(txHover);
						}
					}
					else if (hover) {
						// if got out/off
						active = false;
						hover = false;
						SetImage(txNormal);
					}
				}
			}
			break;
			case sf::Event::MouseButtonPressed: {
				sf::Rect<float> btnBounds(GetScreenBounds());

				if (btnBounds.Contains(itSfEvent->MouseButton.X, itSfEvent->MouseButton.Y)) {
					if (!active) {
						active = true;

						if (txHover != "") SetImage(txHover);

						sfString->SetColor(colTextActive);

						textCursor = new ScreenSprite("Content/Textures/textcursor.png", mAlignH, mAlignV, layerDepth);

						GameObjects->Update(0);

						textCursor->SetScale( sf::Vector2f(
							baseScale.x / 2
								* (sfString->GetSize() / PFWConstants::baseFontSize)
									* PFWConstants::fontSizeScaleMultiplier,
							baseScale.y * 2
								* (sfString->GetSize() / PFWConstants::baseFontSize)
									* PFWConstants::fontSizeScaleMultiplier) );

						CalcTextCursorPosY();
					}

					timeSinceLastFlash = 0;
					textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));

					if (GetText() != "") {
						if (std::string(GetText()).length() == 1) {
							std::cout << "a.x" << std::endl;

							if ( itSfEvent->MouseButton.X < sfString->TransformToGlobal(sfString->GetCharacterPos(1)).x ) {
								iTextCursor = 0;

								textCursor->SetPosition( sf::Vector2f(
									x_GetTextCursorX(),
									textCursorY) );

								break;
							}
							else {
								iTextCursor = 1;

								textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );

								break;
							}
						}
						else {
							for (unsigned int itChar = 1; itChar < std::string(GetText()).length(); itChar++) {
								if ( itSfEvent->MouseButton.X < sf::Vector2f(sfString->TransformToGlobal(sfString->GetCharacterPos(itChar))).x ) {
									iTextCursor = itChar - 1;

									textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );

									break;

								}
								else
								if (itChar == std::string(GetText()).length() - 1) {
									if ( itSfEvent->MouseButton.X < sfString->TransformToGlobal(sfString->GetCharacterPos(itChar + 1)).x ) {
										iTextCursor = itChar;

										textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );

										break;
									}
									else {
										iTextCursor = itChar + 1;

										textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );

										break;
									}
								}
							}
						}
					}
					else {
						textCursor->SetPosition( sf::Vector2f(
							x_GetTextCursorX(),
							textCursorY) );
					}
				}
				else if (active) {
					active = false;

					if (txNormal != "") SetImage(txNormal);

					sfString->SetColor(colTextInactive);

					Drawables->PopScreenSpace(textCursor);
					delete textCursor;
					textCursor = 0;
				}
			} break;
			case sf::Event::TextEntered: {
				if (active) {
					if (Game->GetRenderWindow()->GetInput().IsKeyDown(sf::Key::Delete)) {
						SetText( std::string(GetText()).erase(iTextCursor, 1) );

						textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
						timeSinceLastFlash = 0;
						textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));
					}
					else {
						switch (itSfEvent->Text.Unicode) {
							case 8: // backspace
								if (iTextCursor != 0) {
									SetText( std::string(GetText()).erase(--iTextCursor, 1) );

									textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
									timeSinceLastFlash = 0;
									textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));

									if (GetText() == "") {
										textCursor->SetPosition( sf::Vector2f(
											x_GetTextCursorX(),
											textCursorY) );
									}
								}
							break;
	//						case //286:
	//							127:
	//							// delete
	//							SetText( std::string(GetText()).erase(iTextCursor, 1) );
	//
	//							textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
	//							timeSinceLastFlash = 0;
	//							textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));
	//						break;
							default:
								std::string t = GetText();

								SetText( std::string(GetText()).insert(iTextCursor++, 1, char(itSfEvent->Text.Unicode)) );

								if (sfString->GetRect().GetWidth() >= GetScreenSize().x) {
									iTextCursor--;
									SetText(t);
									break;
								}

								textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
								timeSinceLastFlash = 0;
								textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));
							break;
						}
					}
				}
			} break;
			default:
				// don't care about no other event types
			break;
		}

		itSfEvent++;
	}
}

void GUITextfield::Update (float dT) {
	if (active) {
		timeSinceLastFlash += dT;
		if (timeSinceLastFlash > flashTime) {
			timeSinceLastFlash -= flashTime;

			if (textCursor->GetColor().a == 0)
				textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));
			else
				textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 0));
		}
	}
}

sf::Vector2f GUITextfield::GetScreenSize () {
	if (sfSprite != 0) {
		sf::Vector2f imgSize(sfSprite->GetImage()->GetWidth(), sfSprite->GetImage()->GetHeight());
		return sf::Vector2f(imgSize.x * sfSprite->GetScale().x, imgSize.y * sfSprite->GetScale().y);
	}

	CalcHeight();

	return sf::Vector2f(widthWoBg * windowFakeScale, height);
}


sf::Rect<float> GUITextfield::GetScreenBounds () {
	sf::Vector2f screenSize(GetScreenSize());
	sf::Vector2f fakePos(CalcFakePos());

	return sf::Rect<float>(fakePos.x,
							fakePos.y - (screenSize.y / 2),
							fakePos.x + (screenSize.x),
							fakePos.y + (screenSize.y / 2)
							 );
}

void GUITextfield::CalcHeight () {
	std::string t = GetText();
	sfString->SetText(PFWConstants::stringMaxHeight);

	height = sfString->GetRect().GetHeight();

	sfString->SetText(t);
}

float GUITextfield::GetTextCursorX () {
	switch (mAlignH) {
		case LEFT:
			return sfString->TransformToGlobal(sfString->GetCharacterPos(iTextCursor)).x /
				windowFakeScale - CalcFakePos().x / windowFakeScale + basePos.x;
		break;
		case CENTER:
			return sfString->TransformToGlobal(sfString->GetCharacterPos(iTextCursor)).x /
				windowFakeScale - CalcFakePos().x / windowFakeScale + basePos.x;
		break;
		default:// RIGHT:
			return sfString->TransformToGlobal(sfString->GetCharacterPos(iTextCursor)).x / windowFakeScale
				- CalcFakePos().x / windowFakeScale + basePos.x
				- GetScreenSize().x / windowFakeScale;
		break;
	}
}

float GUITextfield::x_GetTextCursorX () {
	switch (mAlignH) {
		case LEFT:
			return CalcFakePos().x / windowFakeScale;
		break;
		case CENTER:
			return CalcFakePos().x / windowFakeScale - GetScreenSize().x / windowFakeScale;
		break;
		default:// RIGHT:
			return CalcFakePos().x / windowFakeScale - PFWConstants::baseResolution.x;
		break;
	}
}

void GUITextfield::CalcTextCursorPosY () {
	std::string t = GetText();
	sfString->SetText(sf::Unicode::Text(&PFWConstants::charNonHanging));

	float baseLineY = sfString->GetRect().Bottom / windowFakeScale;

	switch (mAlignV) {
		case TOP:
			textCursorY = baseLineY - textCursor->GetScreenSize().y / windowFakeScale;
		break;
		case MIDDLE:
			textCursorY = baseLineY - PFWConstants::baseResolution.y / 2 - (textCursor->GetScreenSize().y / windowFakeScale) / 2;
		break;
		case BOTTOM:
			textCursorY = baseLineY - PFWConstants::baseResolution.y;
		break;
	}

	sfString->SetText(t);
}

void GUITextfield::SetText (std::string text) {
	sfString->SetText(text);

	sfString->SetCenter(0, height / sfString->GetScale().y / 2);

	SetFakePos(CalcFakePos());
}

sf::Vector2f GUITextfield::CalcFakePos () {
	sf::Vector2f fakePos;

	switch (mAlignH) {
		case LEFT:
			fakePos.x = basePos.x * windowFakeScale;
		break;
		case CENTER:
			fakePos.x = Game->GetRenderWindow()->GetWidth() / 2 + basePos.x * windowFakeScale;
		break;
		case RIGHT:
			fakePos.x = Game->GetRenderWindow()->GetWidth() - GetScreenSize().x + basePos.x * windowFakeScale;
		break;
	}

	switch (mAlignV) {
		case TOP:
			fakePos.y = GetScreenSize().y / 2 + basePos.y * windowFakeScale;
		break;
		case MIDDLE:
			fakePos.y = Game->GetRenderWindow()->GetHeight() / 2 + basePos.y * windowFakeScale;
		break;
		case BOTTOM:
			fakePos.y = Game->GetRenderWindow()->GetHeight() - GetScreenSize().y / 2 + basePos.y * windowFakeScale;
		break;
	}

	return fakePos;
}
