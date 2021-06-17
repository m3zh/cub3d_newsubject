/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlazzare <mlazzare@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 07:43:16 by mlazzare          #+#    #+#             */
/*   Updated: 2021/06/11 07:30:55 by mlazzare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	check_map(t_map *map, int err)
{
	if (err)
		return (-1);
	if (!BNS && map->sprites > 0)
		return (-get_error('m'));
	if ((!BNS && map->complete != 6) || (BNS && map->complete != 7))
		return (-get_error('i'));
	if (map->x < 0)
		return (-get_error('p'));
	return (1);
}

static int	wrong_line_setup(t_map *map)
{
	if (!map->line)
		return (get_error('l'));
	map->trim = trimspaces(map->line);
	if (!map->trim)
		return (get_error('l'));
	return (0);
}

static int	fill_map(t_map *map)
{
	if (wrong_line_setup(map))
		return (1);
	if (map->trim[0] == 'N' && !map->no)
		return (get_path(map, map->trim));
	else if (map->trim[0] == 'S' && map->trim[1] == 'O' && !map->so)
		return (get_path(map, map->trim));
	else if (map->trim[0] == 'W' && !map->we)
		return (get_path(map, map->trim));
	else if (map->trim[0] == 'E' && !map->ea)
		return (get_path(map, map->trim));
	else if (BNS && map->trim[0] == 'S' && map->trim[1] != 'O' && !map->s)
		return (get_sprite_path(map, map->trim));
	else if (map->trim[0] == 'C' && !map->c_check)
		return (get_rgb_value(map, map->c, map->line));
	else if (map->trim[0] == 'F' && !map->f_check)
		return (get_rgb_value(map, map->f, map->line));
	else if (map_checked(map))
		return (get_maze(map));
	else
	{
		free(map->trim);
		map->trim = NULL;
		return (get_error('x'));
	}
	return (0);
}

static int	check_lastline(t_map *map, int space)
{
	if (!space && check_bottom_wall(map, map->maze[map->idx - 1]))
		return (get_error('w'));
	map->maze[map->idx] = ft_strdup("");
	if (!map->maze[map->idx])
		return (1);
	map->idx++;
	map->maze[map->idx] = 0;
	return (0);
}

static int	read_line(int fd, int gnl, int err, t_map *map)
{
	int	space;

	space = 0;
	while (fd && gnl)
	{
		gnl = get_next_line(fd, &map->line, err);
		if (err)
			break ;
		if (space && !ft_strcmp(map->line, ""))
			err = check_lastline(map, space);
		else if (map->line && ft_strcmp(map->line, ""))
		{
			err = fill_map(map);
			if (!err && space)
				err = check_top_wall(map);
			space = 0;
		}
		else if (gnl && map->line && map->idx > 0 && !ft_strcmp(map->line, ""))
		{
			err = check_lastline(map, space);
			space = 1;
		}
		if (map->line)
			free(map->line);
		map->line = NULL;
	}
	if (!err && !space && check_bottom_wall(map, map->maze[map->idx - 1]))
		return (get_error('w'));
	return (err);
}

int	read_map(t_map *map, char **av)
{
	int		fd;
	int		err;
	int		gnl;

	err = 0;
	gnl = 1;
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
		return (-get_error('o'));
	err = read_line(fd, gnl, err, map);
	if (close(fd) < 0)
		return (-1);
	return (check_map(map, err));
}
