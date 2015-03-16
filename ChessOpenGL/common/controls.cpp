#include <iostream>

// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* glfwwindow; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 RotationMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}
glm::mat4 getRotationMatrix(){
	return RotationMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float rotationSpeed = 1.0f;
float mouseSpeed = 0.005f;

bool hasClicked = false;

int windowX, windowY;

int rDelay = 0;
double lastTime = glfwGetTime();

double ogxpos = 0;
double ogypos = 0;

float g_scale = 0.0f;
float g_mouseWheelScale = 10.0f;

void ScrollCallback(GLFWwindow* window, double x, double y)
{
	g_scale += g_mouseWheelScale * (float)y;
}

void computeMatricesFromInputs(){

	glfwGetWindowSize(glfwwindow, &windowX, &windowY);

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	if (glfwGetMouseButton(glfwwindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){

		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(glfwwindow, &xpos, &ypos);

		if (hasClicked){

			// Reset mouse position for next frame
			glfwSetCursorPos(glfwwindow, windowX / 2, windowY / 2);

			// Compute new orientation
			horizontalAngle += mouseSpeed * float(windowX / 2 - xpos);
			verticalAngle += mouseSpeed * float(windowY / 2 - ypos);
		}

		else{
			ogxpos = xpos;
			ogypos = ypos;
			glfwSetCursorPos(glfwwindow, windowX / 2, windowY / 2);
			hasClicked = true;

		}
	}

	if (glfwGetMouseButton(glfwwindow, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE && hasClicked){
		glfwSetCursorPos(glfwwindow, ogxpos, ogypos);
		hasClicked = false;
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);



	//// Move forward
	//if (glfwGetKey(glfwwindow, GLFW_KEY_UP) == GLFW_PRESS){
	//	position += direction * deltaTime * speed;
	//}
	//// Move backward
	//if (glfwGetKey(glfwwindow, GLFW_KEY_DOWN) == GLFW_PRESS){
	//	position -= direction * deltaTime * speed;
	//}
	//// Strafe right
	//if (glfwGetKey(glfwwindow, GLFW_KEY_RIGHT) == GLFW_PRESS){
	//	position += right * deltaTime * speed;
	//}
	//// Strafe left
	//if (glfwGetKey(glfwwindow, GLFW_KEY_LEFT) == GLFW_PRESS){
	//	position -= right * deltaTime * speed;
	//}
	// Move Up
	if (glfwGetKey(glfwwindow, ' ') == GLFW_PRESS) {
		position += up * deltaTime * speed;
	}
	// Down
	if (glfwGetKey(glfwwindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(glfwwindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
		position -= up * deltaTime * speed;
	}

	position += direction * g_scale * deltaTime * speed;
	g_scale = 0;

	//RotationMatrix = glm::mat4(1.0f);

	// Rotation attempt
	if (glfwGetKey(glfwwindow, GLFW_KEY_UP) == GLFW_PRESS){
		glm::vec3 myRotationAxis(1, 0, 0);
		RotationMatrix = glm::rotate(RotationMatrix, -rotationSpeed, myRotationAxis);
	}
	if (glfwGetKey(glfwwindow, GLFW_KEY_DOWN) == GLFW_PRESS){
		glm::vec3 myRotationAxis(1, 0, 0);
		RotationMatrix = glm::rotate(RotationMatrix, rotationSpeed, myRotationAxis);
	}
	if (glfwGetKey(glfwwindow, GLFW_KEY_LEFT) == GLFW_PRESS){
		glm::vec3 myRotationAxis(0, 1, 0);
		RotationMatrix = glm::rotate(RotationMatrix, -rotationSpeed, myRotationAxis);
	}
	if (glfwGetKey(glfwwindow, GLFW_KEY_RIGHT) == GLFW_PRESS){
		glm::vec3 myRotationAxis(0, 1, 0);
		RotationMatrix = glm::rotate(RotationMatrix, rotationSpeed, myRotationAxis);
	}
	if (glfwGetKey(glfwwindow, 'R') == GLFW_PRESS){
		glm::vec3 myRotationAxis(0, 0, 1);
		RotationMatrix = glm::rotate(RotationMatrix, -rotationSpeed, myRotationAxis);
	}
	if (glfwGetKey(glfwwindow, 'Y') == GLFW_PRESS){
		glm::vec3 myRotationAxis(0, 0, 1);
		RotationMatrix = glm::rotate(RotationMatrix, rotationSpeed, myRotationAxis);
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
