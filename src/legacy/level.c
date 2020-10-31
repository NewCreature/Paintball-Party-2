#include <allegro5/allegro5.h>
#include <stdio.h>
#include "../gameplay/sprites/object_defines.h"
#include "../file/level.h"
#include "../misc/fixed_point.h"
#include "animation.h"
#include "palette.h"

static int pp2_legacy_il = 0;
static int pp2_legacy_cx = 0;
static int pp2_legacy_cy = 0;
static int pp2_legacy_cb = 0;
static int pp2_legacy_cr = 0;
static char pp2_legacy_tile_used[256] = {0};
static bool pp2_legacy_is_il = false;
static int pp2_legacy_current_tile = 0;

static T3F_TILE * pp2_legacy_load_tile_fp(ALLEGRO_FILE * fp, const char * fn)
{
    char header[4]; /* identifier buffer */
    int i;       /* looping variables */
    int fr;
    T3F_TILE * tp;

    /* load the header */
    al_fread(fp, header, 4);
    if(header[0] != 'T' || header[1] != 'I' ||header[2] != 'L' || header[3] != 21)
    {
        return NULL;
    }

    tp = t3f_create_tile();

    /* load dimensions */
    al_fread16le(fp);
    al_fread16le(fp);

    /* load flags */
    tp->flags = 0;
    for(i = 0; i < 50; i++)
    {
	    fr = al_fread32le(fp);
        tp->flags |= fr ? (1 << i) : 0;

        if(i == 4)
        {
	        tp->user_data[0] = fr;
        }
        else if(i == 5)
        {
	        tp->user_data[1] = fr;
        }
        else if(i == 11)
        {
			tp->user_data[2] = fr << 1;
		}
    }
    if(tp->flags & (1 << 7))
    {
      pp2_legacy_set_color_type(PP2_LEGACY_COLOR_TRANS);
    }
    else if(tp->flags & (1 << 12))
    {
      pp2_legacy_set_color_type(PP2_LEGACY_COLOR_LIGHT);
    }
    else
    {
      pp2_legacy_set_color_type(PP2_LEGACY_COLOR_SOLID);
    }

    /* create and load image */
    tp->ap = pp2_legacy_load_ani_fp(fp, fn, NULL);

    /* return success */
    return tp;
}

static T3F_TILESET * pp2_legacy_load_tileset_fp(ALLEGRO_FILE * fp, const char * fn)
{
    char header[3]; /* tileset identifier */
    int i;       /* looping variables */
    T3F_TILESET * tsp;
    int r, g, b;
	ALLEGRO_STATE old_state;

    /* load the header */
    al_fread(fp, header, 3);
    if(header[0] != 'T' || header[1] != 'S' || header[2] != 21)
    {
        return NULL;
    }

    tsp = t3f_create_tileset(32, 32);
	al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP);
	al_restore_state(&old_state);

    /* load the palette */
    for(i = 0; i < 256; i++)
    {
        r = al_fgetc(fp);
        g = al_fgetc(fp);
        b = al_fgetc(fp);
        pp2_legacy_set_color(i, al_map_rgba_f((float)r / 63.0, (float)g / 63.0, (float)b / 63.0, i > 0 ? 1.0 : 0.0));
    }

    /* load tileset dimensions */
    al_fread16le(fp);
    al_fread16le(fp);

    /* load the tile image data */
    pp2_legacy_current_tile = 0;
    for(i = 0; i < 256; i++)
    {
        tsp->tile[i] = pp2_legacy_load_tile_fp(fp, fn);
    }
    tsp->tiles = 256;
    tsp->width = 32;
    tsp->height = 32;

    /* return success */
    return tsp;
}

static int pp2_legacy_load_level_data_fp(PP2_LEVEL * lp, ALLEGRO_FILE * fp)
{
	int i, j;
	int a;

	if(fp && lp)
	{
        lp->object = malloc(sizeof(PP2_LEVEL_OBJECT) * 256);
		lp->objects = 0;
		for(i = 0; i < 256; i++)
		{
			lp->object[lp->objects].type = al_fread32le(fp);
			lp->object[lp->objects].x = al_fread32le(fp) * 2;
			lp->object[lp->objects].y = al_fread32le(fp) * 2;
//			printf("type %d\n", lp->object[lp->objects].type);
			a = al_fread32le(fp);
			lp->object[lp->objects].flags = 0;
			for(j = 0; j < 16; j++)
			{
				lp->object[lp->objects].flags |= (al_fread32le(fp) ? (1 << j) : 0);
			}
			if(a)
			{
				lp->objects++;
			}
		}
		return 1;
	}
	return 0;
}

