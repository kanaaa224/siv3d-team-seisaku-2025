/* Copied from "https://github.com/kanaaa224/siv3d-2d-game-base" */

# include "ObjectBase.hpp"
# include "../Stages/Stage.hpp"

void ObjectBase::deleteSelf()
{
	Stage::GetInstance()->deleteObject(this);
}
