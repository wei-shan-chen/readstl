#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <math.h>

#include "ball.h"
#include "shader.h"
#include "matrixStack.h"
#include "item.h"
#include "camera.h"
#include "world.h"
#include "stb_image.h"
#include "STLmodel.h"
#include "Texture.h"

#include <iostream>
#include <array>

#define TSIZE 64

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void comput_axis();
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(10.5f, 10.5f, 35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
// cube
glm::vec3 cubePos(7.5f, 0.0f, 7.5f);
float cube_scale = 6.0f;
// light
int turn = 0;
// objectpos
glm::vec3 objectPos(0.0f, 0.0f, 0.0f);

// texture
unsigned int textName[2];

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------

	Shader ourShader("shader/vShader.vs", "shader/fShader.fs");			  // you can name your shader files however you like
	Shader lightShader("shader/lightShader.vs", "shader/lightShader.fs"); // you can name your shader files however you like

	STLmodel stlModel;
	stlModel.LoadSTLfile("stl/ball.stl");

	create_world();
	Item triangle(world.tri);
	Item squarett(world.squarett, world.squ_indices);
	Item square(world.square, world.squ_indices);
	Item lightCube(world.cube);
	Item stlmodel(stlModel.m_MeshTri);

	// std::cout << stlModel.m_MaxPos[0] << " " << stlModel.m_MaxPos[1] << " " << stlModel.m_MaxPos[2] << std::endl;
	// std::cout << stlModel.m_MinPos[0] << " " << stlModel.m_MinPos[1] << " " << stlModel.m_MinPos[2] << std::endl;

	Texture texture;
	texture.setUpTexture("textures/a.png");
	Ball ball;
	ball.comput_axis(cubePos, cube_scale);
	glEnable(GL_DEPTH_TEST);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// move(keyboard&mouth)
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		std::cout << "deltaTime: " << deltaTime << std::endl;
		lastFrame = currentFrame;
		ball.ball_collision(0.2, deltaTime, cubePos);
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render the triangle
		ourShader.use();

		for (int i = 0; i < 4; i++)
		{
			MatrixStack view;
			MatrixStack projection;
			MatrixStack model;
			switch (i)
			{
			case 0:
				glViewport(0, SCR_HEIGHT / 2, SCR_WIDTH / 2, SCR_HEIGHT / 2);
				projection.Save(glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
				view.Save(camera.GetViewMatrix());
				ourShader.setVec3("viewPos", camera.Position);
				break;
			case 1: // x
				glViewport(SCR_WIDTH / 2, SCR_HEIGHT / 2, SCR_WIDTH / 2, SCR_HEIGHT / 2);
				projection.Save(glm::perspective(glm::radians(120.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
				view.Save(glm::lookAt(glm::vec3(20.0f, 10.5, 10.5), glm::vec3(0.0f, 10.5f, 10.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
				ourShader.setVec3("viewPos", glm::vec3(20.0f, 10.5, 10.5));
				break;
			case 2: // y
				glViewport(0, 0, SCR_WIDTH / 2, SCR_HEIGHT / 2);
				projection.Save(glm::perspective(glm::radians(120.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
				view.Save(glm::lookAt(glm::vec3(10.5f, 20.0, 10.5), glm::vec3(10.0f, 6.0f, 10.5f), glm::vec3(0.0f, 0.0f, 1.0f)));
				ourShader.setVec3("viewPos", glm::vec3(10.5f, 20, 10.5));
				break;
			case 3: // z
				glViewport(SCR_WIDTH / 2, 0, SCR_WIDTH / 2, SCR_HEIGHT / 2);
				projection.Save(glm::perspective(glm::radians(120.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
				view.Save(glm::lookAt(glm::vec3(10.5f, 10.5, 22), glm::vec3(10.0f, 10.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
				ourShader.setVec3("viewPos", glm::vec3(10.5f, 10.5, 22));
				break;
			default:
				break;
			}
			// light properties
			ourShader.setVec3("dirLight.direction", 0.0f, 1.0f, -1.0f);
			ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
			ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
			// point light
			ourShader.setVec3("pointLight.position", 10.5f, 0.5f, 10.5f);
			ourShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
			ourShader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
			ourShader.setFloat("pointLight.constant", 1.0f);
			ourShader.setFloat("pointLight.linear", 0.09f);
			ourShader.setFloat("pointLight.quadratic", 0.032f);
			// material properties
			ourShader.setInt("material.diffuse1", 0);
			ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			ourShader.setFloat("material.shininess", 32.0f);

			ourShader.setInt("turn", turn);
			// ball
			ourShader.setInt("no_texture", 1);
			for (int j = 0; j < ball.num; j++)
			{
				
				if (ball.color[j] == 1)
					ourShader.setVec3("cubeColor", 1.0f, 0.0f, 0.0f);
				else if (ball.color[j] == 0)
					ourShader.setVec3("cubeColor", 0.0f, 0.0f, 1.0f);
				else
					ourShader.setVec3("cubeColor", 0.0f, 1.0f, 0.0f);
				model.Push();
				model.Save(glm::translate(model.Top(), glm::vec3(ball.move[j][0], ball.move[j][1], ball.move[j][2])));
				model.Save(glm::scale(model.Top(), glm::vec3(stlModel.m_scale * 0.4, stlModel.m_scale * 0.4, stlModel.m_scale * 0.4)));
				ourShader.setMat4("model", model.Top());
				ourShader.setMat4("view", view.Top());
				ourShader.setMat4("projection", projection.Top());
				glBindVertexArray(stlmodel.VAO);
				glDrawArrays(GL_TRIANGLES, 0, stlModel.triangleNum);
				model.Pop();
			}
			

			{
				ourShader.setInt("no_texture", 0);
				model.Push();
				model.Save(glm::scale(model.Top(), glm::vec3(21.0f, 21.0f, 21.0f))); //---------------------------
				model.Push();														 // floor
				model.Save(glm::rotate(model.Top(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
				ourShader.setMat4("model", model.Top());
				ourShader.setMat4("view", view.Top());
				ourShader.setMat4("projection", projection.Top());
				texture.useTexture();
				glBindVertexArray(squarett.VAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				model.Pop();

				model.Push(); // in
				ourShader.setMat4("model", model.Top());
				ourShader.setMat4("view", view.Top());
				ourShader.setMat4("projection", projection.Top());
				glBindVertexArray(squarett.VAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				model.Pop();

				model.Push(); // left
				model.Save(glm::rotate(model.Top(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
				ourShader.setMat4("model", model.Top());
				ourShader.setMat4("view", view.Top());
				ourShader.setMat4("projection", projection.Top());
				glBindVertexArray(squarett.VAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				model.Pop();

				model.Push(); // right
				model.Save(glm::translate(model.Top(), glm::vec3(1.0, 0.0, 0.0)));
				model.Save(glm::rotate(model.Top(), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
				ourShader.setMat4("model", model.Top());
				ourShader.setMat4("view", view.Top());
				ourShader.setMat4("projection", projection.Top());
				glBindVertexArray(squarett.VAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				model.Pop();

				model.Push(); // up
				model.Save(glm::translate(model.Top(), glm::vec3(0.0, 1.0, 0.0)));
				model.Save(glm::rotate(model.Top(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

				ourShader.setMat4("model", model.Top());
				ourShader.setMat4("view", view.Top());
				ourShader.setMat4("projection", projection.Top());
				glBindVertexArray(squarett.VAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				model.Pop();
				model.Pop();
			}
			// light
			ourShader.use();
			ourShader.setInt("no_texture", 1);
			ourShader.setVec3("cubeColor", 1.0f, 1.0f, 1.0f);
			model.Push();
			model.Save(glm::translate(model.Top(), cubePos));
			model.Save(glm::scale(model.Top(), glm::vec3(cube_scale, cube_scale, cube_scale)));
			// model.Save(glm::rotate(model.Top(), glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
			ourShader.setMat4("view", view.Top());
			ourShader.setMat4("projection", projection.Top());
			ourShader.setMat4("model", model.Top());
			glBindVertexArray(lightCube.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			model.Pop();
		}
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	destroy_world();
	return 0;
}

// move ball collision

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		objectPos += glm::vec3(0.0f, 2.5 * deltaTime, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		objectPos -= glm::vec3(0.0f, 2.5 * deltaTime, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		objectPos += glm::vec3(0.0f, 0.0f, 2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		objectPos -= glm::vec3(0.0f, 0.0f, 2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		if (turn == 0)
			turn = 1;
		else
			turn = 0;
	}
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}