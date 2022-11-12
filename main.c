/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:59:28 by shalimi           #+#    #+#             */
/*   Updated: 2022/11/12 18:05:06 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	first_process(int in, int out[2], char *args, char **env)
{
	char	**paths;
	char	**command;
	char	*path;
	int		pid;

	pipe(out);
	pid = fork();
	if (!pid)
	{
		paths = ft_split(&(env[ft_find_paths_index(env)][5]), ':');
		command = ft_split(args, ' ');
		path = get_path(paths, command[0]);
		dup2(in, 0);
		close(out[0]);
		dup2(out[1], 1);
		execve(path, command, env);
	}
	close(in);
	close(out[1]);
	return (pid);
}

int	middle_process(int in[2], int out[2], char *args, char **env)
{
	char	**paths;
	char	**command;
	int		pid;
	char	*path;

	pid = fork();
	if (!pid)
	{
		paths = ft_split(&(env[ft_find_paths_index(env)][5]), ':');
		command = ft_split(args, ' ');
		path = get_path(paths, command[0]);
		dup2(in[0], 0);
		dup2(out[1], 1);
		close(out[0]);
		execve(path, command, env);
	}
	close(out[1]);
	return (pid);
}

int	final_process(int in[2], int out, char *args, char **env)
{
	int		pid;
	char	**command;
	char	**paths;
	char	*path;

	close(in[1]);
	pid = fork();
	if (!pid)
	{
		command = ft_split(args, ' ');
		paths = ft_split(&(env[ft_find_paths_index(env)][5]), ':');
		path = get_path(paths, command[0]);
		dup2(in[0], 0);
		dup2(out, 1);
		execve(path, command, env);
	}
	close(out);
	return (pid);
}

void	launch_pipex(int argc, char **argv, char **env, int files[2])
{
	int	*pid;
	int	in[2];
	int	out[2];
	int	j;

	pid = malloc(sizeof(int) * argc - 3);
	if (!pid)
		return ;
	pid[0] = first_process(files[0], in, argv[2], env);
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
	pid[j] = final_process(out, files[1], argv[argc - 2], env);
	close_wait(in, out, j, pid);
}

int	main(int argc, char **argv, char **env)
{
	int	files[2];

	if (argc < 5)
	{
		ft_putendl_fd("Mauvaise execution de la commande", 1);
		exit(1);
	}
	files[0] = open(argv[1], O_RDONLY);
	files[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (files[0] < 0 || files[1] < 0)
	{
		ft_putendl_fd("T'as fais dla merde bg du coup je quitte.", 1);
		ft_putendl_fd("Je n'arrive pas a lire l'input ou creer l'output", 1);
		ft_putendl_fd("Bisou", 1);
		ft_putendl_fd("PIPEX OVER", 1);
		exit(1);
	}
	launch_pipex(argc, argv, env, files);
	return (0);
}
