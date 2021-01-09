#include "Camera.h"
Camera::Camera()
{
	cam_x = 0.00f;
	cam_y = -SCREEN_HEIGHT;
	Mario = NULL;
}
void Camera::Update()
{
	float cx, cy;
	Mario->GetPosition(cx, cy);
	if (cx > float(SCREEN_WIDTH/2)&&cy<=0)
	{
		cx -= float(SCREEN_WIDTH / 2);
		if (abs(cy) > SCREEN_HEIGHT)
		{
			cy -= float(SCREEN_HEIGHT / 2);
		}
		else
			cy= -SCREEN_HEIGHT + 64.0f;
		cam_x = cx;
		cam_y = cy;
	}
	else if (cy > 0)
	{
		cx -= float(SCREEN_WIDTH / 2);
		cy -= float(SCREEN_HEIGHT / 2);
		cam_x = cx;
		cam_y = cy;
	}
	else
	{
		cam_x = 0.0f;
		cam_y = -SCREEN_HEIGHT+64.0f;
	}
	CGame::GetInstance()->SetCamPos(round(cam_x), round(cam_y));
}
Camera::~Camera()
{

}
