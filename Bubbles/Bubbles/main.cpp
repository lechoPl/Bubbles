#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <cstdio>
#include <vector>
#include <ctime>

#include "shader.hpp"
#include "GeometryGenerator.h"
#include "Light.h"
#include "Material.h"

#include "Game.h"

GLFWwindow* window;

const unsigned WINDOW_WIDTH = 640;
const unsigned WINDOW_HEIGHT = 480;

const glm::vec3 COLOR_CLEAR_COLOR(0.f, 0.f, 0.f);

const glm::vec2 DEFAULT_MOUSE_POS(WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0);

CGame game;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

MOVEDIR keyToMoveDir(int key)
{
	switch (key)
	{
	case GLFW_KEY_UP:
	case GLFW_KEY_W: return MOVE_FORWARD;
	case GLFW_KEY_DOWN:
	case GLFW_KEY_S: return MOVE_BACKWARD;
	case GLFW_KEY_RIGHT:
	case GLFW_KEY_D: return MOVE_RIGHT;
	case GLFW_KEY_LEFT: 
	case GLFW_KEY_A: return MOVE_LEFT;
	case GLFW_KEY_LEFT_SHIFT:
	case GLFW_KEY_PAGE_UP: return MOVE_UP;
	case GLFW_KEY_LEFT_CONTROL:
	case GLFW_KEY_PAGE_DOWN: return MOVE_DOWN;
	default: return MOVE_NONE;
	}
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		game.changeCamera();

	if ((key == GLFW_KEY_R || key == GLFW_KEY_F5) && action == GLFW_PRESS) 
		game.init();

	if (game.getCurrentAquarium() == NULL)
		return;

	//move player
	CPlayer& player = game.getCurrentAquarium()->getPlayer();
	MOVEDIR tempDir = keyToMoveDir(key);
	if (tempDir != MOVE_NONE)
	{
		if(action == GLFW_PRESS)
			player.move(tempDir, true);

		if (action == GLFW_RELEASE)
			player.move(tempDir, false);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset < 0)
		game.getCamera()->zoomOut();

	if (yoffset > 0)
		game.getCamera()->zoomIn();
}

void update(double dt)
{
	game.update(dt);

	//mouse update
	double mousePosX, mousePosY;
	glfwGetCursorPos(window, &mousePosX, &mousePosY);

	game.updateCamera(DEFAULT_MOUSE_POS, dt, mousePosX, mousePosY);

	glfwSetCursorPos(window, DEFAULT_MOUSE_POS.x, DEFAULT_MOUSE_POS.y);
}

