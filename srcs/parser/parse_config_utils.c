/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlazzare <mlazzare@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 16:40:08 by mlazzare          #+#    #+#             */
/*   Updated: 2021/06/07 12:59:56 by mlazzare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	check_cf(char *line)
{
	return ((line[0] == 'F' || line[0] == 'C')
		&& ft_isspace(line[1]));
}

void	update_cf(t_map *map, char c)
{
	if (c == 'F')
		map->f_check = 1;
	else if (c == 'C')
		map->c_check = 1;
}

int	skip_space(char *l, int i)
{
	while (l[i] && ft_isspace(l[i]))
		i++;
	return (i);
}

int	check_spaces(int pos, t_map *map, char *l)
{
	if (l[pos - 1] != '1')
		return (-1);
	while (l[pos] && ft_isspace(l[pos]) && not_walled(pos, map))
		pos++;
	if (l[pos] && l[pos] != '1')
		return (-1);
	if (l[pos])
		return (pos + 1);
	return (pos);
}

int	check_emptyline(char *line)
{
	char	*r;

	r = trimspaces(line);
	if (!r)
		return (-1);
	if (ft_strcmp(r, ""))
	{
		free(r);
		return (0);
	}
	free(r);
	return (1);
}
