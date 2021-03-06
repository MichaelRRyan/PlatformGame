// @Author Michael Rainsford Ryan
#ifndef GAME
#define GAME
const double PI{ 3.14159265358979323846 };

#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();

	// main method for game
	void run();

private:
	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();

	void setupTexturesAndSprites();
	
	float vectorLength(sf::Vector2f t_vector);
	float signOf(float t_value);

	bool isColliding(sf::RectangleShape t_subjectOne, sf::Vector2f t_position);

	sf::RenderWindow m_window;
	bool m_exitGame;
	bool m_onGround{ false };
	bool m_hitGround{ false };

	sf::Vector2f m_playerVelocity{ 0.0f, 0.0f };
	sf::Vector2f m_playerAcceleration{ 0.0f, 0.0f };

	float m_playerMovementSpeed{ 6.0f };
	float m_gravity{ 0.4f };
	float m_jumpSpeed{ 18.0f };

	int m_trailNum = 0;
	
	const static int M_NUM_OF_PLATFORMS{ 20 };
	const static int M_NUM_OF_TRAILS{ 20 };

	sf::Vector2f platformPositions[M_NUM_OF_PLATFORMS] = { sf::Vector2f{ 400.0f, 560.0f }, sf::Vector2f{ 600.0f, 560.0f }, sf::Vector2f{ 700.0f, 520.0f }, sf::Vector2f{ 250.0f, 350.0f }, sf::Vector2f{ 600.0f, 100.0f }, sf::Vector2f{ 750.0f, 70.0f } };

	sf::RectangleShape m_platforms[M_NUM_OF_PLATFORMS];
	sf::RectangleShape m_trailEffect[M_NUM_OF_TRAILS];
	sf::RectangleShape m_player;
	sf::RectangleShape m_platform;
};

#endif