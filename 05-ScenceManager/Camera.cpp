#include "Camera.h"
Camera::Camera()
{
	cam_x = 0.00f;
	cam_y = (float)-SCREEN_HEIGHT;
	Mario = NULL;
}
void Camera::Update()
{
	if (Mario == NULL)
		return;
	if (Mario->isInsidePlayScence == false)
	{
		cam_x = 0.0f;
		cam_y = -SCREEN_HEIGHT + 64.0f;
		CGame::GetInstance()->SetCamPos(round(cam_x), round(cam_y));
		return;
	}
	float cx, cy;
	Mario->GetPosition(cx, cy);
	
	if (cx > (float)SCREEN_WIDTH/2 && cy<=0)
	{
		cx -= (float)SCREEN_WIDTH / 2;
		if (abs(cy) > SCREEN_HEIGHT)
		{
			cy -= (float)SCREEN_HEIGHT / 2;
		}
		else
			cy= (float)-SCREEN_HEIGHT + 64.0f;
		cam_x = cx;
		cam_y = cy;
		if (cam_x > CAM_X_MAX_WHEN_Y_NEGATIVE)
			cam_x = CAM_X_MAX_WHEN_Y_NEGATIVE;
	}
	else if (cy > 0)
	{
		cx -= (float)SCREEN_WIDTH / 2;
		if (cx >= CAM_X_MIN_WHEN_UNDERGROUND && cx <= CAM_X_MAX_WHEN_UNDERGROUND)
		{
			cam_x = cx;
			cam_y = CAM_Y_WHEN_UNDERGROUND;
		}		
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
