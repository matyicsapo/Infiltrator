#include "GUITextfield.hpp"

#include "../../GameObjectManager.hpp"
#include "../../PFWConstants.hpp"
#include "../../SFMLGameManager.hpp"
#include <sstream>
//#include <iostream>

void GUITextfield::ReleaseResources () {
	Sprite::ReleaseResources();
	DrawableString::ReleaseResources();
}

void GUITextfield::HandleSFEvents (std::list<sf::Event>& sfEvents) {
	bool output = true;

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
				sf::Rect<float> bounds(GetScreenBounds());

				if (bounds.Contains(itSfEvent->MouseButton.X, itSfEvent->MouseButton.Y)) {
//std::cout << "contains" << std::endl;

					SetActive(true);

					CalcTextCursorPosY();

					timeSinceLastFlash = 0;
					textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));

					if (GetText() != "") {
//std::cout << "GetText() != ''" << std::endl;
						if (GetText().length() == 1) {
//std::cout << "GetText().length() == 1" << std::endl;
							if ( itSfEvent->MouseButton.X < sfString->TransformToGlobal(sfString->GetCharacterPos(1)).x / windowFakeScale ) {
//std::cout << "<" << std::endl;
								iTextCursor = 0;

								textCursor->SetPosition(sf::Vector2f(GetTextCursorX(), textCursorY));

								break;
							}
							else {
//std::cout << ">" << std::endl;
								iTextCursor = 1;

								textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );

								break;
							}
						}
						else {
//std::cout << "GetText().length() != 1" << std::endl;
							for (unsigned int itChar = 1; itChar < GetText().length(); itChar++) {
								if ( itSfEvent->MouseButton.X < sfString->TransformToGlobal(sfString->GetCharacterPos(itChar)).x / windowFakeScale ) {
									iTextCursor = itChar - 1;

									textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );

									break;

								}
								else
								if (itChar == GetText().length() - 1) {
									if ( itSfEvent->MouseButton.X < sfString->TransformToGlobal(sfString->GetCharacterPos(itChar + 1)).x / windowFakeScale ) {
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
							x_GetTextCursorX()
//								+ .9f * -GetScreenSize().x / 2
																	,
							textCursorY) );
					}
				}
				else if (active) {
					SetActive(false);

					if (onApply != 0) {
						onApply();
					}
				}
			} break;
			case sf::Event::KeyPressed: {
				if (active) {
					switch (itSfEvent->Key.Code) {
						case sf::Key::Delete: {
							std::string t = GetText();

							SetText( GetText().erase(iTextCursor, 1) );

//							if (!regex_match(GetText(), regexPattern)) {
//								++iTextCursor;
//								SetText(t);
//								break;
//							}

							textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
							timeSinceLastFlash = 0;
							textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));

							output = false;
						} break;
						case sf::Key::Return:
							if (onApply != 0) {
								onApply();

								output = false;
							}
						break;
						case sf::Key::Left:
							--iTextCursor;
							if (iTextCursor < 0) {
								iTextCursor = 0;
							}

							textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
							timeSinceLastFlash = 0;
							textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));
						break;
						case sf::Key::Right:
							++iTextCursor;
							if (iTextCursor > (int)GetText().length()) {
								iTextCursor = GetText().length();
							}

							textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
							timeSinceLastFlash = 0;
							textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));
						break;
						case sf::Key::Home:
							iTextCursor = 0;

							textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
							timeSinceLastFlash = 0;
							textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));
						break;
						case sf::Key::End:
							iTextCursor = GetText().length();

							textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
							timeSinceLastFlash = 0;
							textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));
						break;
						default:
						break;
					}
				}
			}
			case sf::Event::TextEntered: {
				if (active) {
//					if (Game->GetRenderWindow()->GetInput().IsKeyDown(sf::Key::Delete)) {
//						SetText( std::string(GetText()).erase(iTextCursor, 1) );
//
//						textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
//						timeSinceLastFlash = 0;
//						textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));
//					}
//					else {
						switch (itSfEvent->Text.Unicode) {
							case 8: // backspace
								if (iTextCursor != 0) {
									std::string t = GetText();

									SetText( GetText().erase(--iTextCursor, 1) );

//									if (!regex_match(GetText(), regexPattern)) {
//										++iTextCursor;
//										SetText(t);
//										break;
//									}

									textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
									timeSinceLastFlash = 0;
									textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));

									if (GetText() == "") {
										textCursor->SetPosition( sf::Vector2f(
											x_GetTextCursorX()
//												+ .9f * -GetScreenSize().x / 2
																						,
											textCursorY) );
									}

									output = false;
								}
							break;
							case 127:
								// ahol rendesen elkapná ezis a Delete-et ott lekell kezelni, hogy
								// ne kerüljön defaultba
							break;
							case 13: // carriage return
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
								if (output) {
									sf::Uint32 u = itSfEvent->Text.Unicode;

									bool printable = false;

									for (unsigned int i = 0; i != PFWConstants::charset_length; i++) {
										if (u == PFWConstants::charset[i]) {
											printable = true;
											break;
										}
									}

									if (!printable)
										break;

									std::string t = GetText();

									SetText( GetText().insert(iTextCursor++, 1, u) );

									if (!regex_match(GetText(), regexPattern)) {
										--iTextCursor;
										SetText(t);
										break;
									}

									if (sfString->GetRect().GetWidth() >= GetScreenSize().x * .9f) {
										iTextCursor--;
										SetText(t);
										break;
									}

									textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
									timeSinceLastFlash = 0;
									textCursor->SetColor(sf::Color(colTextActive.r, colTextActive.g, colTextActive.b, 255));

									output = false;
								}
							break;
						}
