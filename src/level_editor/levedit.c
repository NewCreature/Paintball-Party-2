#include "t3f/t3f.h"
#include "t3f/view.h"
#include "t3f/menu.h"
#include "file/level.h"
#include "legacy/palette.h"
#include "legacy/animation.h"
#include "legacy/level.h"
#include "pp2/gameplay/sprites/objects.h"
#include "pp2/gameplay/sprites/object_defines.h"
#include "instance.h"

bool levedit_initialize(int argc, char * argv[])
{
	PP2_LEVEL_EDITOR_INSTANCE * level_editor = NULL;

	level_editor = pp2_create_level_editor_instance(argc, argv);
	if(!level_editor)
	{
		goto fail;
	}
	return true;

	fail:
	{
		return false;
	}
}

int main(int argc, char * argv[])
{
	if(!levedit_initialize(argc, argv))
	{
		printf("Initialization failed!\n");
		return -1;
	}
	t3f_run();
	return 0;
}