static T3F_TILEMAP_LAYER * pp2_legacy_load_tilemap_layer_fp(ALLEGRO_FILE * fp)
{
    char header[4];
    int i, j;
    int tw, th;
    T3F_TILEMAP_LAYER * tlp;

    al_fread(fp, header, 4);
    if(header[0] != 'T' || header[1] != 'M' || header[2] != 'L' || header[3] != 23)
    {
        return NULL;
    }

    tw = al_fread16le(fp);
    th = al_fread16le(fp);
    tlp = t3f_create_tilemap_layer(tw, th);
    if(!tlp)
    {
	    return NULL;
    }
    tlp->speed_x = pp2_fixtof(al_fread32le(fp));
    tlp->speed_y = pp2_fixtof(al_fread32le(fp));
    tlp->flags = 0;
    for(i = 0; i < 50; i++)
    {
//        tlp->flags |= al_fread16le(fp) ? 1 << i : 0;
        al_fread16le(fp);
    }

    for(i = 0; i < th; i++)
    {
        for(j = 0; j < tw; j++)
        {
            tlp->data[i][j] = al_fgetc(fp);

            /* tile is the interact layer, mark so we can generate tile reader objects */
            if(pp2_legacy_is_il)
            {
            	pp2_legacy_tile_used[tlp->data[i][j]] = 1;
        	}
        }
    }

    return tlp;
}

static T3F_TILEMAP * pp2_legacy_load_tilemap_fp(ALLEGRO_FILE * fp)
{
    char header[3];
    int i;
    T3F_TILEMAP * tmp;
    int w, h, l;

    /* read the ID and check it */
    al_fread(fp, header, 3);
    if(header[0] != 'T' || header[1] != 'M' || header[2] != 21)
    {
        return NULL;
    }

    /* allocate memory */
    tmp = (T3F_TILEMAP *)malloc(sizeof(T3F_TILEMAP));
    memset(tmp, 0, sizeof(T3F_TILEMAP));

    /* read dimensions */
    w = al_fread16le(fp);
    h = al_fread16le(fp);
    l = al_fread16le(fp);

    /* read clipping data */
    pp2_legacy_il = al_fread16le(fp);
    pp2_legacy_cx = al_fread32le(fp);
    pp2_legacy_cy = al_fread32le(fp);
    pp2_legacy_cb = al_fread32le(fp);
    pp2_legacy_cr = al_fread32le(fp);

    /* read layers */
    for(i = 0; i < l; i++)
    {
	    if(i == pp2_legacy_il)
	    {
		    pp2_legacy_is_il = true;
	    }
	    else
	    {
		    pp2_legacy_is_il = false;
	    }
        tmp->layer[i] = pp2_legacy_load_tilemap_layer_fp(fp);
        if(!tmp->layer[i])
        {
	        return NULL;
        }
    }
    tmp->layers = l;
	tmp->layer[0]->flags |= T3F_TILEMAP_LAYER_SOLID;

    return tmp;
}

