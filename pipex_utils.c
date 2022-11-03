/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 20:38:06 by shalimi           #+#    #+#             */
/*   Updated: 2022/11/03 20:49:06 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	int_swap(int dest[2], int src[2])
{
	dest[0] = src[0];
	dest[1] = src[1];
}

void	close_wait(int fd[2], int out[2], int j, int *pid)
{
	close(fd[0]);
	close(fd[1]);
	close(out[0]);
	close(out[1]);
	while (j >= 0)
		wait(&pid[j--]);
}

int	launch_process(int in[2], int out[2], char *arg, char **env)
{
	pipe(out);
	return (middle_process(in, out, arg, env));
}

int	ft_find_paths_index(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_strncmp("PATH=", str[i], 5) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*get_path(char **path, char *command)
{
	int		i;
	char	*ret;

	i = 0;
	while (path[i])
	{
		ret = ft_strjoin(path[i], ft_strjoin("/", command));
		if (!access(ret, X_OK))
			return (ret);
		i++;
	}
	return (0);
}
