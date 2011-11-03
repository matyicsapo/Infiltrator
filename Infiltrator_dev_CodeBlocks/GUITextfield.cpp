#include "GUITextfield.hpp"





				#include <iostream>






void GUITextfield::HandleEvents (std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {

		switch (itSfEvent->Type) {
			case sf::Event::MouseButtonPressed: {
				sf::Vector2f screenSize(GetScreenSize());
				sf::Rect<float> btnBounds(CalcFakePos().x - (screenSize.x / 2),
											CalcFakePos().y - (screenSize.y / 2 ),
											CalcFakePos().x + (screenSize.x / 2),
											CalcFakePos().y + (screenSize.y / 2 ) );

				if (btnBounds.Contains(itSfEvent->MouseButton.X, itSfEvent->MouseButton.Y)) {
					if (!active) {
						active = true;

						if (textCursor == 0) {
							textCursor = new ScreenSprite("Content/Textures/textcursor.png", mAlignH, mAlignV, layerDepth);
							textCursor->SetColor(sf::Color::Cyan);

							//textCursor->SetScale(baseScale);
							textCursor->SetScale(sf::Vector2f(.5, 1));
						}

						DrawManager::Instance()->AddScreenSpace(textCursor);
					}

					for (unsigned int itChar = 0; itChar < std::string(sfString->GetText()).length(); itChar++) {
						//itSfEvent->MouseButton.X

						//sf::Vector2f(sfString->TransformToGlobal(sfString->GetCharacterPos(itChar))).x

						//*******************************************************************************************************************************************

						/**/
						if ( itSfEvent->MouseButton.X < sf::Vector2f(sfString->TransformToGlobal(sfString->GetCharacterPos(itChar))).x ) {
							//if (itChar == std::string(sfString->GetText()).length() - 1)
								//textCursor->SetPosition( sf::Vector2f(sfString->TransformToGlobal(sfString->GetCharacterPos(itChar)).x, basePos.y) );
							//else {
								iTextCursor = itChar - 1;
								textCursor->SetPosition( sf::Vector2f(sfString->TransformToGlobal(sfString->GetCharacterPos(iTextCursor)).x, basePos.y) );
							//}

							break;

						}
						else if (itChar == std::string(sfString->GetText()).length() - 1) {
							iTextCursor = itChar;
							textCursor->SetPosition( sf::Vector2f(sfString->TransformToGlobal(sfString->GetCharacterPos(iTextCursor)).x, basePos.y) );
						}
						/**/
					}

					//textCursord->SetPosition(sf::Vector2f(itSfEvent->MouseButton.X, basePos.y));
				}
				else if (active) {
					active = false;

					DrawManager::Instance()->PopScreenSpace(textCursor);
				}
			} break;
			case sf::Event::TextEntered: {
				if (active) {
					if ( itSfEvent->Text.Unicode == 8) {
						//std::cout << "backspace" << std::endl;

						/*std::string text = sfString->GetText();
						text.erase(--iTextCursor, 1);
						sfString->SetText(text);*/

						// don't delete when on from 0th character - exception ***********************************

						sfString->SetText( std::string(sfString->GetText()).erase(--iTextCursor, 1) );
					}
					else {
						//std::cout << (char)(itSfEvent->Text.Unicode) << std::endl;

						/*std::string text = sfString->GetText();
						char entered = (char)(itSfEvent->Text.Unicode);
						text.insert(iTextCursor, 1, entered);
						sfString->SetText(text);
						iTextCursor++;
						textCursor->SetPosition( sf::Vector2f(sfString->TransformToGlobal(sfString->GetCharacterPos(iTextCursor)).x, basePos.y) );*/

						sfString->SetText( std::string(sfString->GetText()).insert(iTextCursor++, 1, char(itSfEvent->Text.Unicode)) );
					}

					textCursor->SetPosition( sf::Vector2f(sfString->TransformToGlobal(sfString->GetCharacterPos(iTextCursor)).x, basePos.y) );
				}
			} break;
			default:
				// don't care about no other event types
			break;
		}

		itSfEvent++;
	}
}
