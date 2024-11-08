/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls_keyboard.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:18:06 by jmakkone          #+#    #+#             */
/*   Updated: 2024/11/08 16:56:48 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static int check_collision(t_caster *c, double new_px, double new_py)
{
	int can_move_x;
	int can_move_y;

	can_move_x = 1;
	can_move_y = 1;
	if (c->map->map_arr[(int)(new_py)][(int)(new_px)] != '0')
		return (0);
	if (c->map->map_arr[(int)c->py][(int)(new_px + 0.15)] == '1' ||
		c->map->map_arr[(int)c->py][(int)(new_px - 0.15)] == '1')
		can_move_x = 0;
	if (c->map->map_arr[(int)(new_py + 0.15)][(int)c->px] == '1' ||
		c->map->map_arr[(int)(new_py - 0.15)][(int)c->px] == '1')
		can_move_y = 0;
	if (can_move_x)
		c->px = new_px;
	if (can_move_y)
		c->py = new_py;
	c->mmap_px = (c->px - 0.25) * c->map->scale_x + 0.25;
	c->mmap_py = (c->py - 0.25) * c->map->scale_y + 0.25;
	return (can_move_x || can_move_y);
}

int movement_up_down(t_caster *c)
{
	double new_px;
	double new_py;

	if (mlx_is_key_down(c->window->handle, MLX_KEY_W))
	{
		new_px = c->px + cos(c->view_angle) * c->speed_multiplier * 1.4;
		new_py = c->py + sin(c->view_angle) * c->speed_multiplier * 1.4;
		return (check_collision(c, new_px, new_py));
	}
	else if (mlx_is_key_down(c->window->handle, MLX_KEY_S))
	{
		new_px = c->px - cos(c->view_angle) * c->speed_multiplier;
		new_py = c->py - sin(c->view_angle) * c->speed_multiplier;
		return (check_collision(c, new_px, new_py));
	}
	return (0);
}

int movement_left_right(t_caster *c)
{
	double new_px;
	double new_py;

	if (mlx_is_key_down(c->window->handle, MLX_KEY_A))
	{
		new_px = c->px + sin(c->view_angle) * c->speed_multiplier;
		new_py = c->py - cos(c->view_angle) * c->speed_multiplier;
		return (check_collision(c, new_px, new_py));
	}
	else if (mlx_is_key_down(c->window->handle, MLX_KEY_D))
	{
		new_px = c->px - sin(c->view_angle) * c->speed_multiplier;
		new_py = c->py + cos(c->view_angle) * c->speed_multiplier;
		return (check_collision(c, new_px, new_py));
	}
	return (0);
}

static void keys_utils(mlx_key_data_t key, t_caster *c)
{
	if (key.key == MLX_KEY_ESCAPE)
	{
		exit_mlx(c);
		return;
	}
	else if (key.key == MLX_KEY_SPACE && key.action == MLX_PRESS)
		;
	else if (key.key == MLX_KEY_R && key.action == MLX_PRESS)
		;
}

void keyboard_listener(mlx_key_data_t key, void *param)
{
	t_caster *c;

	c = (t_caster *)param;
	keys_utils(key, c);
}
