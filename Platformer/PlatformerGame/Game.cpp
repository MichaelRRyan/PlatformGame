// A platformer test game
// @Author Michael Rainsford Ryan
// @Date 19/11/2018
// 18:23

#include <iostream>
#include <stdlib.h> 
#include <time.h> 

#include "Game.h"

Game::Game() :
	m_window{ sf::VideoMode{ 800, 600, 32 }, "Infectious" },
	m_exitGame{ false } //when true game will exit
{
	setupTexturesAndSprites(); // load font & set text properties	
}


Game::~Game()
{
}



// Runs the game at 60 frames per seconds
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float FPS = 60.0f;
	sf::Time timePerFrame = sf::seconds(1.0f / FPS); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // Run as many times as possible
		timeSinceLastUpdate += clock.restart();
		if (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // Run at a minimum of 60 fps
			update(timePerFrame); // 60 fps
		}
		render(); // Run as many times as possible
	}
}



void Game::processEvents()
{

	sf::Event nextEvent;
	while (m_window.pollEvent(nextEvent))
	{
		if (sf::Event::Closed == nextEvent.type) // check if the close window button is clicked on.
		{
			m_window.close();
		}
	}
}



void Game::render()
{
	m_window.clear();

	// Draw all the platforms
	/*for (int i = 0; i < M_NUM_OF_PLATFORMS; i++)
	{
		m_window.draw(m_platforms[i]);
	}*/

	// Draw all the platforms
	for (int i = 0; i < M_NUM_OF_TRAILS; i++)
	{
		m_window.draw(m_trailEffect[i]);
	}

	m_window.draw(m_platform);
	m_window.draw(m_player);
	m_window.display();
}

void Game::setupTexturesAndSprites()
{
	// Set the random seed
	srand(static_cast<unsigned>(time(nullptr)));

	sf::Texture platformTexture;
	if(!platformTexture.loadFromFile("platform.png")) { /* Error... */ }

	// Setup player
	m_player.setSize(sf::Vector2f(20, 20));
	m_player.setOrigin(m_player.getGlobalBounds().width / 2, m_player.getGlobalBounds().height / 2);
	m_player.setPosition(sf::Vector2f(400, 300));

	// Setup platforms
	for (int i = 0; i < M_NUM_OF_PLATFORMS; i++)
	{
		//m_platforms[i].setTexture(platformTexture);
		//m_platforms[i].setScale(0.3, 0.3);
		m_platforms[i].setSize(sf::Vector2f(80.0f, 20.0f));
		m_platforms[i].setPosition(rand() % 750, rand() % 500 + 50);
		m_platforms[i].setOrigin(m_platforms[i].getGlobalBounds().width / 2, m_platforms[i].getGlobalBounds().height / 2);
	}

	// Setup trail effect
	for (int i = 0; i < M_NUM_OF_TRAILS; i++)
	{
		m_trailEffect[i].setSize(sf::Vector2f(10.0f, 10.0f));
		m_trailEffect[i].setPosition(rand() % 750, rand() % 500 + 50);
		m_trailEffect[i].setOrigin(m_trailEffect[i].getGlobalBounds().width / 2, m_trailEffect[i].getGlobalBounds().height / 2);
		m_trailEffect[i].setFillColor(sf::Color(255, 35, 68));
	}

	

	// Setup single platform
	m_platform.setSize(sf::Vector2f{ 400.0f, 30.0f });
	m_platform.setPosition(400.0f, 560.0f);
	m_platform.setOrigin(m_platform.getGlobalBounds().width / 2, m_platform.getGlobalBounds().height / 2);
}



