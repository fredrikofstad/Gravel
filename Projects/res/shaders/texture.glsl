#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_textureCoordinates;
layout(location = 3) in float a_textureIndex;
layout(location = 4) in float a_tiling;

out vec2 v_textureCoordinates;
out vec4 v_color;
out float v_textureIndex;
out float v_tiling;

uniform mat4 u_viewProjection;

void main()
{
	v_color = a_color;
	v_textureCoordinates = a_textureCoordinates;
	v_textureIndex = a_textureIndex;
	v_tiling = a_tiling;
	gl_Position = u_viewProjection * vec4(a_position, 1.0);
};


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_textureCoordinates;
in vec4 v_color;
in float v_textureIndex;
in float v_tiling;

uniform sampler2D u_textures[32];

void main()
{
	vec4 textureColor = v_color;

	int index = int(v_textureIndex);
	const int indexLowerBound = 0, indexUpperBound = 31;
	if (index >= indexLowerBound && index <= indexUpperBound) 
		textureColor *= texture(u_textures[index], v_textureCoordinates * v_tiling);

	color = textureColor;
}