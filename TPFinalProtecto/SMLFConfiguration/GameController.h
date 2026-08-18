#pragma once
#include "EnemyShip.h"
#include "Ship.h"
#include "PlayerShip.h"
//#include "Projectile.h"
#include "PlayerProjectile.h"
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

enum class State
{
	MainMenu,
	Play,
	GameOver
};

class GameController
{
public:
	GameController();
	void Play();
	void ProcessEvents();
	void Update();
	void Render();
	void SpawnShips();
	void CheckCollisions();
	void RenderMainMenu();
	void RenderPlayScene();
	void RenderGameOver();
	void RestartGame();

private:
	Clock clock;
	Event evt;
	Mouse mouse;
	RenderWindow window;
	State state;
	PlayerShip player;
	vector<EnemyShip*> enemies;
	
	Texture bgTex;
	Texture playerTex;
	Texture playerProjTex;
	Texture enemyTex;
	Texture enemyProjTex;
	vector<Projectile*> enemyProjectiles;
	vector<Projectile*> playerProjectiles;

	Sprite bgSpr;
	Font font;
	Text playButton,
		exitButton,
		resultText,
		backToMenuButton,
		livesHud,
		enemiesDefeatedHud,
		scoreHud,
		nombreAlumno,
		mainMenuTitle,
		gameInstructions;

	Vector2f spawn;
	Vector2f lastPosition;

	int enemiesDefeated;
	int score;
	int maxLives = 3;
};