void Game::update(sf::Time t_deltaTime)
{
	// Get horisontal input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_playerVelocity.x = -m_playerMovementSpeed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_playerVelocity.x = m_playerMovementSpeed;
	}
	else
	{
		m_playerVelocity.x = 0.0f;
	}

	// Check if on the ground
	if (isColliding(m_player, m_platform, sf::Vector2f{ m_player.getPosition().x, m_player.getPosition().y + 1.0f }))
	{
		// Jump
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			m_playerVelocity.y = -m_jumpSpeed;
		}

		m_playerAcceleration.y = 0;
	}
	else
	{
		// Add gravity
		m_playerAcceleration.y = m_gravity;
	}

	m_playerVelocity += m_playerAcceleration;

	// Check for horisontal collisions
	if (isColliding(m_player, m_platform, sf::Vector2f{ m_player.getPosition().x + m_playerVelocity.x, m_player.getPosition().y }))
	{
		while (!isColliding(m_player, m_platform, sf::Vector2f{ m_player.getPosition().x + signOf(m_playerVelocity.x), m_player.getPosition().y }))
		{
			m_player.move(signOf(m_playerVelocity.x), 0.0f);
		}
		m_playerVelocity.x = 0.0f;
	}
	else
	{
		m_player.move(m_playerVelocity.x, 0.0f);
	}

	// Check for vertical collisions
	if (isColliding(m_player, m_platform, sf::Vector2f{ m_player.getPosition().x, m_player.getPosition().y + m_playerVelocity.y }))
	{
		while (!isColliding(m_player, m_platform, sf::Vector2f{ m_player.getPosition().x, m_player.getPosition().y + signOf(m_playerVelocity.y) }))
		{
			m_player.move(0.0f, signOf(m_playerVelocity.y));
		}
		m_playerVelocity.y = 0.0f;
	}
	else
	{
		m_player.move(0.0f, m_playerVelocity.y);
	}

	m_trailEffect[m_trailNum].setSize(sf::Vector2f(M_NUM_OF_TRAILS, M_NUM_OF_TRAILS));
	m_trailEffect[m_trailNum].setOrigin(m_trailEffect[m_trailNum].getGlobalBounds().width / 2, m_trailEffect[m_trailNum].getGlobalBounds().height / 2);
	m_trailEffect[m_trailNum].setPosition(m_player.getPosition());
	m_trailNum++;
	if (m_trailNum >= M_NUM_OF_TRAILS)
	{
		m_trailNum = 0;
	}

	// change the size of the trails trail effect
	for (int i = 0; i < M_NUM_OF_TRAILS; i++)
	{
		m_trailEffect[i].setSize(m_trailEffect[i].getSize() - sf::Vector2f(1.0f, 1.0f));
		m_trailEffect[i].setOrigin(m_trailEffect[i].getGlobalBounds().width / 2, m_trailEffect[i].getGlobalBounds().height / 2);
	}
}



float Game::vectorLength(sf::Vector2f t_vector)
{
	const float length = sqrt((t_vector.x * t_vector.x) + (t_vector.y * t_vector.y));
	return length;
}



float Game::signOf(float t_value)
{
	float sign = 0.0f;

	if (t_value > 0)
	{
		sign = 1.0f;
	}
	if (t_value < 0)
	{
		sign = -1.0f;
	}

	return sign;
}

bool Game::isColliding(sf::RectangleShape t_subjectOne, sf::RectangleShape t_subjectTwo, sf::Vector2f t_position)
{
	// Set the bool
	bool colliding = false;

	// Get the distance vector
	sf::Vector2f distanceVector = t_position - t_subjectTwo.getPosition();

	// Get the X and Y distance values
	float xDistance = sqrt(distanceVector.x * distanceVector.x);
	float yDistance = sqrt(distanceVector.y * distanceVector.y);

	// Check horisontal collisions
	if (xDistance < t_subjectOne.getGlobalBounds().width / 2 + t_subjectTwo.getGlobalBounds().width / 2)
	{
		// Check vertical collisions
		if (yDistance < t_subjectOne.getGlobalBounds().height / 2 + t_subjectTwo.getGlobalBounds().height / 2)
		{
			colliding = true;
		}
	}

	return colliding;
}