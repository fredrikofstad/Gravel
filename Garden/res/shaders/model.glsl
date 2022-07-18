#type vertex
#version 330 core
layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec2 texCoords;

out vec2 TexCoords;
out flat int entityID;

uniform mat4 model;
uniform mat4 view;
uniform int entity;


void main( )
{
    gl_Position = view * model * vec4( position, 1.0f );
    TexCoords = texCoords;
    entityID = entity;
}

#type fragment

#version 330 core

in vec2 TexCoords;
in flat int entityID;


layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

uniform sampler2D texture_diffuse;

void main( )
{
    color = vec4( texture( texture_diffuse, TexCoords ));
    color2 = entityID;
}
