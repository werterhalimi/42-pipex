/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:59:28 by shalimi           #+#    #+#             */
/*   Updated: 2022/11/03 22:44:01 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	first_process(int in, int out[2], char *args, char **env)
{
	char	**paths;
	char	**command;
	int		pid;

	pipe(out);
	pid = fork();
	if (!pid)
	{
		paths = ft_split(&(env[ft_find_paths_index(env)][5]), ':');
		command = ft_split(args, ' ');
		dup2(in, 0);
		close(out[0]);
		dup2(out[1], 1);
		execve(get_path(paths, command[0]), command, env);
	}
	close(in);
	return (pid);
}

int	middle_process(int in[2], int out[2], char *args, char **env)
{
	char	**paths;
	char	**command;
	int		pid;

	pid = fork();
	if (!pid)
	{
		paths = ft_split(&(env[ft_find_paths_index(env)][5]), ':');
		command = ft_split(args, ' ');
		dup2(in[0], 0);
		close(in[1]);
		dup2(out[1], 1);
		close(out[0]);
		execve(get_path(paths, command[0]), command, env);
	}
	close(in[1]);
	return (pid);
}

int	final_process(int in[2], int out, char *args, char **env)
{
	int		pid;
	char	**command;
	char	**paths;

	close(in[1]);
	pid = fork();
	if (!pid)
	{
		dup2(in[0], 0);
		close(in[1]);
		dup2(out, 1);
		paths = ft_split(&(env[ft_find_paths_index(env)][5]), ':');
		command = ft_split(args, ' ');
		execve(get_path(paths, command[0]), command, env);
	}
	close(out);
	return (pid);
}

int	main(int argc, char **argv, char **env)
{
	int	*pid;
	int	in[2];
	int	out[2];
	int	j;
	int	file_fd;

	file_fd = open(argv[1], O_RDONLY);
	pid = malloc(sizeof(int) * argc - 3);
	if (!pid)
		return (0);
	pid[0] = first_process(file_fd, in, argv[2], env);
	if (argc == 5)
		int_swap(out, in);
	j = 3;
	while (j != argc - 2)
	{
		if (j > 3)
			int_swap(in, out);
		pid[j - 2] = launch_process(in, out, argv[j], env);
		j++;
	}
	file_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pid[j] = final_process(out, file_fd, argv[argc - 2], env);
	close_wait(in, out, j, pid);
	return (0);
}
