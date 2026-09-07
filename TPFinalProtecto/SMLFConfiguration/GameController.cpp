#include "GameController.h"
#include "Ship.h"
#include "EnemyShip.h"
#include "TankEnemy.h"
#include "FastEnemyShip.h"
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
	if (!font.loadFromFile("Assets/Fonts/Audiowide.ttf"))
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
	if (!enemyTexTank.loadFromFile("Assets/Images/enemy1.png"))
	{
		cout << "Error loading enemy texture" << endl;
	}
	if (!enemyTexFast.loadFromFile("Assets/Images/enemy2.png"))
	{
		cout << "Error loading enemy texture" << endl;
	}
	if (!enemyProjTex.loadFromFile("Assets/Images/enemyprojectile.png"))
	{
		cout << "Error loading enemy projectile" << endl;
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
					if (playerShootTimer >= playerShootCooldown)
					{
						Vector2f spawnPos = player.getPosition();
						spawnPos.y -= 40.0f;

						playerProjectiles.push_back(new PlayerProjectile(spawnPos, playerProjTex));
						playerShootTimer = 0.0f;
					}
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
		case State::Win:
			if (evt.type == Event::KeyPressed && evt.key.code == Keyboard::Enter)
			{
				//Alguna funcion de Reset
				state = State::Win;
			}
		}

	}
}

