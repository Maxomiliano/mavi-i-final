#include "ClockTest.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

ClockTest::ClockTest()
{
	Sprite cursorSprite;
	Texture cursorTex;

	cursorTex.loadFromFile("crosshair.png");
	cursorSprite.setTexture(cursorTex);
	//cursorSprite->setPosition(0, 0);

	Clock clock;
	RenderWindow window(VideoMode(800, 600, 32), "clock");
	Event evt;
	int x = 0;
	window.setMouseCursorVisible(false);


	while (window.isOpen())
	{
		while (window.pollEvent(evt))
		{
			if (evt.type == Event::Closed)
			{
				window.close();
			}
		}
		Time Tf = clock.restart();
		x = x + 1 * Tf.asSeconds();
		cursorSprite.setPosition(x, x);
		window.clear(Color::White);
		window.draw(cursorSprite);
		window.display();
	}
}