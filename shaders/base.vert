#version 450

layout(location = 0) in vec3 vInPosition;

layout(location = 0) out vec3 fragmentColor;

void main()
{
	fragmentColor = vec3(1.0, 0.2, 0.2);
	gl_Position = vec4(vInPosition, 1.0);
}
