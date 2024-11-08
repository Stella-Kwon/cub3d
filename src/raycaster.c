/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 10:54:48 by jmakkone          #+#    #+#             */
/*   Updated: 2024/11/08 14:03:04 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static void	init_ray_dir_and_cast_position(t_caster *c)
{
	c->ray_dir_x = cos(c->view_angle) + c->plane_x * c->view_offset;
	c->ray_dir_y = sin(c->view_angle) + c->plane_y * c->view_offset;
	c->dist_increment_x = fabs(1 / c->ray_dir_x);
	c->dist_increment_y = fabs(1 / c->ray_dir_y);
	if (c->ray_dir_x < 0)
	{
		c->step_x = -1;
		c->dist_to_grid_x = (c->px - c->map_x) * c->dist_increment_x;
	}
	else
	{
		c->step_x = 1;
		c->dist_to_grid_x = (c->map_x + 1.0 - c->px) * c->dist_increment_x;
	}
	if (c->ray_dir_y < 0)
	{
		c->step_y = -1;
		c->dist_to_grid_y = (c->py - c->map_y) * c->dist_increment_y;
	}
	else
	{
		c->step_y = 1;
		c->dist_to_grid_y = (c->map_y + 1.0 - c->py) * c->dist_increment_y;
	}
}

static void	trace_ray_until_wall_hit(t_caster *c)
{
	int hit;

	hit = 0;
	while (!hit)
	{
		if (c->dist_to_grid_x < c->dist_to_grid_y)
		{
			c->dist_to_grid_x += c->dist_increment_x;
			c->map_x += c->step_x;
			c->wall_hit_is_horizontal = 0;
		}
		else
		{
			c->dist_to_grid_y += c->dist_increment_y;
			c->map_y += c->step_y;
			c->wall_hit_is_horizontal = 1;
		}
		if (c->map_x >= c->map->map_width || c->map_y >= c->map->map_height)
			break;
		if (c->map->map_arr[c->map_y][c->map_x] == '1')
			hit = 1;
	}
}

static void	get_wall_dist_and_height(t_caster *c)
{
	if (c->wall_hit_is_horizontal == 0)
		c->wall_dist = (c->map_x - c->px + \
			(1 - c->step_x) / 2) / c->ray_dir_x;
	else
		c->wall_dist = (c->map_y - c->py + \
			(1 - c->step_y) / 2) / c->ray_dir_y;
	if (c->view_offset == 0)
		printf("Check wall_dist %f\n", c->wall_dist);
	c->wall_height = (int)(HEIGHT / c->wall_dist);
	c->draw_start = -c->wall_height / 2 + HEIGHT / 2;
	if (c->draw_start < 0)
		c->draw_start = 0;
	c->draw_end = c->wall_height / 2 + HEIGHT / 2;
	if (c->draw_end >= HEIGHT)
		c->draw_end = HEIGHT;
}
static void get_wall_texture(t_caster *c)
{
	if (c->wall_hit_is_horizontal == 0)
	{
		if (c->ray_dir_x > 0)
			c->wall_texture = c->textures->east_texture;
		else
			c->wall_texture = c->textures->west_texture;
	}
	else
	{
		if (c->ray_dir_y > 0)
			c->wall_texture = c->textures->south_texture;
		else
			c->wall_texture = c->textures->north_texture;
	}
}

static void	get_texture_offset(t_caster *c)
{
	double	wall_texture_offset;
	if (c->wall_hit_is_horizontal == 0)
		wall_texture_offset = c->py + c->wall_dist * c->ray_dir_y; //texture start point : where the palyer + walldistance * direction( & speed)
	else
		wall_texture_offset = c->px + c->wall_dist * c->ray_dir_x;// 오프셋은 벽에닿은 레이의 교차점위치를 수평이나 수직층에 해당하는 상대적위치로 계사나는 값.0 -1 상의 값이되므로
	wall_texture_offset -= floor(wall_texture_offset);
	c->tex_x = (int)(wall_texture_offset * (double)TEXTURE_WIDTH); //실제 텍스처의 픽셀좌표로 변환해야하기때문에 정확한 텍스쳐 픽셀의 위치에서 추출해 그려나가진다.
}

static void	render_wall_column(t_caster *c, int x)
{
	int y;
	int pixel_pos;
	int tex_index;

	y = c->draw_start;
	while (y < c->draw_end)
	{
		pixel_pos = (y - (-c->wall_height / 2 + HEIGHT / 2)); //시작이 0이되게하기위해서 //벽의 화면에서의 위치와 텍스처에서의 위치를 연관 짓기 위한 준비 :y는 렉더링한 픽셀의 위치
			c->tex_y = ((pixel_pos * TEXTURE_HEIGHT) / c->wall_height); //
		if (c->tex_y >= TEXTURE_HEIGHT)
			c->tex_y = TEXTURE_HEIGHT - 1;
		if (c->tex_y < 0)
			c->tex_y = 0;
		tex_index = (c->tex_y * TEXTURE_WIDTH + c->tex_x) * 4;
		c->pixel_color = (c->wall_texture->pixels[tex_index] << 24) \
			| (c->wall_texture->pixels[tex_index + 1] << 16) \
			| (c->wall_texture->pixels[tex_index + 2] << 8) \
			| c->wall_texture->pixels[tex_index + 3];
		mlx_put_pixel(c->window->view, x, y, c->pixel_color);
		y++;
	}
}

static void	render_floor_and_ceiling(t_caster *c, int draw_end, int x)
{
	int	y_floor;
	int	y_ceiling;
	
	y_floor = draw_end;
	y_ceiling = HEIGHT - draw_end - 1;
	while (y_floor < HEIGHT && y_ceiling >= 0)
	{
		mlx_put_pixel(c->window->view, x, y_floor++, c->textures->floor_color);
		mlx_put_pixel(c->window->view, x, y_ceiling--, c->textures->ceiling_color);
	}
}

void raycaster(t_caster *c)
{
	int x;


	x = -1;
	while (++x < WIDTH)
	{
		c->view_offset = 2 * x / (double)WIDTH - 1;
		c->map_x = (int)c->px;
		c->map_y = (int)c->py;
		init_ray_dir_and_cast_position(c);
		trace_ray_until_wall_hit(c);
		get_wall_dist_and_height(c);
		get_wall_texture(c);
		get_texture_offset(c);
		render_wall_column(c, x);
		render_floor_and_ceiling(c, c->draw_end, x);
	}
}
