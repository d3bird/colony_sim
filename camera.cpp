#include "camera.h"



typedef Angel::vec4  point4;
typedef Angel::vec4  color4;
using namespace std;
camera::camera() {

	//booleans for movments
	mforward = false;
	mleft = false;
	mright = false;
	mbackward = false;
	mup = false;
	mdown = false;

	//Everything to do with the camera
	cameraPos = vec3(0.0f, 2.0f, 3.0f);
	cameraTarget = vec3(-1.0f, 2.0f, -1.0f);
	cameraDirection = normalize(cameraPos - cameraTarget);
	up = vec3(0.0f, 1.0f, 0.0f);
	cameraRight = normalize(cross(up, cameraDirection));
	cameraUp = cross(cameraDirection, cameraRight);
	view = LookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	cameraFront = vec3(0.0f, 0.0f, -1.0f);
	cameraSpeed = 0.1f;

	//window vars
	Wheight = 900;
	Wwidth = 900;


	//vars for the first person 
	firstMouse = true;
	yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	pitch = 0.0f;
	lastX = 450.0f;
	lastY = 450;
	fov = 45.0f;

	x = 0;
	y = 0;
	z = -1;
	moveamount = 1;
	// the information for the camer pos
	camera_angle = 45.0; // Camera's angle of view in degrees

	controlerSensitivityL = 10;
	invert_con_look = false;
	grids = false;
}

bool camera::connectControllerConected() {
	int temp;
	XINPUT_STATE state;
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)	{
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool camera::processControllerInput() {
	XINPUT_STATE state;
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);

		if (dwResult == ERROR_SUCCESS) {
			float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
			float normLY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

			float normRX = fmaxf(-1, (float)state.Gamepad.sThumbRX / 32767);
			float normRY = fmaxf(-1, (float)state.Gamepad.sThumbRY / 32767);

			int buttons = state.Gamepad.wButtons;
			//std::cout << normRX << std::endl;

			if (normLX >= 0.25) {
				Setmright();
			}
			else if (normLX <= -0.25) {
				Setmleft();
			}

			if (normLY >= 0.25) {
				Setmforward();
			}
			else if (normLY <= -0.25) {
				Setmbackward();
			}

			if (buttons == XINPUT_GAMEPAD_A) {
				Setmup();
			}
			else if (buttons == XINPUT_GAMEPAD_B) {
				Setmdown();
			}

			if (buttons == XINPUT_GAMEPAD_X) {
				grids = !grids;
			}

			bool updatelook = false;
			int xpos = lastX;// +(controlerSensitivityL * normRX);
			int ypos = lastY;// +(controlerSensitivityL * normLX);

			if (normRX >= 0.25 || normRX <= -0.25) {
				xpos += (controlerSensitivityL * normRX);
				updatelook = true;
			}
			if (normRY >= 0.25 || normRY <= -0.25) {
				if (invert_con_look) {
					ypos += (controlerSensitivityL * normRY);
				}
				else {
					ypos -= (controlerSensitivityL * normRY);
				}
				updatelook = true;
			}

			if (updatelook) {

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
			}


			return true;
		}
		else {
			std::cout << "controller not connected" << std::endl;
			return false;
		}
	}

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

	 projection = Perspective(camera_angle, aspect, zNear, zFar);

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