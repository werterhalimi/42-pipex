/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 20:39:32 by shalimi           #+#    #+#             */
/*   Updated: 2022/11/03 20:47:43 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# include <unistd.h>
# include <fcntl.h>

int		ft_find_paths_index(char **str);
char	*get_path(char **path, char *command);
void	int_swap(int fd[2], int src[2]);
void	close_wait(int fd[2], int out[2], int j, int *pid);
int		launch_process(int in[2], int out[2], char *arg, char **env);
int		first_process(int in, int out[2], char *args, char **env);
int		middle_process(int in[2], int out[2], char *args, char **env);
int		final_process(int in[2], int out, char *args, char **env);
#endif