void GameController::Update()
{
	if (state == State::Play)
	{
		float time = clock.restart().asSeconds();

		playerShootTimer += time;

		player.Update(time);

		for (auto enemy : enemies)
		{
			enemy->Update(time);
		}

		//Proyectiles jugador
		UpdatePlayerProjectiles(time);

		//Proyectiles enemigos
		UpdateEnemyProjectiles(time);

		CheckCollisions();
		/*
		if (enemies.empty())
		{
			state = State::Win;
		}
		*/

		scoreHud.setString("Score: " + to_string(score));
		livesHud.setString("Lives: " + to_string(maxLives));
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
	//else if (state == State::Win)
		//RenderWinScene();

	window.display();
}

void GameController::RenderMainMenu()
{
	mainMenuTitle.setFont(font);
	mainMenuTitle.setString("TPFinal");
	mainMenuTitle.setCharacterSize(60);
	FloatRect titleBounds = mainMenuTitle.getGlobalBounds();
	mainMenuTitle.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
	mainMenuTitle.setPosition(768 / 2, 100);

	gameInstructions.setFont(font);
	gameInstructions.setString("Instrucciones:\n\n"
		"- Te mueves con las teclas A y D o con las flechitas.\n"
		"- Disparas con la barra espaciadora.\n"
		"- Empiezas el juego con 3 vidas.\n"
		"- Si el enemigo te pega, se resta 1 vida.\n"
		"- El nivel se completa al destruir a todos los enemigos.\n"
		"- El nivel se pierde si pierdes tus 3 vidas.");
	gameInstructions.setCharacterSize(24);
	FloatRect instructionsBounds = gameInstructions.getGlobalBounds();
	gameInstructions.setOrigin(instructionsBounds.width / 2, instructionsBounds.height / 2);
	gameInstructions.setPosition(768 / 2, 400);


	playButton.setFont(font);
	playButton.setString("Play");
	playButton.setCharacterSize(30);
	FloatRect playBounds = playButton.getGlobalBounds();
	playButton.setOrigin(playBounds.width / 2, playBounds.height / 2);
	playButton.setPosition(768 / 4, 700);


	exitButton.setFont(font);
	exitButton.setString("Exit");
	exitButton.setCharacterSize(30);
	FloatRect exitBounds = exitButton.getGlobalBounds();
	exitButton.setOrigin(exitBounds.width / 2, exitBounds.height / 2);
	exitButton.setPosition(3 * 768 / 4, 700);

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
	enemiesDefeatedHud.setPosition(768 / 2, 40);

	scoreHud.setFont(font);
	scoreHud.setCharacterSize(30);
	scoreHud.setString("Score " + to_string(score));
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
	gameOverTitle.setFont(font);
	gameOverTitle.setString("GAME OVER");
	gameOverTitle.setCharacterSize(60);
	FloatRect titleBounds = gameOverTitle.getGlobalBounds();
	gameOverTitle.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
	gameOverTitle.setPosition(768 / 2, 100);

	resultText.setFont(font);
	resultText.setCharacterSize(60);
	FloatRect resulBounds = resultText.getGlobalBounds();
	resultText.setOrigin(resulBounds.width / 2, resulBounds.height / 2);
	resultText.setPosition(768 / 2, 100);

	backToMenuButton.setFont(font);
	backToMenuButton.setString("Back to Menu");
	backToMenuButton.setCharacterSize(30);
	FloatRect backToMenuBounds = backToMenuButton.getGlobalBounds();
	backToMenuButton.setOrigin(backToMenuBounds.width / 2, backToMenuBounds.height / 2);
	backToMenuButton.setPosition(768 / 2, 700);

	window.draw(resultText);
	window.draw(gameOverTitle);
	window.draw(backToMenuButton);
}

void GameController::RenderWinScene()
{

}

void GameController::SpawnShips()
{
	int filas = 3;
	int enemigosPorFila = 5;

	for (int fila = 0; fila < filas; fila++)
	{
		for (int col = 0; col < enemigosPorFila; col++)
		{
			EnemyShip* newEnemy = nullptr;

			if (fila == 0)
			{
				newEnemy = new TankEnemy();
				newEnemy->setTexture(enemyTexTank);
			}
			else if (fila == 1)
			{
				newEnemy = new EnemyShip();
				newEnemy->setTexture(enemyTex);
			}
			else
			{
				newEnemy = new FastEnemyShip();
				newEnemy->setTexture(enemyTexFast);
			}
			//newEnemy->setTexture(enemyTex);
			newEnemy->setOrigin(52.5f, 52.5f);
			newEnemy->setScale(0.6f, 0.6f);

			float posY = 100.0f + (fila * 90.0f);
			float posX = 100.0f + (col * 120.0f);

			newEnemy->Spawn(Vector2f(posX, posY));

			if (fila % 2 == 0) {
				newEnemy->setDirection(1.0f);
			}
			else {
				newEnemy->setDirection(-1.0f);
			}

			enemies.push_back(newEnemy);
		}
	}
}
void GameController::CheckCollisions()
{
	//Colisiones de los proyectiles del jugador.
	for (int i = 0; i < playerProjectiles.size(); i++)
	{
		bool bulletDestroyed = false;

		for (int j = 0; j < enemies.size(); j++)
		{
			if (playerProjectiles[i]->getBounds().intersects(enemies[j]->getBounds()))
			{
				enemies[j]->TakeDamage(1);

				if (enemies[j]->IsDead())
				{
					score += 100;

					if (dynamic_cast<FastEnemyShip*>(enemies[j]) != nullptr)
					{
						playerShootCooldown = 0.15f;
					}

					delete enemies[j];
					enemies.erase(enemies.begin() + j);
				}

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

	//Colisiones de los proyectiles de los enemigos
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

void GameController::UpdatePlayerProjectiles(float deltaTime)
{
	for (int i = 0; i < playerProjectiles.size(); i++)
	{
		playerProjectiles[i]->Update(deltaTime);

		if (playerProjectiles[i]->IsOutOfBounds())
		{
			delete playerProjectiles[i];
			playerProjectiles.erase(playerProjectiles.begin() + i);
			i--;
		}
	}
}

void GameController::UpdateEnemyProjectiles(float deltaTime)
{
	for (auto enemy : enemies)
	{
		if (enemy->CanShoot(deltaTime))
		{
			Vector2f spawnPos = enemy->getPosition();
			spawnPos.y += 40.0f;
			enemyProjectiles.push_back(new EnemyProjectile(spawnPos, enemyProjTex));
		}
	}

	/*
	enemyShootTimer += deltaTime;
	 

	if (enemyShootTimer >= enemyShootInterval && !enemies.empty())
	{
		enemyShootTimer = 0.0f;
		int randomIndex = rand() % enemies.size();
		Vector2f spawnPos = enemies[randomIndex]->getPosition();
		spawnPos.y += 40.0f;
		enemyProjectiles.push_back(new EnemyProjectile(spawnPos, enemyProjTex));
	}
	*/

	for (int i = 0; i < enemyProjectiles.size(); i++)
	{
		enemyProjectiles[i]->Update(deltaTime);
		if (enemyProjectiles[i]->IsOutOfBounds())
		{
			delete enemyProjectiles[i];
			enemyProjectiles.erase(enemyProjectiles.begin() + i);
			i--;
		}
	}
}


void GameController::RestartGame()
{
	enemiesDefeated = 0;
	maxLives = 3;
	score = 0;
	enemyShootTimer = 0.0f;
	playerShootTimer = 0.0f;

	for (auto proj : playerProjectiles)
	{
		delete proj;
	}
	playerProjectiles.clear();

	for (auto proj : enemyProjectiles)
	{
		delete proj;
	}
	enemyProjectiles.clear();

	for (auto enemy : enemies) delete enemy;
	{
		enemies.clear();
	}

	SpawnShips();

	player.Spawn(Vector2f(768.0f / 2.0f, 900.0f));
}
