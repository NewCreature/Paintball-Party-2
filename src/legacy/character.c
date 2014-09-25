#include <stdio.h>
#include "t3f/t3f.h"
#include "t3f/atlas.h"
#include "../pp2.h"
#include "../file/character.h"
#include "../file/wav.h"
#include "animation.h"

int pp2_flash_table[PP2_CHARACTER_MAX_STATES] =  {  0,   1,   2,   3,   0,   1,   0,   1,   0,   1,   0,   1,   0,   1,   2,   3,   0,   1,   0,   1,   0,   1,   0,   1};
int pp2_legacy_state_table[12] = {PP2_CHARACTER_STATE_STAND_R_R, PP2_CHARACTER_STATE_STAND_L_L, PP2_CHARACTER_STATE_STAND_R_U, PP2_CHARACTER_STATE_STAND_L_U, PP2_CHARACTER_STATE_WALK_R_R, PP2_CHARACTER_STATE_WALK_L_L, PP2_CHARACTER_STATE_DUCK_R_R, PP2_CHARACTER_STATE_DUCK_L_L, PP2_CHARACTER_STATE_JUMP_R_R, PP2_CHARACTER_STATE_JUMP_L_L, PP2_CHARACTER_STATE_FALL_R_R, PP2_CHARACTER_STATE_FALL_L_L};

static void pp2_character_replace_extension(char * fn, const char * ext)
{
	int p = strlen(fn);
	int i;
	
	while(p >= 0)
	{
		p--;
		if(fn[p] == '.')
		{
			for(i = 0; i < strlen(ext); i++)
			{
				fn[p + 1 + i] = ext[i];
			}
			fn[p + 1 + i] = '\0';
			break;
		}
	}
}

bool pp2_legacy_load_sounds(PP2_CHARACTER * cp, const char * fn)
{
	ALLEGRO_FILE * fp;
	int i;
	
	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		return 0;
	}
	for(i = 0; i < 64; i++)
	{
		if(al_fgetc(fp))
		{
			cp->sample[i] = pp2_load_wav_f(fp);
//			cp->sample[i] = al_load_wav_f(fp);
			if(!cp->sample[i])
			{
				return 0;
			}
		}
	}
	al_fclose(fp);
	return 1;
}

static PP2_CHARACTER * pp2_load_legacy_character_f(ALLEGRO_FILE * fp, int flags)
{
	int i;
	int temp;
	PP2_CHARACTER * cp;
	int mix, miy, mir;
	int ix, iy;

	cp = malloc(sizeof(PP2_CHARACTER));
	if(!cp)
	{
		return NULL;
	}
	memset(cp, 0, sizeof(PP2_CHARACTER));
	
	al_fread32le(fp);
	temp = al_fread32le(fp);
	if(temp)
	{
		cp->animation[cp->animations] = pp2_legacy_load_ani_fp(fp, NULL); // paintball
		cp->animations++;
	}
	else
	{
		return NULL;
	}
	temp = al_fread32le(fp);
	if(temp)
	{
		cp->animation[cp->animations] = pp2_legacy_load_ani_fp(fp, NULL); // particle
		cp->animations++;
	}
	else
	{
		return NULL;
	}
	for(i = 0; i < 4; i++)
	{
		temp = al_fread32le(fp);
		if(temp)
		{
			cp->animation[cp->animations] = pp2_legacy_load_ani_fp(fp, NULL); // flash
			cp->animations++;
		}
		else
		{
			return NULL;
		}
	}
	for(i = 0; i < 24; i++)
	{
		temp = al_fread32le(fp);
		if(temp)
		{
			cp->animation[cp->animations] = pp2_legacy_load_ani_fp(fp, NULL); // state
			cp->animations++;
		}
		else
		{
			return NULL;
		}
		if(i < 12)
		{
			cp->state[pp2_legacy_state_table[i]].piece[0].animation = i + 6;
			cp->state[pp2_legacy_state_table[i]].piece[0].x = 0.0;
			cp->state[pp2_legacy_state_table[i]].piece[0].y = 0.0;
			cp->state[pp2_legacy_state_table[i]].piece[0].cx = 0.0;
			cp->state[pp2_legacy_state_table[i]].piece[0].cy = 0.0;
			cp->state[pp2_legacy_state_table[i]].piece[0].angle = 0.0;
			cp->state[pp2_legacy_state_table[i]].pieces = 1;
			
			cp->state[pp2_legacy_state_table[i]].collision_x = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i]].collision_y = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i]].collision_w = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i]].collision_h = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i]].paintball.animation = 0;
			cp->state[pp2_legacy_state_table[i]].paintball.x = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i]].paintball.y = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i]].paintball.cx = cp->animation[0]->frame[0]->width / 2;
			cp->state[pp2_legacy_state_table[i]].paintball.cy = cp->animation[0]->frame[0]->height / 2;
