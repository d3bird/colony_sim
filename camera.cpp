#include "camera.h"

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

camera::camera() {


}

void camera::motion(int xpos, int ypos)
{
	//std::cout<<"mopuse is moving:"<<xpos<<" "<<ypos<<std::endl;
	//std::cout<<"check"<<std::endl;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	vec3 front;
	front.x = cos(DegreesToRadians * yaw) * cos(DegreesToRadians * pitch);
	front.y = sin(DegreesToRadians * pitch);
	front.z = sin(DegreesToRadians * yaw) * cos(DegreesToRadians * pitch);
	cameraFront = normalize(front);
	//glutWarpPointer(450,450); 

	if (xpos < 50 || xpos > Wwidth - 50) {  //you can use values other than 100 for the screen edges if you like, kind of seems to depend on your mouse sensitivity for what ends up working best
		lastX = Wwidth / 2;   //centers the last known position, this way there isn't an odd jump with your cam as it resets
		lastY = Wheight / 2;
		glutWarpPointer(Wwidth / 2, Wheight / 2);  //centers the cursor
	}
	else if (ypos < 50 || ypos > Wheight - 50) {
		lastX = Wwidth / 2;
		lastY = Wheight / 2;
		glutWarpPointer(Wwidth / 2, Wheight / 2);
	}

	//glutPostRedisplay();
}

void camera::reshape(int width, int height) {
	glViewport(0, 0, width, height);

	Wheight = width;
	Wheight = height;

	GLfloat left = -2.0, right = 2.0;
	GLfloat top = 2.0, bottom = -2.0;

	// Use following for ortho projection
	//  GLfloat zNear = -20.0, zFar = 20.0;

	// Use following for perspective projection
	zNear = 0.2;
	zFar = 40.0;

	aspect = GLfloat(width) / height;

	if (aspect > 1.0) {
		left *= aspect;
		right *= aspect;
	}
	else {
		top /= aspect;
		bottom /= aspect;
	}

	mat4 projection = Perspective(camera_angle, aspect, zNear, zFar);

	// Can use either perspective or ortho projection.
	 // mat4 projection = Ortho(left, right, bottom, top, zNear, zFar);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

void camera::moveCam() {
	if (mforward) {
		cameraPos += cameraSpeed * cameraFront;
		mforward = false;
	}
	if (mbackward) {
		cameraPos -= cameraSpeed * cameraFront;
		mbackward = false;
	}
	if (mleft) {
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		mleft = false;
	}
	if (mright) {
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		mright = false;
	}
	if (mup) {
		cameraPos.y += cameraSpeed;
		mup = false;
	}
	if (mdown) {
		cameraPos.y -= cameraSpeed;
		mdown = false;
	}


	mat4 view = LookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(Modelview, 1, GL_TRUE, (view));
}