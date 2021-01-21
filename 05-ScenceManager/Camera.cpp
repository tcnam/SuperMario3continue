#include "Camera.h"
Camera::Camera()
{
	cam_x = 0.00f;
	cam_y = (float)-SCREEN_HEIGHT + 64.0f;
	Mario = NULL;
	vx = 0;
	vy = 0;
	dt = 0;
	DebugOut(L"cam_y min:%f\n", cam_y);
}
void Camera::Update(DWORD t)
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
	dt = t;
	float cx, cy;
	Mario->GetPosition(cx, cy);
	if (CGame::GetInstance()->GetSceneNumber() == 2)
	{
		if (cx > (float)SCREEN_WIDTH / 2 && cy <= 0&&cx< CAM_X_MAX_WHEN_Y_NEGATIVE+SCREEN_WIDTH)
		{
			cx -= (float)SCREEN_WIDTH / 2;
			if (abs(cy) > SCREEN_HEIGHT - 16.0f)
			{
				cy -= (float)SCREEN_HEIGHT / 2;
			}
			else
				cy = (float)-SCREEN_HEIGHT + 64.0f;
			cam_x = cx;
			cam_y = cy;
			if (cam_x > CAM_X_MAX_WHEN_Y_NEGATIVE)
				cam_x = CAM_X_MAX_WHEN_Y_NEGATIVE;
		}
		else if (cx > CAM_X_MAX_WHEN_Y_NEGATIVE + SCREEN_WIDTH && cy <= 0)
		{
			cx -= (float)SCREEN_WIDTH / 2;
			if (cx >= CAM_X_MAX_WHEN_Y_NEGATIVE + SCREEN_WIDTH + BONUS_SCENCE_1_1 && cx <= CAM_X_MAX_WHEN_Y_NEGATIVE+2*SCREEN_WIDTH)
			{
				cam_x = cx;
			}		
			cam_y= (float)-SCREEN_HEIGHT + 64.0f;
		}
		/*else if (cy > 0)
		{
			cx -= (float)SCREEN_WIDTH / 2;
			if (cx >= CAM_X_MIN_WHEN_UNDERGROUND && cx <= CAM_X_MAX_WHEN_UNDERGROUND)
			{
				cam_x = cx;
				cam_y = CAM_Y_WHEN_UNDERGROUND;
			}
		}*/
		else
		{
			cam_x = 0.0f;
			cam_y = -SCREEN_HEIGHT + 64.0f;
		}
	}
	else if (CGame::GetInstance()->GetSceneNumber() == 3)
	{
		if (cx > 0 && cy <= 0&&cx<=2032.0f)
		{
			vx = CAM_SPEED_VX_SCENCE_1_4;
			cam_x += vx*dt;
			if (cx < cam_x)
				Mario->SetPosition(cam_x, cy);
			if (cam_x > 1776.0f||cx>1776.0f)
				cam_x = 1776.0f;
			cam_y = (float)-SCREEN_HEIGHT + 64.0f;
		}
		else if (cx > 2032.0f)
		{
			cx -= (float)SCREEN_WIDTH / 2;
			cy = (float)-SCREEN_HEIGHT + 64.0f;
			cam_x = cx;
			if (cam_x > 2304.0f)
				cam_x = 2304.0f;
			if (cam_x < 2064.0f)
				cam_x = 2064.0f;
			cam_y = cy;
		}
		else
		{
			cam_x = 0.0f;
			cam_y = -SCREEN_HEIGHT + 64.0f;
		}
	}
	
	CGame::GetInstance()->SetCamPos(round(cam_x), round(cam_y));
	if (Mario != NULL)
	{
		
		if (checkifMarioInside() == false && cy>0)
		{
			if (CGame::GetInstance()->GetLife() == 1)
				CGame::GetInstance()->SetLife(4);
			else
			{
				CGame::GetInstance()->SetLife(CGame::GetInstance()->GetLife() - 1);
			}			
			CGame::GetInstance()->SwitchScene(1);
		}
			
	}
	
}
bool Camera::checkifMarioInside()
{
	float cx, cy;
	Mario->GetPosition(cx, cy);
	bool result = CGame::GetInstance()->AABBCheck(cx, cy, cx + 30.0f, cy + 30.0f, cam_x, cam_y, cam_x + SCREEN_WIDTH, cam_y + SCREEN_HEIGHT-32.0f);
	return result;
}
bool Camera::CheckIfObjectInside(float x, float y)
{
	bool result = CGame::GetInstance()->AABBCheck(x, y, x + 16.0f, y + 16.0f, cam_x, cam_y, cam_x + SCREEN_WIDTH, cam_y + SCREEN_HEIGHT - 64.0f);
	return result;
}
Camera::~Camera()
{

}