static PP2_LEVEL * pp2_load_legacy_level_f(ALLEGRO_FILE * fp, const char * fn, int flags)
{
	PP2_LEVEL * lp;
	int i, j, k, l;

	memset(pp2_legacy_tile_used, 0, 256);
	lp = pp2_create_level();
    if(!lp)
    {
        return NULL;
    }
	lp->tileset = pp2_legacy_load_tileset_fp(fp, fn);
	if(!lp->tileset)
	{
		free(lp);
		return NULL;
	}
	lp->tilemap = pp2_legacy_load_tilemap_fp(fp);
	if(!lp->tilemap)
	{
		return NULL;
	}

	/* finish up */
	pp2_legacy_load_level_data_fp(lp, fp);
	for(i = 0; i < lp->objects; i++)
	{
		lp->object[i].layer = pp2_legacy_il;
	}
	lp->room.x = pp2_legacy_cx;
	lp->room.y = pp2_legacy_cy;
	lp->room.bx = pp2_legacy_cr;
	lp->room.by = pp2_legacy_cb;

	/* construct collision tilemap from tilemap data */
	lp->collision_tilemap[pp2_legacy_il] = t3f_create_collision_tilemap(lp->tilemap->layer[pp2_legacy_il]->width, lp->tilemap->layer[pp2_legacy_il]->height, 32, 32);
	for(i = 0; i < lp->collision_tilemap[pp2_legacy_il]->height; i++)
	{
		for(j = 0; j < lp->collision_tilemap[pp2_legacy_il]->width; j++)
		{
			if(lp->tileset->tile[lp->tilemap->layer[pp2_legacy_il]->data[i][j]]->flags & 1)
			{
				lp->collision_tilemap[pp2_legacy_il]->data[i][j].flags |= T3F_COLLISION_FLAG_SOLID_TOP;
			}
			if(lp->tileset->tile[lp->tilemap->layer[pp2_legacy_il]->data[i][j]]->flags & 2)
			{
				lp->collision_tilemap[pp2_legacy_il]->data[i][j].flags |= T3F_COLLISION_FLAG_SOLID_BOTTOM;
			}
			if(lp->tileset->tile[lp->tilemap->layer[pp2_legacy_il]->data[i][j]]->flags & 4)
			{
				lp->collision_tilemap[pp2_legacy_il]->data[i][j].flags |= T3F_COLLISION_FLAG_SOLID_LEFT;
			}
			if(lp->tileset->tile[lp->tilemap->layer[pp2_legacy_il]->data[i][j]]->flags & 8)
			{
				lp->collision_tilemap[pp2_legacy_il]->data[i][j].flags |= T3F_COLLISION_FLAG_SOLID_RIGHT;
			}
			if(lp->tileset->tile[lp->tilemap->layer[pp2_legacy_il]->data[i][j]]->flags & 128)
			{
				lp->collision_tilemap[pp2_legacy_il]->data[i][j].flags |= PP2_LEVEL_COLLISION_FLAG_ICE;
			}
			if(lp->tileset->tile[lp->tilemap->layer[pp2_legacy_il]->data[i][j]]->flags & 2048)
			{
				lp->collision_tilemap[pp2_legacy_il]->data[i][j].flags |= PP2_LEVEL_COLLISION_FLAG_CONVEYOR;
				lp->collision_tilemap[pp2_legacy_il]->data[i][j].user_data = malloc(sizeof(int));
				lp->collision_tilemap[pp2_legacy_il]->data[i][j].user_data[0] = lp->tileset->tile[lp->tilemap->layer[pp2_legacy_il]->data[i][j]]->user_data[2]; // fix me
			}
		}
	}

	/* construct tile animations */
	int ct;
	for(i = 0; i < lp->tileset->tiles; i++)
	{
		lp->tileset->tile[i]->user_data[15] = lp->tileset->tile[i]->flags;
	}
	for(i = 0; i < lp->tileset->tiles; i++)
	{
		if(lp->tileset->tile[i]->user_data[0] != 0 && lp->tileset->tile[i]->user_data[0] != i && lp->tileset->tile[i]->user_data[1] != 0)
		{
			bool used[T3F_MAX_TILES] = {false};
			int oflags = lp->tileset->tile[i]->user_data[15] & 15;
			ct = i;
			lp->tileset->tile[i]->flags = 0; // clear temporary tile flags
			while(1)
			{
				for(j = 0; j < lp->tileset->tile[ct]->user_data[1]; j++)
				{
					if(lp->tileset->tile[i]->frame_list_total < 1024)
					{
						lp->tileset->tile[i]->frame_list[lp->tileset->tile[i]->frame_list_total] = ct;
						lp->tileset->tile[i]->frame_list_total++;
					}
				}
				used[ct] = true;
				ct = lp->tileset->tile[ct]->user_data[0];
				if(used[ct])
				{
					break;
				}
				if(ct == i)
				{
					break;
				}

				/* this tile has changing collision properties */
				if(oflags != (lp->tileset->tile[ct]->user_data[15] & 15))
				{
					pp2_legacy_tile_used[i] = 2;
//					printf("dynamic collision\n");
				}

				if(j == 0 || lp->tileset->tile[ct]->user_data[0] == ct)
				{
					lp->tileset->tile[i]->flags |= T3F_TILE_FLAG_ONCE;
					break;
				}
			}
			lp->tileset->tile[i]->flags |= T3F_TILE_FLAG_ANIMATED;
		}
	}
	if(flags)
	{
		t3f_atlas_tileset(lp->tileset);
	}

    /* allocate tile reader objects */
    l = lp->objects;
    for(i = 0; i < lp->tilemap->layer[pp2_legacy_il]->height; i++)
	{
		for(j = 0; j < lp->tilemap->layer[pp2_legacy_il]->width; j++)
		{
			for(k = 0; k < 256; k++)
			{
				if(pp2_legacy_tile_used[k] == 2 && lp->tilemap->layer[pp2_legacy_il]->data[i][j] == k)
				{
                    lp->objects++;
				}
			}
		}
	}
    lp->object = realloc(lp->object, sizeof(PP2_LEVEL_OBJECT) * lp->objects);
    if(!lp->object)
    {
        return NULL;
    }

	/* add tile reader objects */
    for(i = 0; i < lp->tilemap->layer[pp2_legacy_il]->height; i++)
	{
		for(j = 0; j < lp->tilemap->layer[pp2_legacy_il]->width; j++)
		{
			for(k = 0; k < 256; k++)
			{
				if(pp2_legacy_tile_used[k] == 2 && lp->tilemap->layer[pp2_legacy_il]->data[i][j] == k)
				{
					lp->object[l].type = PP2_OBJECT_TILE_READER;
					lp->object[l].x = j * 32;
					lp->object[l].y = i * 32;
					lp->object[l].layer = pp2_legacy_il;
					lp->object[l].flags = 0;
                    l++;
				}
			}
		}
	}
    lp->flags = PP2_LEVEL_FLAG_LEGACY;

	return lp;
}

PP2_LEVEL * pp2_load_legacy_level(const char * fn, int flags)
{
	ALLEGRO_FILE * fp;
	PP2_LEVEL * lp;

	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		return NULL;
	}
	lp = pp2_load_legacy_level_f(fp, fn, flags);
	al_fclose(fp);
	return lp;
}
