#include "Game.h"

std::map<char, Character> Characters;
GLuint VAO, VBO;
FT_Library FontLoader::ft;

int FontLoader::Init()
{
	//FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	return 0;
}

TextRenderer* FontLoader::Load(glm::mat4* projection, char* fontPath, int width, int height)
{
	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		//return -1;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, width, height);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// configure VAO/VBO for texture quads
	// -----------------------------------
	return new TextRenderer(projection, new Shader("shaders/text.vert.glsl", "shaders/text.frag.glsl"), &Characters);
}

TextRenderer::TextRenderer(glm::mat4* projection, Shader* shader, std::map<char, Character>* charset)
	: shader(shader),
	  projection(projection),
	  shader2("shaders/test.vert.glsl", "shaders/test.frag.glsl")
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Game::Game(GLFWwindow* window, int xunits, int yunits)
	: state(State::MENU),
	  xunits(xunits),
	  yunits(yunits),
	  menuWindows{},
	  shader("shader.vert", "shader.frag"),
	  textshader("shaders/text.vert.glsl", "shaders/text.frag.glsl")
{
	/* Associate GLFW window user pointer with the game object being created */
	glfwSetWindowUserPointer(window, this);

	/* Initialize main menu */
	InitMainMenu();

	/* Register callbacks */
	glfwSetKeyCallback(window, Game::KeyCallback);
	//glfwSetCursorPosCallback(window, Game::SetMouseCoordinates);
	//glfwSetMouseButtonCallback(window, Game::LeftMousePressed);

	/* Set projection matrix */
	SetProjection(xunits, yunits);

	/* Init font loader */
	//FontLoader::Init();
	//glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800.0f), 0.0f, static_cast<float>(600.0f));
	//projection2 = glm::ortho(0.0f, static_cast<float>(800.0f), 0.0f, static_cast<float>(600.0f));
	//textRenderer = FontLoader::Load(projection.matrix, (char*)"fonts/arial.ttf", 0, 48);
	//textRenderer = FontLoader::Load(&projection, (char*)"fonts/arial.ttf", 0, 48);

	
	
	//textshader.use();
	//glUniformMatrix4fv(glGetUniformLocation(textshader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//glUniformMatrix4fv(glGetUniformLocation(textshader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection2));

	FontLoader::Init();
	textRenderer = FontLoader::Load(&projection2, (char*)"fonts/arial.ttf", 0, 24);
	//textRenderer = FontLoader::Load(projection.matrix, (char*)"fonts/arial.ttf", 0, 24);
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
	//MenuWindow* window = GetTopWindow();

	//window->SetMouseX(mouseX);
	//window->SetMouseY(mouseY);

	//if (mousePressed)
	//{
	//	window->HandleLeftMousePressed();

	//	/*
	//	 * In the next frame it should not hold true if left mouse IS NOT pressed,
	//	 * so we manually deactivate it
	//	*/
	//	mousePressed = false;
	//}

	//if (mouseReleased)
	//{
	//	window->HandleLeftMouseReleased();

	//	/* Same as for mousePressed */
	//	mouseReleased = false;
	//}

	//glUniformMatrix4fv(projection.loc, 1, GL_TRUE, glm::value_ptr(*projection.matrix));
	textRenderer->Render("1Hello world asd ksa[dk osakd opkao pdkasop kopask do", 50.0f, 50.0f, 1.0f, glm::vec3(0.5f, 0.0f, 1.0f));
	//textRenderer->Render("Hello world asd ksa[dk osakd opkao pdkasop kopask do", 200, 200, 1.0f, glm::vec3(0.5f, 0.0f, 1.0f));
	//window->Render();
	//RenderText(textshader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
	//RenderText(textshader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
	//RenderText(textshader, "Hello world asd ksa[dk osakd opkao pdkasop kopask do", 200, 200, 1.0f, glm::vec3(0.5f, 0.0f, 1.0f));
}

MenuWindow* Game::GetTopWindow()
{
	return menuWindows.val;
}

