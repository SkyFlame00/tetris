//#include "FontLoader.h"
//
//FT_Library FontLoader::ft;
//
//int FontLoader::Init()
//{
//	if (FT_Init_FreeType(&ft))
//	{
//		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//		return -1;
//	}
//}
//
//TextRenderer* FontLoader::Load(glm::mat4* projection, char* fontPath, int width, int height)
//{
//	FT_Face face;
//
//	if (FT_New_Face(ft, fontPath, 0, &face))
//	{
//		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//		return nullptr;
//	}
//
//	//FT_Set_Pixel_Sizes(face, width, height);
//    FT_Set_Pixel_Sizes(face, 0, 48);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    std::map<char, Character>* Characters = new std::map<char, Character>;
//
//    for (unsigned char c = 0; c < 128; c++)
//    {
//        // load character glyph 
//        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//        {
//            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//            continue;
//        }
//        // generate texture
//        unsigned int texture;
//        glGenTextures(1, &texture);
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glTexImage2D(
//            GL_TEXTURE_2D,
//            0,
//            GL_RED,
//            face->glyph->bitmap.width,
//            face->glyph->bitmap.rows,
//            0,
//            GL_RED,
//            GL_UNSIGNED_BYTE,
//            face->glyph->bitmap.buffer
//        );
//        // set texture options
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        // now store character for later use
//        Character character = {
//            texture,
//            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//            face->glyph->advance.x
//        };
//        Characters->insert(std::pair<char, Character>(c, character));
//    }
//
//    glBindTexture(GL_TEXTURE_2D, 0);
//    FT_Done_Face(face);
//    FT_Done_FreeType(ft);
//
//    return new TextRenderer(projection, new Shader("shaders/text.vert.glsl", "shaders/text.frag.glsl"), Characters);
//}