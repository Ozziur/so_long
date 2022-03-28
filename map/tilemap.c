/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilemap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 13:57:28 by mruizzo           #+#    #+#             */
/*   Updated: 2022/03/25 16:52:09 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int		ft_chartable_linecount(char **table);

t_tile	**alloc_tilemap(char **map)
{
	t_tile	**tilemap;
	int		i;

	tilemap = ft_calloc(ft_chartable_linecount(map) + 1, sizeof(t_tile *));
	if (tilemap == NULL)
	{
		ft_printf("Error\nAllocazione fallita in alloc_tilemap\n");
		return (NULL);
	}
	i = 0;
	while (map[i] != NULL)
	{
		tilemap[i] = ft_calloc(ft_strlen(*map) + 1, sizeof(t_tile));
		if (tilemap == NULL)
		{
			while (i > 0)
				free(tilemap[--i]);
			ft_printf("Error\nAllocazione fallita in alloc_tilemap\n");
			return (NULL);
		}
		i++;
	}
	return (tilemap);
}

t_tiletype	define_tiletype(char definer)
{
	if (definer == '1')
		return (WALL);
	if (definer == 'C')
		return (COLLECTABLE);
	if (definer == 'P')
		return (PLAYER);
	if (definer == 'E')
		return (EXIT);
	if (definer == 'H' || definer == 'V')
		return (ENEMY);
	return (EMPTY);
}

void	setup_tile(t_tile **tilemap, int x, int y)
{
	tilemap[y][x].position.x = x * IMG_SIZE;
	tilemap[y][x].position.y = y * IMG_SIZE;
	if (y != 0)
		tilemap[y][x].up = &tilemap[y - 1][x];
	if (tilemap[y + 1] != NULL)
		tilemap[y][x].down = &tilemap[y + 1][x];
	if (x != 0)
		tilemap[y][x].left = &tilemap[y][x - 1];
	tilemap[y][x].right = &tilemap[y][x + 1];
}

void	set_gamevars(t_tile *tile, t_game *game, char c)
{
	if (tile->type == PLAYER)
		game->player.tile = tile;
	else if (tile->type == COLLECTABLE)
		game->collects++;
	else if (tile->type == ENEMY)
		add_enemy (game, c, tile);
}

t_tile	**generate_tilemap(char **map, t_game *game)
{
	t_tile	**tilemap;
	int		x;
	int		y;

	tilemap = alloc_tilemap(map);
	if (!tilemap)
		return (NULL);
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x] != 0)
		{
			tilemap[y][x].type = define_tiletype(map[y][x]);
			setup_tile(tilemap, x, y);
			set_gamevars(&tilemap[y][x], game, map[y][x]);
			x++;
		}
		y++;
	}
	game->wndw_size.x = x * IMG_SIZE;
	game->wndw_size.y = y * IMG_SIZE;
	return (tilemap);
}
