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
	//if (cx > float(SCREEN_WIDTH/2))
	//{
	//	cx -= float(SCREEN_WIDTH / 2);
	//	cy -= float(SCREEN_HEIGHT / 2);
	//	if (cy > float(SCREEN_HEIGHT * 3 / 2))
	//		cy = -SCREEN_HEIGHT;
	//	CGame::GetInstance()->SetCamPos(round(cx), round(cy) + 32);
	//}
	//else if (cy > 0)
	//{
	//	cx -= float(SCREEN_WIDTH / 2);
	//	cy -= float(SCREEN_HEIGHT / 2);
	//	CGame::GetInstance()->SetCamPos(round(cx), round(cy) + 32);
	//}
	//else
	//	CGame::GetInstance()->SetCamPos(0, round((float)(-SCREEN_HEIGHT + 32)));
	CGame::GetInstance()->SetCamPos(0, -192);

}
Camera::~Camera()
{

}
