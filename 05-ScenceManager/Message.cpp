#include "Message.h"
CMessage::CMessage()
{
	isDisplay = false;
}
void CMessage::Render()
{
	if (isDisplay == false)
		return;
	if (state == MESSAGE_STATE_MUSHROOM)
		animation_set->at(MESSAGE_STATE_MUSHROOM)->Render(x, y);
	else if (state == MESSAGE_STATE_FLOWER)
		animation_set->at(MESSAGE_STATE_FLOWER)->Render(x, y);
	else
		animation_set->at(MESSAGE_STATE_STAR)->Render(x, y);
}
CMessage::~CMessage()
{

}