//void Game::RegisterWindowCallback(WindowCallback callback)
//{
//	windowCallbacks.Push(callback);
//}

//void Game::ProcessUserInput(GLFWwindow* window)
//{
//	if (windowCallbacks.length > 0)
//	{
//		for (auto li = windowCallbacks.head; li; li = li->next)
//		{
//			WindowCallback callback = li->val;
//			callback(window);
//		}
//	}
//}

void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void Game::SetMouseCoordinates(GLFWwindow* window, double xpos, double ypos)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	game->mouseX = xpos;
	game->mouseY = ypos;
	//std::cout << "x: " << game->mouseX << ", y = " << ypos << std::endl;
}

void Game::LeftMousePressed(GLFWwindow* window, int button, int action, int mods)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		game->mousePressed = action == GLFW_PRESS;
		game->mouseReleased = action == GLFW_RELEASE;
	}
}

void Game::InitMainMenu()
{
	/* Create menu window */
	MenuWindow* window = new MenuWindow;
	menuWindows.val = window;
	menuWindows.prev = nullptr;

	/* Create cursor */

	/* Create and add menu objects */
	Button* startGameBtn = new Button(&shader, 10.0f, 10.0f, 200.0f, 60.0f, glm::vec3(0.0f, 0.2f, 0.8f), (void*)this);
	//Button* btn2 = new Button(&shader, 30.0f, 30.0f, 300.0f, 100.0f, glm::vec3(1.0f, 0.2f, 0.8f), (void*)this);
	startGameBtn->OnClick([](void *data) -> void
		{
			Game* game = (Game*)data;
			game->SetState(State::START_GAME);
		}
	);
	window->AddObject(startGameBtn);
	//window->AddObject(btn2);
}

void Game::SetProjection(int xunits, int yunits)
{
	projection.matrix = new glm::mat4(
		2.0f / xunits,  0.0f         , 0.0f, -1.0f,
		0.0f         , -2.0f / yunits, 0.0f,  1.0f,
		0.0f         ,  0.0f         , 0.0f,  0.0f,
		0.0f         ,  0.0f         , 0.0f,  1.0f
	);
	projection2 = glm::ortho(0.0f, static_cast<float>(800.0f), 0.0f, static_cast<float>(600.0f));
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
int count = 0;

void TextRenderer::Render(std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state
	//glm::mat4 projection(
	//	2.0f / 800.0f,  0.0f         , 0.0f,  0.0f,
	//	0.0f         , -2.0f / 600.0f, 0.0f,  1.0f,
	//	0.0f         ,  0.0f         , -1.0f,  0.0f,
	//	-1.0f         ,  -1.0f         , 0.0f,  1.0f
	//);
	glm::mat4 projection(
		2.0f / 800.0f,  0.0f         , 0.0f,  0.0f,
		0.0f         , -2.0f / 600.0f, 0.0f,  0.0f,
		0.0f         ,  0.0f         , 0.0f,  0.0f,
		-1.0f         ,  1.0f         , 0.0f,  1.0f
	);
	shader->use();

	glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y, color.z);
	//glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(*projection));
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	//if (count == 0) {
	//	/*for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			std::cout << glm::to_string(*projection) << " ";
	//		}
	//		std::cout << std::endl;
	//	}*/
	//	std::cout << glm::to_string(projection);
	//	count++;
	//}

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};


		if (count == 0)
		{
			glm::vec4 res[6];

			for (int i = 0; i < 6; i++)
			{
				//res[i] = (*projection) * glm::vec4(vertices[i][0], vertices[i][1], 0.0f, 1.0f);
				res[i] = projection * glm::vec4(vertices[i][0], vertices[i][1], 0.0f, 1.0f);
			}

			for (int i = 0; i < 6; i++)
			{
				//for (int j = 0; j < 4; j++)
				//{
				//	std::cout << res[i][j] << " ";
				//}
				std::cout << glm::to_string(res[i]) << std::endl;
				
			}
			count++;
			
		}
		

		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}