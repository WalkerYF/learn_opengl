#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* ģʽ�л�, Ĭ��Ϊ�鿴ģʽ��1Ϊ̽��ģʽ */
int mode_toggle = 0;

/* ģʽ�л���flatģʽ�� */
int show_mode = 0;

/* ��Դλ�� */
glm::vec3 lightPos = glm::vec3(1.0f);
/* ��Դ��ɫ */
glm::vec3 lightCol = glm::vec3(1.0f, 1.0f, 1.0f);

/* ����ģʽ����ر��� */

// ����ʱ���Ĺ��λ��
int mouse_press_flag = 0;
float curse_press_x = 0;
float curse_press_y = 0;

// ʵʱ�����
float curse_x = 0;
float curse_y = 0;

float translation_x = 0;
float translation_y = 0;
float translation_z = -3.0f;

/* ������࣬���ڸı��ӽ� */
Camera my_camera = Camera();

glm::mat4 mouse_rolate = glm::mat4(1.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
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

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ourShader("resources/hw5.vs", "resources/hw5.fs");

	// load models
	// -----------
	Model ourModel("resources/teapot.obj");

	/* ����ÿһ֡��ʱ����� */
	float delta_time = 0.0f; // ��ǰ֡����һ֡��ʱ���
	float last_frame = glfwGetTime(); // ��һ֡��ʱ��

									  /* ����ÿһ֡����ƶ��Ĳ��� */
	float last_x = 400, last_y = 300;
	float delta_x = 0;
	float delta_y = 0;
	int first_flag = 1;

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		/* ������� */
		if (first_flag) {
			/* ����ǵ�һ�Σ�����������Ϣ���������ӽ�ת�� */
			last_x = curse_x;
			last_y = curse_y;
			first_flag = 0;
		}
		else {
			/* ���ǵ�һ�Σ�ÿһ�ζ���������һ֡�����λ�ò��죬�����ò��촫��������У��޸�������Ƕ� */
			delta_x = curse_x - last_x;
			delta_y = last_y - curse_y; /* ע��y���෴�� */
			last_x = curse_x;
			last_y = curse_y;
			if (mode_toggle) {
				my_camera.process_mouse_movement(delta_x, delta_y);
			}
		}

		/* ����֡��ʱ */
		delta_time = glfwGetTime() - last_frame;
		last_frame = glfwGetTime();
		my_camera.update_delta_time(delta_time);

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// don't forget to enable shader before setting uniforms
		ourShader.use();

		if (mode_toggle) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (mouse_press_flag) {
			// ת���ӽ�
			// cout << curse_x-curse_press_x << "   " << curse_y-curse_press_y << endl;
			float angle = sqrt((curse_x - curse_press_x / 2) *(curse_x - curse_press_x / 2) + (curse_y - curse_press_y / 2) * (curse_y - curse_press_y / 2));
			if (abs(curse_y - curse_press_y) > abs(curse_x - curse_press_x)) {
				mouse_rolate = glm::rotate(mouse_rolate, angle / 1000 * (curse_press_y - curse_y), glm::vec3(1.0f, 0.0f, 0.0f));
			}
			else {
				mouse_rolate = glm::rotate(mouse_rolate, angle / 1000 * (curse_x - curse_press_x), glm::vec3(0, 1.0f, 0.0f));
			}
		}

		// ����ģ�;���
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(translation_x, translation_y, translation_z));
		model = mouse_rolate * model;
		ourShader.setMat4("model", model);


		/* ����view���� */
		glm::mat4 view = my_camera.get_view_matrix();
		ourShader.setMat4("view", view);

		/* ����ͶӰ��Ϣ / ����ͶӰ���Ƕ�Ϊ45��*/
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		ourShader.setMat4("projection", projection);

		/* ���ù�����Ϣ */
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("lightCol", lightCol);
		ourShader.setVec3("objectCol", glm::vec3(0.2f, 0.2f, 0.2f));

		ourModel.Draw(ourShader);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (mode_toggle) {
		/* ̽��ģʽ���� */
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			my_camera.process_keyboard_input(LEFT);
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			my_camera.process_keyboard_input(RIGHT);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			// translation_y += 0.001;
			my_camera.process_keyboard_input(BACKWARD);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			// translation_y += 0.001;
			my_camera.process_keyboard_input(FORWARD);
		}
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			// translation_y += 0.001;
			my_camera.process_keyboard_input(UP);
		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			// translation_y += 0.001;
			my_camera.process_keyboard_input(DOWN);
		}
	}
	else {
		/* ����ģʽ�趨 */
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			translation_y += 0.005;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			translation_y -= 0.005;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			translation_x += 0.005;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			translation_x -= 0.005;
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			translation_z += 0.004;
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			translation_z -= 0.004;
		}
	}


	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		for (int i = 0; i < 100000000; i++);
		{
			if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
				mode_toggle = 1 - mode_toggle;
				if (mode_toggle) {
					cout << "You are in exploring mode!" << endl;
				}
				else {
					cout << "You are in operating mode!" << endl;
				}
			}
		}
	}


	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		show_mode = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		show_mode = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		show_mode = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		my_camera.reflesh();
		translation_x = 0;
		translation_y = 0;
		translation_z = -3.0f;
		mouse_rolate = glm::mat4(1.0f);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	// translation_x = 0;
	// translation_y = 0;
	// translation_z = -2.0f;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	curse_x = 2 * (xpos / SCR_WIDTH - 0.5);
	curse_y = 2 * (0.5 - ypos / SCR_HEIGHT);
	if (mouse_press_flag == 0) {
		curse_press_x = curse_x;
		curse_press_y = curse_y;
	}

	// if (firstMouse)
	// {
	//     lastX = xpos;
	//     lastY = ypos;
	//     firstMouse = false;
	// }

	// float xoffset = xpos - lastX;
	// float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	// lastX = xpos;
	// lastY = ypos;

	// camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// camera.ProcessMouseScroll(yoffset);
}




void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	/* �տ�ʼ���£���¼������Ϣ */
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mouse_press_flag == 0) {
		cout << "mouse press!" << endl;
		mouse_press_flag = 1;
		// curse_press_x = curse_x;
		// curse_press_y = curse_y;

		// std::cout << "(pos:" << curse_x << "," << curse_y << ")" << std::endl;
		// std::cout << "(pos:" << curse_press_x << "," << curse_press_y << ")" << std::endl;
	}
	/* �շ���������ձ�־ */
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		mouse_press_flag = 0;
		curse_press_x = curse_x;
		curse_press_y = curse_y;
	}
	// s1.add_point(curse_x, curse_y);
}
