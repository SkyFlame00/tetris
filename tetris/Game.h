#ifndef GAME_H
#define GAME_H

#include "MenuWindow.h"
#include "MenuObject.h"
#include "data_structures/List.h"
#include <glm/glm.hpp>
#include "Playground.h"
#include "data_structures/misc.h"
#include "helpers/MatrixTool.h"
#include "FontLoader.h"
#include "TextRenderer.h"

#include <iostream>
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

#include "LayoutElement.h"
#include "LayoutContainer.h"
#include "Button.h"
#include "Sprite.h"

typedef void (*WindowCallback)(GLFWwindow* window);

enum class State {
	INIT,
	MENU,
	START_GAME,
	PLAYING,
	MENU_WHILE_PLAYING
};

struct StackItem
{
	MenuWindow* val;
	StackItem* prev;
};

class Game
{
public:
	Game(GLFWwindow *window, int xunits, int yunits);
	~Game();

	void Render();
	void RegisterWindowCallback(WindowCallback window);
	void ProcessUserInput(GLFWwindow *window);
	void StartGame();
	void SetState(State state);
	void Update(float deltaTime, UserInput* userInput);

	State state;
	Shader shader;
	double mouseX;
	double mouseY;
	bool mousePressed;
	bool mouseReleased;
private:
	int xunits, yunits;
	StackItem menuWindows;
	List<void (*)(GLFWwindow*)> windowCallbacks;
	Matrix projection;
	Playground* playground;
	TextRenderer* textRenderer;
	Sprite* logo;
	Shader* spriteShader;
	Matrix* spriteProjection;

	void Menu();
	void InitMainMenu();
	MenuWindow* GetTopWindow();
	void SetProjection(int xunits, int yunits);
	void Play();
};

#endif