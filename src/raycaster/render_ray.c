/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakkone <jmakkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 19:16:47 by jmakkone          #+#    #+#             */
/*   Updated: 2024/11/08 19:32:59 by jmakkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

void	render_wall_column(t_caster *c, int x)
{
	int	y;
	int	pixel_pos;
	int	tex_index;

	y = c->draw_start;
	while (y < c->draw_end)
	{
		pixel_pos = (y - (-c->wall_height / 2 + HEIGHT / 2));
		c->tex_y = ((pixel_pos * TEXTURE_HEIGHT) / c->wall_height);
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

void	render_floor_and_ceiling(t_caster *c, int draw_end, int x)
{
	int	y_floor;
	int	y_ceiling;

	y_floor = draw_end;
	y_ceiling = HEIGHT - draw_end - 1;
	while (y_floor < HEIGHT && y_ceiling >= 0)
	{
		mlx_put_pixel(c->window->view, x, y_floor++, \
				c->textures->floor_color);
		mlx_put_pixel(c->window->view, x, y_ceiling--, \
				c->textures->ceiling_color);
	}
}
