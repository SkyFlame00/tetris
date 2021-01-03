#include "Game.h"

Game::Game(GLFWwindow* window, int xunits, int yunits)
	: state(State::MENU),
	  xunits(xunits),
	  yunits(yunits),
	  menuWindows{},
	  shader("shader.vert", "shader.frag")
{
	/* Associate GLFW window user pointer with the game object being created */
	glfwSetWindowUserPointer(window, this);

	/* Register callbacks */

	/* Set projection matrix */
	SetProjection(xunits, yunits);

	/* Init font loader */
	FontLoader::Init();
	textRenderer = FontLoader::Load(projection.matrix, (char*)"fonts/arial.ttf", 0, 24);

	/* Initialize main menu */
	InitMainMenu();
}

Game::~Game()
{
	delete projection.matrix;
	/* DELETE THE STACK OF THE MENU WINDOWS */
}

void Game::Render()
{
	switch (state)
	{
	case State::PLAYING:
		Play();
		break;
	case State::MENU:
		Menu();
		break;
	case State::START_GAME:
		StartGame();
		break;
	case State::MENU_WHILE_PLAYING:
		break;
	default:
		break;
	}
}

void Game::Menu()
{
	MenuWindow* window = GetTopWindow();

	window->SetMouseX(mouseX);
	window->SetMouseY(mouseY);

	if (mousePressed)
	{
		window->HandleLeftMousePressed();
	}

	if (mouseReleased)
	{
		window->HandleLeftMouseReleased();
	}
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	window->Render();
	//textRenderer->Render("1Hello world asd ksa[dk osakd opkao pdkasop kopask do", 50.0f, 50.0f, 1.0f, glm::vec3(0.5f, 0.0f, 1.0f));
	
}

MenuWindow* Game::GetTopWindow()
{
	return menuWindows.val;
}

void Game::InitMainMenu()
{
	/* Create menu window */
	MenuWindow* window = new MenuWindow;
	menuWindows.val = window;
	menuWindows.prev = nullptr;

	/* Create cursor */

	/* Create and add menu objects */
	using namespace ButtonPackage;
	s_AppearanceSettings aSettings;
	aSettings.xpos = 10.0f;
	aSettings.ypos = 10.0f;
	aSettings.width = 200.0f;
	aSettings.height = 60.0f;
	aSettings.color = glm::vec3(0.0f, 0.2f, 0.8f);
	aSettings.colorPressed = glm::vec3(0.0f, 0.2f, 0.8f);
	s_TextSettings tSettings;
	tSettings.size = 24;
	tSettings.text = (char*)"Press me";
	tSettings.color = glm::vec3(0.0f, 0.0f, 0.3f);
	tSettings.colorPressed = glm::vec3(1.0f, 0.0f, 0.3f);

	Button* startGameBtn = new Button(&projection, &shader, textRenderer, &aSettings, &tSettings, (void*)this);
	tSettings.text = (char*)"shit";
	Button* btn2 = new Button(&projection, &shader, textRenderer, &aSettings, &tSettings, (void*)this);
	tSettings.text = (char*)"OH MY FUCKING GOD";
	Button* btn3 = new Button(&projection, &shader, textRenderer, &aSettings, &tSettings, (void*)this);
	startGameBtn->OnClick([](void *data) -> void
		{
			Game* game = (Game*)data;
			game->SetState(State::START_GAME);
		}
	);
	window->AddObject(startGameBtn);
	window->AddObject(btn2);
	window->AddObject(btn3);

	/* Layout container */
	LayoutContainer* layoutContainer = new LayoutContainer;
	LayoutElement* el1 = new LayoutElement(startGameBtn, 0, 0, 0, 20);
	LayoutElement* el2 = new LayoutElement(btn2, 0, 0, 0, 0);
	LayoutElement* el3 = new LayoutElement(btn3, 0, 0, 150, 0);
	layoutContainer->AddElement(el1);
	layoutContainer->AddElement(el2);
	layoutContainer->AddElement(el3);
	layoutContainer->SetOffsetX(xunits / 2 - layoutContainer->GetWidth() / 2);
	layoutContainer->SetOffsetY(yunits / 2 - layoutContainer->GetHeight() / 2);
}

void Game::SetProjection(int xunits, int yunits)
{
	projection.matrix = new glm::mat4(
		2.0f / xunits,  0.0f         , 0.0f, -1.0f,
		0.0f         , -2.0f / yunits, 0.0f,  1.0f,
		0.0f         ,  0.0f         , 0.0f,  0.0f,
		0.0f         ,  0.0f         , 0.0f,  1.0f
	);
	projection.loc = glGetUniformLocation(shader.ID, "projection");

	if (projection.loc == -1)
	{
		std::cout << "ERROR::GAME::SET_PROJECTION: Location of projection matrix cannot be found." << std::endl;
		return;
	}
}

void Game::SetState(State state)
{
	this->state = state;
}

void Game::StartGame()
{
	this->playground = new Playground(projection.matrix, xunits, yunits);
	this->SetState(State::PLAYING);
	Play();
}

void Game::Play()
{
	playground->Render();
}

void Game::Update(float deltaTime, UserInput* userInput)
{
	switch (state)
	{
	case State::MENU:
		mouseX = userInput->mouseX;
		mouseY = userInput->mouseY;
		mousePressed = userInput->mousePressed;
		mouseReleased = userInput->mouseReleased;
		break;
	case State::PLAYING:
		playground->Update(deltaTime, userInput);
		break;
	default:
		break;
	}
}
