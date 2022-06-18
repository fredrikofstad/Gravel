#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_textureCoordinates;

out vec2 v_textureCoordinates;

uniform mat4 u_viewProjection;
uniform mat4 u_transform;


void main()
{
	gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
	v_textureCoordinates = a_textureCoordinates;
};


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_textureCoordinates;

uniform sampler2D u_texture;

void main()
{
	color = texture(u_texture, v_textureCoordinates);
};
