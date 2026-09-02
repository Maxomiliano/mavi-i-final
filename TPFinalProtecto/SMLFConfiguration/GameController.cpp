#include "GameController.h"
#include "Ship.h"
#include "EnemyShip.h"
#include "EnemyProjectile.h"
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
	if (!playerProjTex.loadFromFile("Assets/Images/playershipprojectile.png"))
	{
		cout << "Error loading player projectile" << endl;
	}
	if (!enemyTex.loadFromFile("Assets/Images/enemy3.png"))
	{
		cout << "Error loading enemy texture" << endl;
	}
	if (!enemyProjTex.loadFromFile("Assets/Images/enemyprojectile.png"))
	{
		cout << "Error loading enemy projectile" << endl;
	}

	/*
	for (int i = 0; i < 5; i++)
	{
		EnemyShip* newEnemy = new EnemyShip();
		newEnemy->setTexture(enemyTex);
		newEnemy->setOrigin(52.5f, 52.5f);
		//newEnemy->setScale(0.6f, 0.6f);

		newEnemy->Spawn(Vector2f(100.0f + (i * 100.0f), 100.0f));

		enemies.push_back(newEnemy);
	}
	*/

	// --- SISTEMA DE CARRILES (Formación Enemiga) ---
	int filas = 3;             // Cantidad de carriles verticales
	int enemigosPorFila = 5;   // Cuántas naves hay por carril

	for (int fila = 0; fila < filas; fila++)
	{
		for (int col = 0; col < enemigosPorFila; col++)
		{
			EnemyShip* newEnemy = new EnemyShip();
			newEnemy->setTexture(enemyTex);
			newEnemy->setOrigin(52.5f, 52.5f); // Ajusta al centro de tu imagen
			newEnemy->setScale(0.6f, 0.6f);

			// Calculamos la posición Y dependiendo del carril (empieza en 100, y baja 90px por fila)
			float posY = 100.0f + (fila * 90.0f);

			// Calculamos la posición X separando las naves por 120px
			float posX = 100.0f + (col * 120.0f);

			newEnemy->Spawn(Vector2f(posX, posY));

			// Magia de carriles: Si la fila es par (0, 2) van a la derecha. Si es impar (1), a la izquierda.
			if (fila % 2 == 0) {
				newEnemy->setDirection(1.0f);
			}
			else {
				newEnemy->setDirection(-1.0f);
			}

			enemies.push_back(newEnemy);
		}
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
			if (evt.type == Event::KeyPressed)
			{
				if (evt.key.code == Keyboard::Space)
				{
					Vector2f spawnPos = player.getPosition();
					spawnPos.y -= 40.0f;

					playerProjectiles.push_back(new PlayerProjectile(spawnPos, playerProjTex));
				}
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

		for (int i = 0; i < playerProjectiles.size(); i++)
		{
			playerProjectiles[i]->Update(time);

			if (playerProjectiles[i]->IsOutOfBounds())
			{
				delete playerProjectiles[i];
				playerProjectiles.erase(playerProjectiles.begin() + i);
				i--;
			}
		}
		for (auto enemy : enemies)
		{
			enemy->Update(time);
		}

		enemyShootTimer += time;

		if (enemyShootTimer >= enemyShootInterval && !enemies.empty())
		{
			enemyShootTimer = 0.0f;
			int randomIndex = rand() % enemies.size();
			Vector2f spawnPos = enemies[randomIndex]->getPosition();
			spawnPos.y += 40.0f;
			enemyProjectiles.push_back(new EnemyProjectile(spawnPos, enemyProjTex));
		}

		//Proyectiles jugador
		for (int i = 0; i < enemyProjectiles.size(); i++)
		{
			enemyProjectiles[i]->Update(time);
			if (enemyProjectiles[i]->IsOutOfBounds())
			{
				delete enemyProjectiles[i];
				enemyProjectiles.erase(enemyProjectiles.begin() + i);
				i--;
			}
		}

		for (int i = 0; i < playerProjectiles.size(); i++)
		{
			bool bulletDestroyed = false;

			for (int j = 0; j < enemies.size(); j++)
			{
				if (playerProjectiles[i]->getBounds().intersects(enemies[j]->getBounds()))
				{
					score += 100;

					delete enemies[j];
					enemies.erase(enemies.begin() + j);

					bulletDestroyed = true;
					break;
				}
			}
			if (bulletDestroyed)
			{
				delete playerProjectiles[i];
				playerProjectiles.erase(playerProjectiles.begin() + i);
				i--;
			}
		}

		//Proyectiles enemigos
		for (int i = 0; i < enemyProjectiles.size(); i++)
		{
			if (enemyProjectiles[i]->getBounds().intersects(player.getBounds()))
			{
				maxLives--;
				// Acá se actualizaría el hud?

				delete enemyProjectiles[i];
				enemyProjectiles.erase(enemyProjectiles.begin() + i);
				i--;

				if (maxLives <= 0)
				{
					state = State::GameOver;
				}
			}
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
	exitButton.setPosition(3 * 1024 / 4, 700);

	window.draw(mainMenuTitle);
	window.draw(gameInstructions);
	window.draw(playButton);
	window.draw(exitButton);
}

void GameController::RenderPlayScene()
{
	window.draw(bgSpr);

	for (auto proj : playerProjectiles)
	{
		proj->Draw(window);
	}

	for (auto enemy : enemies)
	{
		enemy->Draw(window);
	}

	for (auto enemyProj : enemyProjectiles)
	{
		enemyProj->Draw(window);
	}

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