//			cp->state[pp2_legacy_state_table[i]].paintball.x -= cp->state[pp2_legacy_state_table[i]].paintball.cx;
//			cp->state[pp2_legacy_state_table[i]].paintball.y -= cp->state[pp2_legacy_state_table[i]].paintball.cy;
			cp->state[pp2_legacy_state_table[i]].paintball.angle = 0.0;
			cp->state[pp2_legacy_state_table[i]].fire_piece[1].animation = pp2_flash_table[i] + 2;
			cp->state[pp2_legacy_state_table[i]].fire_piece[1].x = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i]].fire_piece[1].y = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i]].fire_piece[1].angle = 0.0;
			cp->state[pp2_legacy_state_table[i]].particle.animation = 1;
			cp->state[pp2_legacy_state_table[i]].particle.x = 0.0;
			cp->state[pp2_legacy_state_table[i]].particle.y = 0.0;
			cp->state[pp2_legacy_state_table[i]].particle.cx = cp->animation[1]->frame[0]->width / 2;
			cp->state[pp2_legacy_state_table[i]].particle.cy = cp->animation[1]->frame[0]->height / 2;
			cp->state[pp2_legacy_state_table[i]].particle.angle = 0.0;
		}
		else
		{
			cp->state[pp2_legacy_state_table[i - 12]].fire_piece[0].animation = i + 6;
			cp->state[pp2_legacy_state_table[i - 12]].fire_piece[0].x = 0.0;
			cp->state[pp2_legacy_state_table[i - 12]].fire_piece[0].y = 0.0;
			cp->state[pp2_legacy_state_table[i - 12]].fire_piece[0].cx = 0.0;
			cp->state[pp2_legacy_state_table[i - 12]].fire_piece[0].cy = 0.0;
			cp->state[pp2_legacy_state_table[i - 12]].fire_piece[0].angle = 0.0;
			cp->state[pp2_legacy_state_table[i - 12]].fire_pieces = 2;
			
			cp->state[pp2_legacy_state_table[i - 12]].collision_x = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i - 12]].collision_y = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i - 12]].collision_w = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i - 12]].collision_h = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i - 12]].paintball.animation = 0;
			cp->state[pp2_legacy_state_table[i - 12]].paintball.x = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i - 12]].paintball.y = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i - 12]].paintball.cx = cp->animation[0]->frame[0]->width / 2;
			cp->state[pp2_legacy_state_table[i - 12]].paintball.cy = cp->animation[0]->frame[0]->height / 2;
//			cp->state[pp2_legacy_state_table[i - 12]].paintball.x -= cp->state[pp2_legacy_state_table[i - 12]].paintball.cx;
//			cp->state[pp2_legacy_state_table[i - 12]].paintball.y -= cp->state[pp2_legacy_state_table[i - 12]].paintball.cy;
			cp->state[pp2_legacy_state_table[i - 12]].paintball.angle = 0.0;
			cp->state[pp2_legacy_state_table[i - 12]].fire_piece[1].animation = pp2_flash_table[i] + 2;
			cp->state[pp2_legacy_state_table[i - 12]].fire_piece[1].x = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i - 12]].fire_piece[1].y = al_fread32le(fp) * 2;
			cp->state[pp2_legacy_state_table[i - 12]].fire_piece[1].angle = 0.0;
			cp->state[pp2_legacy_state_table[i - 12]].particle.animation = 1;
			cp->state[pp2_legacy_state_table[i - 12]].particle.x = 0.0;
			cp->state[pp2_legacy_state_table[i - 12]].particle.y = 0.0;
			cp->state[pp2_legacy_state_table[i - 12]].particle.cx = cp->animation[1]->frame[0]->width / 2;
			cp->state[pp2_legacy_state_table[i - 12]].particle.cy = cp->animation[1]->frame[0]->height / 2;
			cp->state[pp2_legacy_state_table[i - 12]].particle.angle = 0.0;
		}
	}
	
	mix = al_fread32le(fp);
	miy = al_fread32le(fp);
	mir = al_fread32le(fp);
	ix = al_fread32le(fp);
	iy = al_fread32le(fp);
	cp->paintball_size = al_fread32le(fp) * 2;
	cp->flags = PP2_CHARACTER_FLAG_LEGACY;
	
	if(flags)
	{
		cp->atlas = t3f_create_atlas(1024, 1024);
		if(cp->atlas)
		{
			for(i = 0; i < cp->animations; i++)
			{
				t3f_add_animation_to_atlas(cp->atlas, cp->animation[i], T3F_ATLAS_SPRITE);
			}
		}
	}
	
	return cp;
}

PP2_CHARACTER * pp2_load_legacy_character(const char * fn, int flags)
{
	ALLEGRO_FILE * fp;
	PP2_CHARACTER * cp;
	char fn_copy[1024] = {0};

	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		return NULL;
	}
	cp = pp2_load_legacy_character_f(fp, flags);
	al_fclose(fp);
	if(t3f_flags & T3F_USE_SOUND)
	{
		strcpy(fn_copy, fn);
		pp2_character_replace_extension(fn_copy, "pps");
		pp2_legacy_load_sounds(cp, fn_copy);
	}
	return cp;
}
