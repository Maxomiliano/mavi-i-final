#pragma once
#include "Cross.h"
#include "Enemy.h"
#include "Innocent.h"
#include "Character.h"
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
	void SpawnCharacters();
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
	Cross cross;
	Enemy enemy;
	Innocent innocent;
	Texture crossTex;
	Texture enemyTex;
	Texture innocentTex;
	Texture bgTex;
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

	Character* spawnedChar = NULL;
	vector<Vector2f> spawnPositions;
	Vector2f spawn;
	Vector2f lastPosition;

	int enemiesDefeated;
	int score;
	int maxLives = 3;
	int innocentsShooted;
	float actionTime = 0.0f;
	float actionDelay = 1.5f;
};

