# include "./inc/minihell.h"

void	error_message(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

int	count_arr_elem(char **arr)
{
	int	elem;

	elem = 0;
	while (arr[elem] != NULL)
		elem++;
	return (elem);
}

void	execute_command_in_pipe(char *command, t_pipe *pipes, int in, int out)
{
	char	*path;
	char	*cmd;
	char	**args;

	cmd = strdup(command);
	args = ft_split(cmd, ' ');
	path = find_path(args[0], pipes->ev);
	free(cmd);
	if (!path)
	{
		printf("This cmd %s does not exist!\n", args[0]);
		return ;
	}
	if (in != 0)
	{
		dup2(in, STDIN_FILENO);
		close(in);
	}
	if (out != 1)
	{
		dup2(out, STDOUT_FILENO);
		close(out);
	}
	if (execve(path, args, pipes->ev) == -1)
		error_message("execve");
}

static void	child_process(char *cmd, t_pipe *pipes, int num_commands)
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
	execute_command_in_pipe(cmd, pipes, STDIN_FILENO, STDOUT_FILENO);
}

static void	parent_process(t_pipe *pipes, int num_commands)
{
	if (pipes->input_fd != 0)
		close(pipes->input_fd);
	if (pipes->i < num_commands - 1)
	{
		close(pipes->pipe_fd[1]);
		pipes->input_fd = pipes->pipe_fd[0];
	}
}

void	execute_pipe(t_pipe *pipes, char **cmd, int num_commands)
{
	while (pipes->i < num_commands)
	{
		if (pipes->i < num_commands)
			if (pipe(pipes->pipe_fd) == -1)
				error_message("pipe");
		pipes->id = fork();
		if (pipes->id == -1)
			error_message("fork");
		if (pipes->id == 0)
			child_process(cmd[pipes->i], pipes, num_commands);
		wait(NULL);
		parent_process(pipes, num_commands);
		pipes->i++;
	}
}

int		redir_position(char *str)
{
	int	i = 0;
	while (str[i])
	{
		if (str[i] == '<')
			return (i);
		i++;
	}
	return (0);
}
char	**copy_array(char *str, char **array, int start)
{
	int	i = 1;
	int j = start;
	char	**copy;

	while (array[start] != NULL)
		start++;
	copy = malloc(sizeof(char *) * start + 1);
	copy[0] = ft_strdup(str);
	while (array[j])
	{
		copy[i] = ft_strdup(array[j]);
		i++;
		j++;
	}
	copy[i] = NULL;
	return (copy);
}

char	*eliminate_space_from_str(char *str)
{
	int	i = 0;
	int j = 0;
	char *new_str;

	while (ft_isspace(str[i]))
		i++;
	new_str = malloc(sizeof(str) + 1);
	while (str[i] && str[i] != ' ')
	{
		new_str[j] = str[i];
		i++;
		j++;
	}
	new_str[j] = '\0';
	return(new_str);
}

void	redirect_input2(char **cmd, t_pipe *pipes, int num_commands)
{
	int		x;
	int		fd;
	char	*file;
	char 	**array;
	char	**cmds;

	x = redir_position(cmd[0]);
	if (x == 0)
	{
		array = ft_split(cmd[0], ' ');
		file = eliminate_space_from_str(array[1]);
		cmds = copy_array(array[2], cmd, 1);
	}
	else
	{
		array = ft_split(cmd[0], '<');
		file = eliminate_space_from_str(array[1]);
		cmds = copy_array(array[0], cmd, 1);
	}
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		printf("%s\n", file);
		return ;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return ;
	}
	close(fd);
	execute_pipe(pipes, cmds, num_commands);
}


void	execute_pipe_and_redirections(t_shell *shell)
{
	int		num_commands;
	char	**cmd;
	char	*clean_input;
	t_pipe	*pipes = malloc(sizeof(t_pipe));

	pipes->i = 0;
	pipes->input_fd = 0;
	clean_input = clean_string(shell->input);
	cmd = ft_split(clean_input, '|');
	num_commands = count_arr_elem(cmd);
	pipes->ev = shell->ev;
	if (have_redirections(cmd[0]) || have_redirections(cmd[num_commands - 1]))
	{
		if (have_redirections(cmd[0]))
				redirect_input2(cmd, pipes, num_commands);
		else if (have_redirections(cmd[num_commands - 1]))
		{
			printf("Redirect Output\n");
			return ;
		}
	}
	else
		execute_pipe(pipes, cmd, num_commands);
}