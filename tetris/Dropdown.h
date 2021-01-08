#ifndef DROPDOWN_H
#define DROPDOWN_H

#include "MenuObject.h"
#include "TextRenderer.h"
#include <string>
#include <list>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

namespace DropdownPackage
{

	typedef std::pair < std::string, std::string > Option;
	typedef std::list < Option > Options;

	enum class Theme
	{
		DEFAULT
	};

	struct Settings
	{
		Options options;
		Theme theme;
		int width;
		std::string chosenKey;
	};

	struct OptionObject
	{
		GLuint borderVAO, bgVAO, borderVBO, bgVBO;
		float *borderVertices, *bgVertices;
	};

	struct OptionInstance
	{
		glm::mat4 modelMatrix;
		std::string text, value;
		bool chosen;
		float textX, textY;
	};

	struct Choice
	{
		float* borderVertices, *bgVertices;
		GLuint borderVAO, bgVAO, borderVBO, bgVBO;
		glm::vec3 borderColor, bgColor;
		float textX, textY;
		OptionInstance* option;
	};

	class Dropdown : public MenuObject
	{
	public:
		Dropdown(Matrix* projection, Shader* shader, TextRenderer* textRenderer, Settings* settings, void* eventData);
		~Dropdown();

		bool OverlapsPoint(float x, float y);
		void HandleLeftMousePressed(float x, float y);
		void HandleLeftMouseReleased(bool releasedOnObject, float x, float y);
		void HandleFocus();
		void HandleFocusLost();
		void HandleHover(float x, float y);
		void HandleHoverLost();
		void SetOriginX(int originX);
		void SetOriginY(int originY);
		void OnChange(void (*)(Option, void*));
		void Render();
	private:
		bool isExpanded;
		Choice choice;
		OptionObject optionObject;
		std::list<OptionInstance> options;
		TextRenderer* textRenderer;
		float borderThickness, padding, expandedHeight, contentWidth, contentHeight, triangularBoxWidth, triangularBoxHeight;
		void* eventData;
		struct {
			glm::vec3 borderColor;
			glm::vec3 bgColor;
			glm::vec3 textColor;
			glm::vec3 chosenOptionBgColor;
			glm::vec3 chosenOptionTextColor;
			glm::vec3 hoveredOptionBgColor;
		} themeOptions;
		OptionInstance* hoveredOption;

		void Change(Option option);
		void (*onChangeCallback)(Option option, void*);
	};

}

#endif // !DROPDOWN_H
