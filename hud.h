#pragma once
#include "Angel.h"
//#include "InitShader.h"
#include "shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <map>

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

struct Character {
	GLuint   TextureID;  // ID handle of the glyph texture
	vec2 Size;       // Size of glyph
	vec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};


class hud {
public:

	hud();

	void draw();
	void update();
	void init();
	void setindex(int i) { index = i; }
	void setColorloc(GLuint i) { coloring = i; }
	GLuint get_points_size() { return sizeof(points); }

	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color);

	void setProjection(mat4 p) { projection = p; }

private:
	const static int NumVertices = 36;
	int index;
	point4 points[NumVertices];
	color4 background;
	GLuint coloring;

	

	int width;//315
	int length;//
	int height;//

	//everything to do with text generation
	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;
	GLuint program;
	GLuint projLoc;
	Shader *shader;
	mat4 projection;
};

