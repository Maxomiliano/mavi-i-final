#include "GameController.h"
#include "Ship.h"
#include "EnemyShip.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

GameController::GameController() :

	window(VideoMode(768, 1024, 32), "TPIntegrador"), state(State::MainMenu), maxLives(3), score(0), enemiesDefeated(0)
{
	srand(time(NULL));
	if (!font.loadFromFile("Assets/Fonts/Cowboys.otf"))
	{
		cout << "Error loading font" << endl;
	}
	if (!playerTex.loadFromFile("Assets/Images/spaceshipw.png"))
	{
		cout << "Error loading player ship texture" << endl;
	}
	player.setTexture(playerTex);
	player.setOrigin(52.5f, 52.5f);
	player.setScale(0.5f, 0.5f);
	player.Spawn(Vector2f(768.0f / 2.0f, 920.0f));
	bgTex.loadFromFile("Assets/Images/spacebg.jpg");
	bgSpr.setTexture(bgTex);
}

	void GameController::Play()
	{
		while (window.isOpen())
		{
			ProcessEvents();
			Update();
			Render();
		}
	}

	void GameController::ProcessEvents()
	{
		Vector2f mousePos(evt.mouseButton.x, evt.mouseButton.y);
		while (window.pollEvent(evt))
		{
			if (evt.type == Event::Closed)
			{
				window.close();
			}
			switch (state)
			{
			case State::MainMenu:
				if (evt.type == Event::MouseButtonPressed)
				{
					if (playButton.getGlobalBounds().contains(mousePos))
					{
						state = State::Play;
						RestartGame();
					}
					else if (exitButton.getGlobalBounds().contains(mousePos))
					{
						window.close();
					}
				}
				break;
			case State::Play:
				if (evt.type == Event::MouseMoved)
				{

				}
				else if (evt.type == Event::MouseButtonPressed)
				{
					
				}
				break;
			case State::GameOver:
				if (evt.type == Event::MouseButtonPressed)
				{
					if (backToMenuButton.getGlobalBounds().contains(mousePos))
					{
						state = State::MainMenu;
					}
				}
				break;
			}
		}
	}

	void GameController::Update()
	{
		if (state == State::Play)
		{
			float time = clock.restart().asSeconds();
			//SpawnShips();
			player.Update(time);
			
		}
		/*
		if ()
		{
			state = State::GameOver;
			resultText.setString("Victory!");
		}
		else if ()
		{
			state = State::GameOver;
			resultText.setString("Defeat!");
		}
		*/
	}


	void GameController::Render()
	{
		window.clear();

		if (state == State::MainMenu)
			RenderMainMenu();
		else if (state == State::Play)
			RenderPlayScene();
		else if (state == State::GameOver)
			RenderGameOver();

		window.display();
	}

	void GameController::RenderMainMenu()
	{
		mainMenuTitle.setFont(font);
		mainMenuTitle.setString("TPIntegrador");
		mainMenuTitle.setCharacterSize(60);
		FloatRect titleBounds = mainMenuTitle.getGlobalBounds();
		mainMenuTitle.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
		mainMenuTitle.setPosition(1024 / 2, 100);

		gameInstructions.setFont(font);
		gameInstructions.setString("Instrucciones:\n\n"
			"- Se apunta y dispara con el puntero del raton.\n"
			"- Cada disparo acertado al enemigo suma 1 punto.\n"
			"- Cada disparo a un inocente resta 1 vida.\n"
			"- Si el enemigo dispara resta 1 vida.\n"
			"- Se gana matando 10 enemigos.\n"
			"- Se pierde si gastamos 3 vidas.");
		gameInstructions.setCharacterSize(24);
		FloatRect instructionsBounds = gameInstructions.getGlobalBounds();
		gameInstructions.setOrigin(instructionsBounds.width / 2, instructionsBounds.height / 2);
		gameInstructions.setPosition(1024 / 2, 400);


		playButton.setFont(font);
		playButton.setString("Play");
		playButton.setCharacterSize(30);
		FloatRect playBounds = playButton.getGlobalBounds();
		playButton.setOrigin(playBounds.width / 2, playBounds.height / 2);
		playButton.setPosition(1024 / 4, 700);


		exitButton.setFont(font);
		exitButton.setString("Exit");
		exitButton.setCharacterSize(30);
		FloatRect exitBounds = exitButton.getGlobalBounds();
		exitButton.setOrigin(exitBounds.width / 2, exitBounds.height / 2);
		exitButton.setPosition(3 * 1024 / 4, 700);

		window.draw(mainMenuTitle);
		window.draw(gameInstructions);
		window.draw(playButton);
		window.draw(exitButton);
	}

	void GameController::RenderPlayScene()
	{
		window.draw(bgSpr);

		/*
		nombreAlumno.setFont(font);
		nombreAlumno.setCharacterSize(30);
		nombreAlumno.setString("TP Integral. Garcia, Maximiliano.");
		FloatRect nombreBounds = nombreAlumno.getLocalBounds();
		nombreAlumno.setOrigin(nombreBounds.width / 2, nombreBounds.height / 2);
		nombreAlumno.setPosition(1024 / 2, 750);
		*/

		livesHud.setFont(font);
		livesHud.setCharacterSize(30);
		livesHud.setString("Lives: " + to_string(maxLives));
		FloatRect livesBounds = livesHud.getLocalBounds();
		livesHud.setOrigin(livesBounds.width / 2, livesBounds.height / 2);
		livesHud.setPosition(100, 40);

		enemiesDefeatedHud.setFont(font);
		enemiesDefeatedHud.setCharacterSize(30);
		enemiesDefeatedHud.setString("Enemies killed: " + to_string(enemiesDefeated));
		FloatRect enemiesBounds = enemiesDefeatedHud.getLocalBounds();
		enemiesDefeatedHud.setOrigin(enemiesBounds.width / 2, enemiesBounds.height / 2);
		enemiesDefeatedHud.setPosition(768 / 2, 80);

		scoreHud.setFont(font);
		scoreHud.setCharacterSize(30);
		scoreHud.setString("Score " + to_string(enemiesDefeated));
		FloatRect scoreBounds = scoreHud.getLocalBounds();
		scoreHud.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
		scoreHud.setPosition(668, 40);

		player.Draw(window);

		window.draw(nombreAlumno);
		window.draw(livesHud);
		window.draw(enemiesDefeatedHud);
		window.draw(scoreHud);
	}

	void GameController::RenderGameOver()
	{
		resultText.setFont(font);
		resultText.setCharacterSize(60);
		FloatRect resulBounds = resultText.getGlobalBounds();
		resultText.setOrigin(resulBounds.width / 2, resulBounds.height / 2);
		resultText.setPosition(1024 / 2, 100);

		backToMenuButton.setFont(font);
		backToMenuButton.setString("Back to Menu");
		backToMenuButton.setCharacterSize(30);
		FloatRect backToMenuBounds = backToMenuButton.getGlobalBounds();
		backToMenuButton.setOrigin(backToMenuBounds.width / 2, backToMenuBounds.height / 2);
		backToMenuButton.setPosition(1024 / 2, 700);

		window.draw(resultText);
		window.draw(backToMenuButton);
	}

	void GameController::SpawnShips()
	{
		
	}
	void GameController::CheckCollisions()
	{
		
	}


	void GameController::RestartGame()
	{
		enemiesDefeated = 0;
		maxLives = 3;
		score = 0;
	}
