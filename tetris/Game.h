#ifndef GAME_H
#define GAME_H

#include "MenuWindow.h"
#include "MenuObject.h"
#include "data_structures/List.h"
#include <glm/glm.hpp>
#include "Playground.h"
#include "data_structures/misc.h"
#include "helpers/MatrixTool.h"
//#include "FontLoader.h"
//#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

void _RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

class TextRenderer
{
public:
	//TextRenderer(glm::mat4* projection, Shader* shader, std::map<char, Character>* charset);
	TextRenderer(glm::mat4* projection, Shader* shader, std::map<char, Character>* charset);
	~TextRenderer();
	void Render(std::string text, float x, float y, float scale, glm::vec3 color);
private:
	glm::mat4* projection;
	GLuint VAO, VBO;
	Shader* shader;
	std::map<char, Character>* charset;

	Shader shader2;
	GLuint boxVAO, boxVBO;
};

class FontLoader
{
public:
	static int Init();
	static TextRenderer* Load(glm::mat4* projection, char* fontPath, int width, int height);
	//static void Load(glm::mat4* projection, char* fontPath, int width, int height);
	static FT_Library ft;
private:
};

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
	//Cursor cursor;
	List<void (*)(GLFWwindow*)> windowCallbacks;
	Matrix projection;
	Playground* playground;
	TextRenderer* textRenderer;

	void Menu();
	void InitMainMenu();
	MenuWindow* GetTopWindow();
	void SetProjection(int xunits, int yunits);
	void Play();

	/* Callbacks to register */
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void LeftMousePressed(GLFWwindow* window, int button, int action, int mods);
	static void SetMouseCoordinates(GLFWwindow* window, double xpos, double ypos);

	Shader textshader;
	GLuint VAO, VBO;
	std::map<char, Character> Characters;
	void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
	glm::mat4 projection2;
};

#endif