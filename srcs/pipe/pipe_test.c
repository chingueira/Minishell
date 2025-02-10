#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

typedef struct s_pipe
{
	int		pipe_fd[2];
	int		input_fd;
	pid_t	pid;
	int		i;
	char	**ev;
}		t_pipe;

void	error_message(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

static int	ft_wordcount(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (s[i] != '\0')
		{
			count++;
			i++;
		}
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

static int	ft_wordlen(char const *s, char c, int start)
{
	int	i;
	int	count;

	i = start;
	count = 0;
	while (s[i] && s[i] != c)
	{
		count++;
		i++;
	}
	return (count);
}

static char	*ft_getword(char const *s, char c, int start)
{
	char	*word;
	int		len;
	int		i;
	int		j;

	i = start;
	j = 0;
	len = ft_wordlen(s, c, start);
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	while (s[i] != c && s[i] != '\0')
		word[j++] = s[i++];
	word[j] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**arr;

	i = 0;
	j = -1;
	if (!s)
		return (NULL);
	arr = (char **)malloc((sizeof(char *) * (ft_wordcount(s, c) + 1)));
	if (!arr)
		return (NULL);
	while (s[i])
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (s[i] != '\0')
			arr[++j] = ft_getword(s, c, i++);
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	arr[++j] = NULL;
	return (arr);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*str;
	size_t	total_len;

	total_len = (strlen(s1) + strlen(s2));
	str = (char *)malloc(sizeof(char) * (total_len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	little_len;

	if (*little == '\0')
		return ((char *)big);
	little_len = strlen(little);
	if (little_len == 0)
		return ((char *)big);
	i = 0;
	while ((i <= (len - little_len)) && ((i + little_len) <= len))
	{
		if (strncmp(&big[i], little, little_len) == 0)
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = -1;
	free(paths);
	return (0);
}

void	execute_command(t_pipe *pipes, char *command, int input_fd, int output_fd)
{
	char	*path;
	char	**args;
	char	*cmd_copy = strdup(command);

	
	args = ft_split(cmd_copy, ' ');
	path = find_path(args[0], pipes->ev);
	if (!path)
	{
		printf("This cmd %s does not exist!", args[0]);
		exit(EXIT_FAILURE);
	}
	if (input_fd != 0)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != 1)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	if (execve(path, args, pipes->ev) == -1)
		error_message("execve");
	free(cmd_copy);
}

void	child_process(char *command, t_pipe *pipes, int num_commands)
{
	if (pipes->input_fd != 0)
	{
		dup2(pipes->input_fd, STDIN_FILENO);
		close(pipes->input_fd);
	}
	if (pipes->i < num_commands - 1)
	{
		dup2(pipes->pipe_fd[1], STDOUT_FILENO);
		close(pipes->pipe_fd[1]);
	}
	if (pipes->i < num_commands - 1)
		close(pipes->pipe_fd[0]);
	execute_command(pipes, command, STDIN_FILENO, STDOUT_FILENO);
}

void	parent_process(t_pipe *pipes, int num_commands)
{
	if (pipes->input_fd != 0)
		close(pipes->input_fd);
	if (pipes->i < num_commands - 1)
	{
		close(pipes->pipe_fd[1]);
		pipes->input_fd = pipes->pipe_fd[0];
	}
}

void	execute_pipe(char *command[], t_pipe *pipes, int num_commands)
{
	while(pipes->i < num_commands)
	{
		if (pipes->i < num_commands - 1)
		{
			if (pipe(pipes->pipe_fd) == -1)
				error_message("pipe");
		}
		pipes->pid = fork();
		if (pipes->pid == -1)
			error_message("fork");
		if (pipes->pid == 0)
			child_process(command[pipes->i], pipes, num_commands);
		else
		{
			wait(NULL);
			parent_process(pipes, num_commands);
		}
		pipes->i++;
	}
}

int	main(int ac, char **av, char **env)
{ 
	if (ac == 2)
	{
		t_pipe	*pipes = malloc(sizeof(t_pipe));
		if (!pipes)
			error_message("malloc");
		pipes->i = 0;
		pipes->input_fd = 0;
		pipes->ev = env;
		char **commands = ft_split(av[1], '|');
		int i = 0;
		while (commands[i])
			printf("%s\n", commands[i++]);
		execute_pipe(commands, pipes, i);
		free(pipes);
	}
	printf("\n");
	return (0);
}
