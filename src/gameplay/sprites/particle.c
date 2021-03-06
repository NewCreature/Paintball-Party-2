#include "../../t3f/t3f.h"
#include "../camera.h"
#include "particle.h"
#include "particle_defines.h"
#include "../../resource.h"
#include "object_defines.h"

void pp2_particle_logic(PP2_GAME * gp, PP2_PARTICLE * pp)
{
	pp->x += pp->vx;
	pp->y += pp->vy;
	pp->z += pp->vz;
	pp->vy += 1.0;
	if(pp->vy > 14.0)
	{
		pp->vy = 14.0;
	}

	/* deactivate particle if it goes off the playfield */
	if(pp->x < gp->level->room.x * 32 - 128 || pp->x > gp->level->room.bx * 32 + 32 + 128 || pp->y < gp->level->room.y * 32 - 128 || pp->y > gp->level->room.by * 32 + 32 + 128)
	{
		pp->flags = 0;
		return;
	}

	pp->life--;
	if(pp->life <= 0)
	{
		pp->flags = 0;
	}

	pp->tick++;
}

void pp2_particle_render(PP2_GAME * gp, PP2_PARTICLE * pp, PP2_CAMERA * cp, PP2_RESOURCES * resources)
{
	float alpha;

	if(pp->flags & PP2_PARTICLE_FLAG_ACTIVE)
	{
		alpha = (float)pp->life / (float)pp->total_life;
		if(pp->type == 0)
		{
			t3f_draw_rotated_animation(gp->player[pp->owner].character->animation[gp->player[pp->owner].character->state[pp->state].particle.animation], al_map_rgba_f(alpha, alpha, alpha, alpha), pp->tick, gp->player[pp->owner].character->state[pp->state].particle.cx, gp->player[pp->owner].character->state[pp->state].particle.cy, pp->x + gp->player[pp->owner].character->state[pp->state].particle.cx - cp->x, pp->y + gp->player[pp->owner].character->state[pp->state].particle.cy - cp->y, -cp->z, gp->player[pp->owner].character->state[pp->state].particle.angle, 0);
		}
		else
		{
			t3f_draw_rotated_animation(resources->object_animation[PP2_OBJECT_JET], al_map_rgba_f(alpha, alpha, alpha, alpha), pp->tick, resources->object_animation[PP2_OBJECT_JET]->frame[0]->width / 2, resources->object_animation[PP2_OBJECT_JET]->frame[0]->height / 2, pp->x + resources->object_animation[PP2_OBJECT_JET]->frame[0]->width / 2 - cp->x, pp->y + resources->object_animation[PP2_OBJECT_JET]->frame[0]->width / 2 - cp->y, -cp->z, 0, 0);
		}
	}
}
