/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:59:28 by shalimi           #+#    #+#             */
/*   Updated: 2022/11/02 19:12:30 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <unistd.h>
#include <fcntl.h>
/*void	*ft_calloc(size_t nb, size_t len)
{
	void	*ret;
	int		i;

	i = 0;
	ret = malloc(len * nb);
	if (!ret)
		return (0);
	while (i < (int)(len * nb))
		((unsigned char *)ret)[i++] = 0;
	return (ret);
}
*/



typedef struct s_command
{
	char	*in;
	char	*out;
	int		argc;
	char **argv;
} t_command;

t_command new_command(int argc, char **argv)
{
	t_command	ret;
	
	ret.argc = argc;
	ret.argv = argv;
	return (ret);
}

int	ft_bufferlen(unsigned char *buffer)
{
	int	i;

	i= 0;
	while (buffer[i])
	{
		i++;
	}
	return (i);
}

unsigned char	*ft_strfix(unsigned char *str)
{
	unsigned char	*ret;

	ret = malloc (sizeof(*ret) * 1 + ft_strlen((char *)str));
	ft_memcpy(ret, str, ft_strlen((const char *)str));
	free(str);
	return (ret);
}

//$> ./pipex infile "grep a1" "wc -w" outfile
unsigned char	*read_file(int fd)
{
	unsigned char	*buffer;
	unsigned char	*ret;
	int len;
	int i;

	ret = malloc(sizeof(char));
	buffer = ft_calloc(sizeof(*buffer), 1 + BUFFER_SIZE);
	len = read(fd, buffer, BUFFER_SIZE);
	i = 0;
	while (len > 0)
	{
		ft_strlcat((char *)ret + i, (char *)buffer, len + 2);
		ret = ft_strfix(ret);
		i += len;
		ret[i] = 0;
		len = read(fd, buffer, BUFFER_SIZE);
		if (len < 1)
			break ;
	}
	ret[i] = 0;
	free(buffer);
	return (ret);
}

int	ft_find_paths_index(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_strncmp("PATH=",str[i], 5) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*get_path(char **path, char *command)
{
	int	i;
	char	*ret;

	i = 0;
	while (path[i])
	{
		ret = ft_strjoin(path[i], ft_strjoin("/",command));
		if (!access(ret, X_OK))
			return (ret);
		i++;
	}
	return (0);
}

int	handle_here_doc(char *eof)
{
	int	fd[2];
	(void) eof;
	pipe(fd);
	write(fd[1], "Hello", 4);
	close(fd[1]);
//	close(fd[1]);
/*	int	len;
	char	*buff;
	char	*str;
	pipe(fd);

	str = ft_calloc(sizeof(char), 1);
	buff = ft_calloc(sizeof(char), 1 + ft_strlen(eof));
	len = read(0, buff, ft_strlen(eof));
	while (ft_strncmp(eof, buff, ft_strlen(eof)) != 0)
	{
		buff[len] = 0;
		char *tmp = str;
		str = ft_strjoin(str, buff);
		
		free(tmp);
		len = read(0, buff, ft_strlen(eof));
	}
	ft_putstr_fd(str,fd[1]);
	ft_putstr_fd(str,0);
	close(fd[1]);
	free(buff);*/
	return (fd[0]);
}

#ifndef UNIT
#include <stdio.h>
int	main(int argc, char **argv, char **env)
{
	int	*pid;
	int fd[2];
	int	out[2];
	int i;
	char	**paths;
	int file2_fd;
	char **command;
	char *args;
	int file1_fd;
	
	i= 2;
	
	paths= ft_split(&(env[ft_find_paths_index(env)][5]), ':');
	if (ft_strncmp(argv[1],"here_doc", 8) == 0)
	{
		file1_fd = handle_here_doc(argv[2]);
		pid = malloc(sizeof(int) * argc - 4);
		i++;
	}
	else
	{
		file1_fd = open(argv[1], O_RDONLY);
		pid = malloc(sizeof(int) * argc - 3);

	}
	pipe(fd);
	pid[0] = fork();
	if (!pid[0])
	{
		dup2(file1_fd, 0);
		args = argv[i];
		command= ft_split(args, ' ');
		close(fd[0]);
		dup2(fd[1], 1);
		execve(get_path(paths, command[0]), command, env);
	}
	else
	{
		close(file1_fd);
		i++;
		if (argc == 5)
		{
			out[0] = fd[0];
			out[1] = fd[1];
		}
		int j = 1;
		while (i != argc - 2)
		{
			if (i > 3)
			{
				fd[0] = out[0];
				fd[1] = out[1];
			}
			pipe(out);
			pid[j] = fork();
			if(!pid[j])
			{
				// REGARDE ICI
				dup2(fd[0], 0);
				close(fd[1]);
				dup2(out[1], 1);
				close(out[0]);
				args = argv[i];
				command = ft_split(args, ' ');
				execve(get_path(paths, command[0]), command, env);
			}
			else
			{
				close(fd[0]);
				close(fd[1]);
				//close(out[0]);
				//close(out[1]);
			}
			i++;
			j++;
		}
		pid[j] = fork();
		if (!pid[j])
		{
			dup2(out[0], 0);	
			file2_fd = open(argv[argc - 1],  O_WRONLY | O_CREAT | O_TRUNC, 0644);
			close(out[1]);
			dup2(file2_fd, 1);
			args = argv[argc - 2];
			command = ft_split(args, ' ');
			execve(get_path(paths, command[0]), command, env);
		}
		else
		{
			close(fd[0]);
			close(fd[1]);
			int m = 0;
			while (m < j)
				wait(&pid[m++]);
		}
	}
	return (0);
}
#endif

#ifdef UNIT
#include <stdio.h>
int	main(void)
{
	unsigned char	*test;
	int fd = open("file1",O_RDONLY);
	test = read_file(fd);
	printf("\ncontent of file1 :%s\n", test);
}
#endif
