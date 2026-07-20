#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;
class Character
{
public: Character();
	  enum class CharacterType
	  {
		  Enemy,
		  Innocent
	  };
	  virtual ~Character();
	  virtual void Spawn(Vector2f& position);
	  virtual bool IsClicked(Vector2f crossPosition);
	  virtual void Draw(RenderWindow& window);

	  bool IsVisible() const;
	  void setTexture(const Texture& texture);
	  void setScale(float x, float y);
	  void setOrigin(float x, float y);
	  void setCharacterType(CharacterType charType) { type = charType; }
	  CharacterType getCharacterType() { return type; }
protected:
	CharacterType type;
	Sprite charSpr;
	bool isVisible;
};