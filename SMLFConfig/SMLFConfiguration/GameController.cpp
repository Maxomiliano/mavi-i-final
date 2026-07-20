#include "GameController.h"
#include "Cross.h"
#include "Character.h"
#include "Enemy.h"
#include "Innocent.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

GameController::GameController() :

	window(VideoMode(1024, 900, 32), "TPIntegrador"), state(State::MainMenu), maxLives(3), score(0), enemiesDefeated(0), innocentsShooted(0),
	spawnPositions{ {216.0f, 188.0f}, {800.0f, 188.0f}, {194.0f, 510.0f}, {850.0f, 510.0f} }
{
	srand(time(NULL));
	if (!font.loadFromFile("Cowboys.otf"))
	{
		cout << "Error loading font" << endl;
	}
	crossTex.loadFromFile("crosshair.png");
	enemyTex.loadFromFile("enemy.png");
	innocentTex.loadFromFile("innocent.jpg");
	bgTex.loadFromFile("background.png");

	cross.setTexture(crossTex);
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
				cross.UpdatePosition(Vector2f(evt.mouseMove.x, evt.mouseMove.y));
			}
			else if (evt.type == Event::MouseButtonPressed)
			{
				CheckCollisions();
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
		actionTime += time;
		if (spawnedChar != NULL)
		{
			if (actionTime >= actionDelay)
			{
				if (spawnedChar->getCharacterType() == Character::CharacterType::Enemy)
				{
					maxLives--;
				}
				else if (spawnedChar->getCharacterType() == Character::CharacterType::Innocent)
				{
					delete spawnedChar;
					spawnedChar = NULL;
				}
				if (spawnedChar != NULL)
					actionTime = 0.0f;
			}
		}
		else
		{
			SpawnCharacters();
		}
		if (enemiesDefeated >= 10)
		{
			state = State::GameOver;
			resultText.setString("Victory!");
		}
		else if (maxLives <= 0)
		{
			state = State::GameOver;
			resultText.setString("Defeat!");
		}
	}
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
	exitButton.setPosition(3 * 1024 /4, 700);

	window.draw(mainMenuTitle);
	window.draw(gameInstructions);
	window.draw(playButton);
	window.draw(exitButton);
}

void GameController::RenderPlayScene()
{
	window.draw(bgSpr);
	if (spawnedChar != NULL)
	{
		spawnedChar->Draw(window);
	}
	cross.Draw(window);

	nombreAlumno.setFont(font);
	nombreAlumno.setCharacterSize(30);
	nombreAlumno.setString("TP Integral. Garcia, Maximiliano.");
	FloatRect nombreBounds = nombreAlumno.getLocalBounds();
	nombreAlumno.setOrigin(nombreBounds.width / 2, nombreBounds.height / 2);
	nombreAlumno.setPosition(1024 / 2, 750);

	livesHud.setFont(font);
	livesHud.setCharacterSize(30);
	livesHud.setString("Lives: " + to_string(maxLives));
	FloatRect livesBounds = livesHud.getLocalBounds();
	livesHud.setOrigin(livesBounds.width / 2, livesBounds.height / 2);
	livesHud.setPosition(100, 800);

	enemiesDefeatedHud.setFont(font);
	enemiesDefeatedHud.setCharacterSize(30);
	enemiesDefeatedHud.setString("Enemies killed: " + to_string(enemiesDefeated));
	FloatRect enemiesBounds = enemiesDefeatedHud.getLocalBounds();
	enemiesDefeatedHud.setOrigin(enemiesBounds.width / 2, enemiesBounds.height / 2);
	enemiesDefeatedHud.setPosition(180, 850);

	scoreHud.setFont(font);
	scoreHud.setCharacterSize(30);
	scoreHud.setString("Score " + to_string(enemiesDefeated - (innocentsShooted * 2)));
	FloatRect scoreBounds = scoreHud.getLocalBounds();
	scoreHud.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
	scoreHud.setPosition(850, 850);

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

void GameController::SpawnCharacters()
{
	if (spawnedChar == NULL)
	{
		do
		{
			int randomIndex = rand() % spawnPositions.size();
			spawn = spawnPositions[randomIndex];
		} while (spawn == lastPosition);

		lastPosition = spawn;

		if (rand() % 2 == 0)
		{
			spawnedChar = new Enemy();
			spawnedChar->setTexture(enemyTex);
		}
		else
		{
			spawnedChar = new Innocent;
			spawnedChar->setTexture(innocentTex);
		}
		spawnedChar->setOrigin(162 / 2, 196 / 2);
		spawnedChar->Spawn(spawn);
		actionTime = 0.0f;
	}
}
void GameController::CheckCollisions()
{
	if (spawnedChar != NULL && spawnedChar->IsClicked(Vector2f(evt.mouseButton.x, evt.mouseButton.y)))
	{
		if (spawnedChar->getCharacterType() == Character::CharacterType::Enemy)
		{
			enemiesDefeated++;
		}
		else if (spawnedChar->getCharacterType() == Character::CharacterType::Innocent)
		{
			maxLives--;
			innocentsShooted++;
		}

		delete spawnedChar;
		spawnedChar = NULL;
	}
}


void GameController::RestartGame()
{
	enemiesDefeated = 0;
	maxLives = 3;
	score = 0;
}
