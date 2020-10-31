#include "t3f/t3f.h"

void pp2_legacy_put_pixel(int x, int y, ALLEGRO_COLOR color, int scale)
{
  int i, j;

  for(i = 0; i < scale; i++)
  {
    for(j = 0; j < scale; j++)
    {
      al_put_pixel(x * scale + j, y * scale + i, color);
    }
  }
}
