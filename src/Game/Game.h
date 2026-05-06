#pragma once

#include <glm/vec2.hpp>
#include <array>
#include <iostream>

class Tank;

class Game {
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void render(); // Вызывается каждый раз, когда отрисовывается кадр игры
	void update(const uint64_t delta); // Обновление спрайтов в игре
	void setKey(const int key, const int action); // Обработка нажатий клавиш
	bool init(); // Инициализация игры для подгрузки необходимого

private:
	std::array<bool, 349> m_keys;

	enum class EGameState {
		Active,
		Pause
	};

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;
	std::unique_ptr<Tank> m_pTank;
};