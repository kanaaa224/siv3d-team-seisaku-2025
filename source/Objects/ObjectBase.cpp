/* Copied from "https://github.com/kanaaa224/siv3d-2d-scroll" */

# include "ObjectBase.hpp"
# include "../Stages/Stage.hpp"

void ObjectBase::deleteSelf()
{
	Stage::GetInstance()->deleteObject(this);
}
