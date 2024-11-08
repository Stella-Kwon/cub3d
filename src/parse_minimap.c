/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_minimap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 10:43:31 by jmakkone          #+#    #+#             */
/*   Updated: 2024/11/08 16:54:18 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#define MINIMAP_SIZE 200  // 미니맵 SQUARE SIZE
#define PLAYER_CENTER 100 // 미니맵에서 플레이어의 중앙 위치
#define MINIMAP_SCALE 10 // 미니맵 스케일 크기를 고정해줘야댐

void draw_player_to_minimap(t_caster *c)
{
	// draw_minimap(c, c->mmap_px, c->mmap_py, 3);
	printf("Player pos X: %f Y: %f\n", c->mmap_px, c->mmap_py);
	printf("speed_multiplier: %f\n", c->speed_multiplier);
}

void draw_minimap(t_caster *c, int x, int y, int flag)
{
	int i;
	int j;
	uint32_t color;
	mlx_image_t *window;

	if (flag == 1)
		color = 0x0000FFFF;
	if (flag == 0)
		color = 0xFFFFFFFF; // 벽은 검은색, 빈 공간은 흰색
	if (flag == 2)
	{
		color = 0xFF0000FF;
		// c->map->scale_y = 5;
		// c->map->scale_x = 5;
		window = c->window->minimap; //CALL THE MINIMAP IMAGE FORMAT TO WINDOW
		i = -1;
		while (++i <5)
		{
			j = -1;
			while (++j <5)
			{
				// mlx_put_pixel(window, x + j, y + i, color); // EACH TILE GETS TO BE FILLED
				if (x + j < (int) window->width && y + i < (int) window->height)
					mlx_put_pixel(window, x + j, y + i, color);
			}
		}
		return ;
	}
	// if (flag == 4)
	// 	color = 0xFF0000;
	window = c->window->minimap; //CALL THE MINIMAP IMAGE FORMAT TO WINDOW
	i = -1;
	while (++i < c->map->scale_y)
	{
		j = -1;
		while (++j < c->map->scale_x)
		{
			// mlx_put_pixel(window, x + j, y + i, color); // EACH TILE GETS TO BE FILLED
			if (x + j < (int) window->width && y + i < (int) window->height)
				mlx_put_pixel(window, x + j, y + i, color);
		}
	}
	
}

// 미니맵을 그릴 때 사용될 오프셋을 계산하고 화면 크기 내에서만 그리기

void parse_minimap(t_caster *c)
{
	// 원본 맵을 축소한 미니맵을 전체로 그리기 위해 스케일 계산
	c->map->scale_x = MINIMAP_SCALE; //1보다 커야댐
	c->map->scale_y = MINIMAP_SCALE;
	// c->map->scale_x = MINIMAP_SIZE / c->map->map_width;
	// c->map->scale_y = MINIMAP_SIZE / c->map->map_height;

	printf("scale : %f\n", c->map->scale_x);
	int i = 0;
	mlx_image_t *window = c->window->minimap;
	while (i < MINIMAP_SIZE) // height
	{
		int j = 0;
		while (j < MINIMAP_SIZE)
		{
			// 맵이 미니맵을 벗어난 경우 다른 색으로 칠함
			// if (i >= c->map->map_height || j >= c->map->map_width)
			// {
				mlx_put_pixel(window, j, i,  0xFFFFFF);
			// }
			j++;
		}
		i++;
	}
	// 플레이어가 미니맵 중앙에 위치하도록 카메라 오프셋 계산 
	//즉 camera offset
	// int로 해준이유는 소수값정리하기위해서 +0.201인가 넣어줌// player_center을 빼주면서
	int camera_offset_x = (int)(c->px * c->map->scale_x) - PLAYER_CENTER;
	int camera_offset_y = (int)(c->py * c->map->scale_y) - PLAYER_CENTER;

	// 미니맵의 범위 제한 (오프셋이 미니맵 경계를 넘지 않도록)
	if (c->map->map_width * c->map->scale_x < MINIMAP_SIZE)
	{
		// 맵의 크기가 미니맵보다 작은 경우, 중앙 정렬
		camera_offset_x = (c->map->map_width * c->map->scale_x - MINIMAP_SIZE) / 2;
	}
	else if (camera_offset_x > (c->map->map_width * c->map->scale_x) - MINIMAP_SIZE)
	{
		// 넘지않도록 미니맵윈도우크기 , 오프셋 조정
		camera_offset_x = (c->map->map_width * c->map->scale_x) - MINIMAP_SIZE;
	}
	if (c->map->map_height * c->map->scale_y < MINIMAP_SIZE)
	{
		camera_offset_y = (c->map->map_height * c->map->scale_y - MINIMAP_SIZE) / 2;
	}
	else if (camera_offset_y > (c->map->map_height * c->map->scale_y) - MINIMAP_SIZE)
	{
		camera_offset_y = (c->map->map_height * c->map->scale_y) - MINIMAP_SIZE;
	}

	int y = 0;
	while (y < c->map->map_height && c->map->map_arr[y])
	{
		int x = 0;
		while (x < c->map->map_width && c->map->map_arr[y][x])
		{
			int minimap_x = (int)(x * c->map->scale_x) - camera_offset_x;
			int minimap_y = (int)(y * c->map->scale_y) - camera_offset_y;

			if (minimap_x < 0)
				minimap_x = 0;
			else if (minimap_x >= MINIMAP_SIZE)
				minimap_x = MINIMAP_SIZE - 1;

			if (minimap_y < 0)
				minimap_y = 0;
			else if (minimap_y >= MINIMAP_SIZE)
				minimap_y = MINIMAP_SIZE - 1;

			// printf("map[%d][%d] : %d\n", y, x, c->map->map_arr[y][x]);
			char tile = c->map->map_arr[y][x];
			if (tile == '1')
				draw_minimap(c, minimap_x, minimap_y, 1); // 벽 타일
			else if (tile == '0')
				draw_minimap(c, minimap_x, minimap_y, 0); // 빈 공간
			x++;
		}
		y++;
	}
	int player_x = c->mmap_px - camera_offset_x;
	int player_y =  c->mmap_py  - camera_offset_y;
	if (player_x < 0)
		player_x = 0;
	else if (player_x >= MINIMAP_SIZE)
		player_x = MINIMAP_SIZE - 1;
	if (player_y < 0)
		player_y = 0;
	else if (player_y >= MINIMAP_SIZE)
		player_y = MINIMAP_SIZE - 1;
	printf("player_x : %d\n player_y : %d\n", player_x,  player_y);
	// int map_x = (int)(c->px);
	// int map_y = (int)(c->py);
	// if (map_x >= 0 && map_x < c->map->map_width && map_y >= 0 && map_y < c->map->map_height)
	// {
	// 	printf("map_arr[%d][%d] : %c\n", map_y, map_x, c->map->map_arr[map_y][map_x]);
	// 	if (c->map->map_arr[map_y][map_x] == '1')
	// 	{
	// 		// MAKE THE PLAYER IN SCALESIZE AND BY PUTTING CAMERA_OFFSET TO MAKE THE PLAYE CONSISTENTLY STAYS IN TH MIDDLE
	// 		player_x = c->mmap_px - camera_offset_x;
	// 		player_y =  c->mmap_py  - camera_offset_y;

	// 		// SET PLAYER MOVEMENTS INSIDE OF THE MAP
	// 		if (player_x < 0)
	// 			player_x = 0;
	// 		else if (player_x >= MINIMAP_SIZE)
	// 			player_x = MINIMAP_SIZE - 1;
	// 		if (player_y < 0)
	// 			player_y = 0;
	// 		else if (player_y >= MINIMAP_SIZE)
	// 			player_y = MINIMAP_SIZE - 1;
	// 		printf(" in the wall :: player_x : %d\n player_y : %d\n", player_x,  player_y);
	// 	// Adjust player position to not be drawn on a wall
	// 	//if player position corresponds to a wall
	// 		// if (player_x > 0 && c->map->map_arr[map_y][map_x - 1] != '1') 
	// 		// {
	// 		// 	player_x = (int)((c->px - 1) * c->map->scale_x) - camera_offset_x;
	// 		// }
	// 		// else if (player_x < MINIMAP_SIZE - 1 && c->map->map_arr[map_y][map_x + 1] != '1') 
	// 		// {
	// 		// 	player_x = (int)((c->px + 1) * c->map->scale_x) - camera_offset_x;
	// 		// }
	// 		// if (player_y > 0 && c->map->map_arr[map_y - 1][map_x] != '1') 
	// 		// {
	// 		// 	player_y = (int)((c->py - 1) * c->map->scale_y) - camera_offset_y;
	// 		// }
	// 		// else if (player_y < MINIMAP_SIZE - 1 && c->map->map_arr[map_y + 1][map_x] != '1')
	// 		// {
	// 		// 	player_y = (int)((c->py + 1 )* c->map->scale_y) - camera_offset_y;
	// 		// }
	// 	}

	// }
	// else
	// {
	// 	printf("Invalid map indices: map_x = %d, map_y = %d\n", map_x, map_y);
	// }
	draw_minimap(c, player_x, player_y, 2);
	// double distance = 50;  // Distance to extend the FOV
	// int left_x = c->ray_dir_x * distance;
	// int left_y = c->ray_dir_y * distance;
	// int right_x = c->ray_dir_x * distance;
	// int right_y = c->ray_dir_y * distance;
	// draw_minimap(c, left_x, left_y, 4);
	// draw_minimap(c, right_x, right_y, 4);
	// draw_minimap(c, PLAYER_CENTER, PLAYER_CENTER, 2);
}
