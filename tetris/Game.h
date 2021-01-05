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
#include "MenuController.h"
#include "MainMenuWindow.h"
#include "SettingsWindow.h"
#include "PlainText.h"

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
	void StartGame();
	void SetState(State state);
	void Update(float deltaTime, UserInput* userInput);

	State state;
	Shader shader;
	double mouseX;
	double mouseY;
	bool mousePressed;
	bool mouseReleased;
	MenuControllerPackage::MenuController menuController;
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
	GLFWwindow* appWindow;
	TextRenderer* textRenderer_72;

	void Menu();
	void InitMainMenu();
	void InitSettings();
	MenuWindow* GetTopWindow();
	void SetProjection(int xunits, int yunits);
	void Play();
};

#endif