//					}
				}
			} break;
			default:
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

	return sf::Rect<float>(fakePos.x + .9f * -GetScreenSize().x / 2,
							fakePos.y - (screenSize.y / 2),
							fakePos.x + (screenSize.x) - .9f * -GetScreenSize().x / 2,
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
				windowFakeScale - CalcFakePos().x / windowFakeScale + basePos.x
					//- GetScreenSize().x / 2
					;
		break;
		default:// RIGHT:
			return sfString->TransformToGlobal(sfString->GetCharacterPos(iTextCursor)).x / windowFakeScale
				- CalcFakePos().x / windowFakeScale + basePos.x
				- GetScreenSize().x / windowFakeScale
				//+ .9f * -GetScreenSize().x / 2
				;
		break;
	}
}

float GUITextfield::x_GetTextCursorX () {
	switch (mAlignH) {
		case LEFT:
			return CalcFakePos().x / windowFakeScale;
		break;
		case CENTER:
			return basePos.x - (.9f * GetScreenSize().x / 2) / windowFakeScale;
		break;
		default:// RIGHT:
			return CalcFakePos().x / windowFakeScale - PFWConstants::baseResolution.x
				- (.9f * GetScreenSize().x / 2) / windowFakeScale;
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
			textCursorY = basePos.y// - PFWConstants::baseResolution.y / 2
//				- (textCursor->GetScreenSize().y / windowFakeScale) / 2
				;
		break;
		case BOTTOM:
			textCursorY = baseLineY - PFWConstants::baseResolution.y;
		break;
	}

	sfString->SetText(t);
}

void GUITextfield::SetText (std::string text, bool resetCursorToEnd) {
	sfString->SetText(text);

	sfString->SetCenter(0, height / sfString->GetScale().y / 2);

	SetFakePos(CalcFakePos());

	if (textCursor != 0 && resetCursorToEnd) {
		iTextCursor = text.length();
		textCursor->SetPosition( sf::Vector2f(GetTextCursorX(), textCursorY) );
	}
}

sf::Vector2f GUITextfield::CalcFakePos () {
	sf::Vector2f fakePos;

	switch (mAlignH) {
		case LEFT:
		break;
			fakePos.x = basePos.x * windowFakeScale;
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

void GUITextfield::SetActive (bool active) {
	if (this->active == active)
		return;

	this->active = active;

	if (active) {
		if (txHover != "") SetImage(txHover);

		sfString->SetColor(colTextActive);

		textCursor = new ScreenSprite("Content/Textures/textcursor.png",
									mAlignH, mAlignV,
									layerDepth);
		drawManager->AddScreenSpace(textCursor);

		textCursor->SetScale( sf::Vector2f(
			baseScale.x / 2
				* (sfString->GetSize() / PFWConstants::baseFontSize)
					* PFWConstants::fontSizeScaleMultiplier,
			baseScale.y * 2
				* (sfString->GetSize() / PFWConstants::baseFontSize)
					* PFWConstants::fontSizeScaleMultiplier) );

		textCursor->SetFakeStuff(Game->GetWindowFakeScale());
	}
	else {
		if (onApply != 0) onApply();

		if (txNormal != "") SetImage(txNormal);

		sfString->SetColor(colTextInactive);

		drawManager->PopScreenSpace(textCursor);
		delete textCursor;
		textCursor = 0;
	}
}

void GUITextfield::SetFakeScale (sf::Vector2f fakeScale) {
	if (sfSprite) sfSprite->SetScale(fakeScale);
	sfString->SetScale(fakeScale);

	CalcHeight();
	if (textCursor != 0) CalcTextCursorPosY();
}

void GUITextfield::SetFakePos (sf::Vector2f fakePos) {
	if (sfSprite) sfSprite->SetPosition(fakePos);

	sf::Vector2f textOffset(mAlignH != LEFT ? sf::Vector2f(-GetScreenSize().x / 2, 0)
											: sf::Vector2f(0, 0));
//	switch (mAlignH) {
//		case LEFT:
//			textOffset = sf::Vector2f(0, 0);
//		break;
//		case CENTER:
//			textOffset = sf::Vector2f(-GetScreenSize().x / 2, 0);
//		break;
//		case RIGHT:
//			textOffset = sf::Vector2f(-GetScreenSize().x / 2, 0);
//		break;
//	}
	sfString->SetPosition(fakePos + .9f * textOffset);

	if (textCursor != 0) CalcTextCursorPosY();
}