int main(void)
{
	srand(static_cast<unsigned>(time(NULL)));
	

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bubbles", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//set key callback
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);


	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		return -1;
	}

	glClearColor(COLOR_CLEAR_COLOR.r, COLOR_CLEAR_COLOR.g, COLOR_CLEAR_COLOR.b, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders("VertexShader.vshader", "FragmentShader.fshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "model");
	GLuint CameraPosID = glGetUniformLocation(programID, "cameraPosition");
	GLuint PlayerPosID = glGetUniformLocation(programID, "playerPosition");
	GLuint MaxDistanceID = glGetUniformLocation(programID, "maxDistance");

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<UINT> indices;

	//insert data to buffers
	MeshData tempMesh;

	CreateSphere(1.f, 20, 20, tempMesh);

	for (unsigned i = 0; i < tempMesh.Vertices.size(); ++i)
	{
		Vertex& tempV = tempMesh.Vertices.at(i);

		vertices.push_back(tempV.Position);
		normals.push_back(tempV.Normal);
	}

	const unsigned sphereIndicesOffset = indices.size() * sizeof(unsigned int);
	for (unsigned i = 0; i < tempMesh.Indices.size(); ++i)
	{
		indices.push_back(tempMesh.Indices.at(i));
	}
	const unsigned sphereVericesNumber = tempMesh.Vertices.size();
	const unsigned sphereIndicesSize = tempMesh.Indices.size();


	CreateAquarium(1.f, 1.f, 1.f, tempMesh);
	for (unsigned i = 0; i < tempMesh.Vertices.size(); ++i)
	{
		Vertex& tempV = tempMesh.Vertices.at(i);

		vertices.push_back(tempV.Position);
		normals.push_back(tempV.Normal);
	}

	const unsigned aquariumIndicesOffset = indices.size() * sizeof(unsigned int);
	for (unsigned i = 0; i < tempMesh.Indices.size(); ++i)
	{
		indices.push_back(sphereVericesNumber + tempMesh.Indices.at(i));
	}
	const unsigned aquariumVerices = tempMesh.Vertices.size();
	const unsigned aquariumIndicesSize = tempMesh.Indices.size();

	
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(window, DEFAULT_MOUSE_POS.x, DEFAULT_MOUSE_POS.y);

	game.init();

	double lastTime = glfwGetTime();
	double currentTime = lastTime;

	
	while (!glfwWindowShouldClose(window))
	{
		if (game.getCurrentAquarium() == NULL)
			return -1;

		
		lastTime = currentTime;
		currentTime = glfwGetTime();
		update(currentTime - lastTime);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Compute the MVP matrix from keyboard and mouse input
		glm::mat4 ProjectionMatrix = game.getCamera()->getProjectionMatix();
		glm::mat4 ViewMatrix = game.getCamera()->getViewMatrix();
		
		// Use our shader
		glUseProgram(programID);
		
		glUniform3fv(CameraPosID, 1, glm::value_ptr(game.getCamera()->getPos()));
		glUniform3fv(PlayerPosID, 1, glm::value_ptr(game.getCurrentAquarium()->getPlayer().getPos()));
		glUniform1f(MaxDistanceID, 0.5f * game.getCurrentAquarium()->getWidth());

		//set lights
		setLightUniform(programID, genereateLights(*game.getCurrentAquarium()));

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);


		// Draw aquarium
		glm::mat4 ModelMatrix = 
			glm::translate(game.getCurrentAquarium()->getPos()) *
			glm::scale(glm::vec3(game.getCurrentAquarium()->getWidth(), game.getCurrentAquarium()->getHeight(), game.getCurrentAquarium()->getDepth()));
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		setMaterialUniform(programID, g_material_aquarium);
		glDrawElements(GL_TRIANGLES, aquariumIndicesSize, GL_UNSIGNED_INT, (void*)aquariumIndicesOffset);

		//draw bubbles
		std::vector<CBubble*>&  bubbles = game.getCurrentAquarium()->getBubbles();
		setMaterialUniform(programID, g_material_bubble);
		for (unsigned i = 0; i < bubbles.size(); ++i)
		{
			CBubble* bubble = bubbles[i];
			if (bubble == NULL)
				continue;

			ModelMatrix =
				glm::translate(bubble->getPos()) *
				glm::scale(glm::vec3(bubble->getRadius(), bubble->getRadius(), bubble->getRadius()));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glDrawElements(GL_TRIANGLES, sphereIndicesSize, GL_UNSIGNED_INT, (void*)sphereIndicesOffset);
		}

		//draw special bubbles
		std::vector<CBubble*>&  specialBubbles = game.getCurrentAquarium()->getSpecialBubbles();
		setMaterialUniform(programID, g_material_special_bubble);
		for (unsigned i = 0; i < specialBubbles.size(); ++i)
		{
			CBubble* bubble = specialBubbles[i];
			if (bubble == NULL)
				continue;

			ModelMatrix =
				glm::translate(bubble->getPos()) *
				glm::scale(glm::vec3(bubble->getRadius(), bubble->getRadius(), bubble->getRadius()));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glDrawElements(GL_TRIANGLES, sphereIndicesSize, GL_UNSIGNED_INT, (void*)sphereIndicesOffset);
		}

		//draw player
		ModelMatrix =
			glm::translate(game.getCurrentAquarium()->getPlayer().getPos()) *
			glm::scale(glm::vec3(game.getCurrentAquarium()->getPlayer().getRadius()));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		setMaterialUniform(programID, g_material_player);
		glDrawElements(GL_TRIANGLES, sphereIndicesSize, GL_UNSIGNED_INT, (void*)sphereIndicesOffset);
		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

	}

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();
	return 0;
}