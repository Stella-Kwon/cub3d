/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_description.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:00:52 by skwon2            #+#    #+#             */
/*   Updated: 2024/11/25 15:39:41 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

void	print_map(t_caster *c)
{
	int	y;
	int	x;

	y = 0;
	while (y < c->map->map_height && c->map->map_arr[y])
	{
		x = 0;
		while (x < c->map->map_width && c->map->map_arr[y][x])
		{
			printf("%c", c->map->map_arr[y][x]);
			x++;
		}
		printf("\n");
		y++;
	}
	printf("\n");
}

int	whole_space_line(char *str)
{
	while (*str)
	{
		if (*str != ' ' && !(*str >= 9 && *str <= 13))
			return (false);
		str++;
	}
	return (true);
}

void	check_map(t_caster *c)
{
	char	*line;
	t_dir	i;

	i = NO;
	line = get_next_line(c->map->map_fd);
	if (!line)
		exit_failure(c, "Empty description.");
	while (line)
	{
		if (whole_space_line(line))
		{
			if (i >= end && c->map->map_arr && c->map->map_arr[0])
				exit_failure(c, "Newline in between the map.");
			if (line)
				free_and_null((void **)&line);
			line = get_next_line(c->map->map_fd);
			continue ;
		}
		process_line(c, &line, &i);
		if (line)
			free_and_null((void **)&line);
		line = get_next_line(c->map->map_fd);
	}
}

void	read_description(t_caster *c)
{
	c->textures->north_texture = NULL;
	c->textures->south_texture = NULL;
	c->textures->west_texture = NULL;
	c->textures->east_texture = NULL;
	file_exist(c, c->map->map_path, ".cub", MAP);
	check_map(c);
	print_map(c);
	find_player_pos(c);
	check_wall(c);
	print_map(c);
}
