/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 19:19:56 by msousa            #+#    #+#             */
/*   Updated: 2022/05/09 17:31:36 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// distance to wall
float	distance(t_point p, t_point q, float a)
{
	(void)a;
	return (sqrt((q.x - p.x) * (q.x - p.x) + (q.y - p.y) * (q.y - p.y)));
	// return ((q.x - p.x) / cos(a));
}

float	trim(float a)
{
	if (a < 0)
		a += 2 * PI;
	if (a > 2 * PI)
		a -= 2 * PI;
	return (a);
}

// void	raycasting3d(t_config *conf)
// {
// 	t_ray	ray;
// 	int		column;

// 	ray.angle = r_angle(30);
// 	column = 0;
// 	while (ray.angle > -r_angle(30) && column < WIN_W)
// 	{
// 		ray.dist_wall = magic_distance(conf, ray.angle);
// 		ray.end = add_vec(conf->pov->p,
// 				vec((ray.angle + conf->pov->angle), ray.dist_wall));
// 		ray.color = set_wall_color(ray.dist_wall,
// 				conf->pov->angle + ray.angle, conf);
// 		ray.height = ray.dist_wall * fcos(abs(ray.angle));
// 		ray.height = ((double)TILE_SIZE / ray.height) * conf->pov->dtp;
// 		draw_line(conf, ray, column);
// 		column++;
// 		ray.angle--;
// 	}
// }

// struct	s_ray
// {
// 	double	distance;
// 	t_point	end;
// 	t_wall	wall;
// 	int		height;
// 	double	angle;
// };

// sudo
// - vertical distance
// - horizontal distance
// - pick
// - pick wall
// - fish bowl
// - draw line

void	raycast(t_app *self)
{
	int r, mx, my, dof; // depth of field
	// mx: map x, my: map y
	float rx, ry, ra, xo, yo, distT;
	t_player	*p;
	t_settings	*s;
	char**		map;

	p = self->player;
	s = self->settings;
	map = s->map;
	ra = trim(p->a - (DR * WIDTH / 2));

	r = 0;
	while (r < WIDTH)
	{
		// check horizontal lines
		dof = 0;
		float	distH = 1000000, hx = p->x, hy = p->y;
		float	aTan = -1 / tan(ra);

		// First intersection

		// check direction up or down
		if (ra > PI) // looking up
		{
			ry = (((int)p->y >> BITS) << BITS) - 0.0001;
			rx = (p->y - ry) * aTan + p->x;
			yo = -SIZE;
			xo = -yo * aTan;
		}
		if (ra < PI) // looking down
		{
			ry = (((int)p->y >> BITS) << BITS) + SIZE;
			rx = (p->y - ry) * aTan + p->x;
			yo = SIZE;
			xo = -yo * aTan;
		}
		if (ra == 0 || ra == PI) // straight looking left or right
		{
			ry = p->y;
			rx = p->x;
			distT = 1000000;
			dof = s->height; // to not loop
		}

		// loop to find next
		while (dof < s->height)
		{
			mx = (int)(rx) >> BITS;
			my = (int)(ry) >> BITS;
			if (mx > 0 && my > 0 && mx < s->width
				&& my < s->height
				&& map[mx][my] == MAP_WALL)
			{
				hx = rx;
				hy = ry;
				distH = distance((t_point){p->x, p->y}, (t_point){hx, hy}, ra);
				dof = s->height; // end loop
			}
			else
			{
				rx += xo;
				ry += yo;
				dof++;
			}
		}

		// check vertical lines
		dof = 0;
		float	distV = 1000000, vx = p->x, vy = p->y;
		float	nTan = -tan(ra);

		// First intersection

		// check direction left or right
		if (ra > PI / 2 && ra < 3 * PI / 2) // looking left
		{
			rx = (((int)p->x >> BITS) << BITS) - 0.0001;
			ry = (p->x - rx) * nTan + p->y;
			xo = -SIZE;
			yo = -xo * nTan;
		}
		if (ra < PI / 2 || ra > 3 * PI / 2) // looking right
		{
			rx = (((int)p->x >> BITS) << BITS) + SIZE;
			ry = (p->x - rx) * nTan + p->y;
			xo = SIZE;
			yo = -xo * nTan;
		}
		if (ra == 0 || ra == PI) // straight looking up or down
		{
			ry = p->y;
			rx = p->x;
			distT = 1000000;
			dof = s->width;
		}

		// loop to find next
		while (dof < s->width)
		{
			mx = (int)(rx) >> BITS;
			my = (int)(ry) >> BITS;
			if (mx > 0 && my > 0 && mx < s->width
				&& my < s->height
				&& map[mx][my] == MAP_WALL)
			{
				vx = rx;
				vy = ry;
				distV = distance((t_point){p->x, p->y}, (t_point){vx, vy}, ra);
				dof = s->width; // end loop
			}
			else
			{
				rx += xo; // next line
				ry += yo;
				dof++;
			}
		}

		if (distV < distH)
		{
			rx = vx;
			ry = vy;
			distT = distV;
			g_wall_color = create_trgb(0, 100, 100, 100);
		}
		if (distV > distH)
		{
			rx = hx;
			ry = hy;
			distT = distH;
			g_wall_color = create_trgb(0, 120, 100, 100);
		}

		// Draw 3d rays

		// fish bowl
		float	ca;
		ca = trim(p->a - ra);
		distT = distT * cos(ca);

		int	lineH = (int)((SIZE * HEIGHT) / distT); // line height
		if (lineH > HEIGHT)
			lineH = HEIGHT;
		int	lineO = (int)((HEIGHT / 2) - (lineH / 2)); // line offset

		draw_line(self, r, lineH, lineO);

		ra = trim(ra + DR);
		r++;
	}
}
