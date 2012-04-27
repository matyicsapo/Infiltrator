#include "Shape.hpp"

void Shape::SetFakeScale (sf::Vector2f fakeScale) {
	sfShape->SetScale(fakeScale);
}

void Shape::SetFakePos (sf::Vector2f fakePos) {
	sfShape->SetPosition(fakePos);
}

Shape::Shape (sf::Shape* s, int layerDepth) : DrawableBase(layerDepth), sfShape(s) {}

Shape::Shape (Shape const& shape) : DrawableBase(shape) {
	sfShape = new sf::Shape( *(shape.sfShape) );
}

Shape::~Shape () {
	delete sfShape;
}

void Shape::ReleaseResources () {}

void Shape::Draw (sf::RenderWindow& rwin) {
	rwin.Draw(*sfShape);
}

void Shape::SetColor (sf::Color color) {
	sfShape->SetColor(color);
}
sf::Color Shape::GetColor () {
	return sfShape->GetColor();
}

void Shape::SetRotation (float rotation) {
	sfShape->SetRotation(rotation);
}
float Shape::GetRotation () {
	return sfShape->GetRotation();
}
void Shape::Rotate (float angle) {
	sfShape->Rotate(angle);
}

void Shape::AddPoint (const sf::Vector2f& Position, const sf::Color& Col, const sf::Color& OutlineCol) {
	sfShape->AddPoint(Position, Col, OutlineCol);
}
unsigned int Shape::GetNbPoints () const {
	return sfShape->GetNbPoints();
}
void Shape::EnableFill (bool enable) {
	sfShape->EnableFill(enable);
}
void Shape::EnableOutline (bool enable) {
	sfShape->EnableOutline(enable);
}
void Shape::SetPointPosition (unsigned int Index, const sf::Vector2f& Position) {
	sfShape->SetPointPosition(Index, Position);
}
void Shape::SetOutlineWidth (float Width) {
	sfShape->SetOutlineWidth(Width);
}
sf::Vector2f Shape::GetPointPosition (unsigned int Index) const {
	return sfShape->GetPointPosition(Index);
}
sf::Vector2f Shape::GetPointPositionTransformed (unsigned int Index) const {
	return sfShape->TransformToGlobal(sfShape->GetPointPosition(Index)) / windowFakeScale;
}
sf::Vector2f Shape::GetPointPositionLocal (unsigned int Index) const {
	return sfShape->GetPointPosition(Index) / windowFakeScale;
}
void Shape::SetCenter (const sf::Vector2f& Center) {
	sfShape->SetCenter(Center);
}

sf::Vector2f Shape::TransformToLocal (const sf::Vector2f& Point) const {
	return sfShape->TransformToLocal(Point) / windowFakeScale;
}
sf::Vector2f Shape::TransformToGlobal (const sf::Vector2f& Point) const {
	return sfShape->TransformToGlobal(Point) / windowFakeScale;
}

sf::Vector2f Shape::GetScreenSize () {
	return sf::Vector2f(0, 0);
}
bool Shape::Contains (sf::Vector2f worldPosition) {
	return false;
}
bool Shape::IsOpaque (sf::Vector2f worldPosition) {
	return false;
}
