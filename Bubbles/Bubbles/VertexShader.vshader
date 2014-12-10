#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

out vec3 Position_modelspace;
out vec3 Normal_modelspace;

uniform mat4 MVP;

void main(){	
	Position_modelspace = vertexPosition_modelspace;
	Normal_modelspace = vertexNormal_modelspace;

	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
}
