//#ifndef TEXT_RENDERER_H
//#define TEXT_RENDERER_H
//
//#include <ft2build.h>
//#include FT_FREETYPE_H
//#include <glad/glad.h>
//#include "Shader.h"
//#include "data_structures/misc.h"
//#include <string>
//#include <map>
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//class TextRenderer
//{
//public:
//	TextRenderer(glm::mat4* projection, Shader* shader, std::map<char, Character>* charset);
//	~TextRenderer();
//	void Render(std::string text, float x, float y, float scale, glm::vec3 color);
//private:
//	glm::mat4* projection;
//	GLuint VAO, VBO;
//	Shader* shader;
//	std::map<char, Character>* charset;
//};
//
//#endif // !TEXT_RENDERER_H