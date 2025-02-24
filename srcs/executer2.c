# include "../inc/minihell.h"

char **tokenize_command(t_token *token)
{
    int i;
    int count;
    char **args;
    t_token *tmp;

    count = 0;
    tmp = token;
    while (tmp)
    {
        if (tmp->type == COMMAND || tmp->type == OPTION || tmp->type == ARGUMENT || tmp->type == DOUBLE_QUOTE  || tmp->type == SINGLE_QUOTE)
            count++;
        else if (is_redirection(tmp->type))
        {
            tmp = tmp->next;
            if (tmp)
                tmp = tmp->next;
            continue;
        }
        else
            break ;
        tmp = tmp->next;
    }
    args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return NULL;
    i = 0;
    while (token)
    {
        if (token->type == COMMAND || token->type == OPTION || token->type == ARGUMENT || token->type == DOUBLE_QUOTE  || token->type == SINGLE_QUOTE)
        {
            args[i++] = strdup(remove_quotes(token->value));
        }
        else if (is_redirection(token->type))
        {
            token = token->next;
            if (token)
                token = token->next;
            continue;
        }
        else
            break ;
        token = token->next;
    }
    printf("\n");
    args[i] = NULL;
    return (args);
}

void redirect_input(t_token *token)
{
	int fd;
	if (!token || !token->next)
		return;
	fd = open(token->next->value, O_RDONLY);
	if (fd == -1)
	{
		perror(token->next->value);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void redirect_output(t_token *token)
{
	int fd;
	if (!token || !token->next)
		return;
	fd = open(token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(token->next->value);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void redirect_output_append(t_token *token)
{
	int fd;
	if (!token || !token->next)
		return;
	fd = open(token->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void redirect_here_doc(t_token *token)
{
	if (!token || !token->next)
		return;
    here_doc(token->next->value);
	int fd = open(".DOC_TMP", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void execute_redirections(t_token *token)
{
	t_token *tmp = token;
	while (tmp)
	{
		if (tmp->type == LESSER)
			redirect_input(tmp);
		else if (tmp->type == GREATER)
			redirect_output(tmp);
		else if (tmp->type == APPEND)
			redirect_output_append(tmp);
		else if (tmp->type == HERE_DOC)
			redirect_here_doc(tmp);
		tmp = tmp->next;
	}
}

void execute_cmd_in_pipe(t_token *token, t_pipe *pipes, int in, int out)
{
	char *path;
	char **args;

	if (!token || token->type != COMMAND)
		return;
	args = tokenize_command(token);
	path = find_path(args[0], pipes->ev);
	if (!path)
	{
		printf("This cmd %s does not exist!\n", args[0]);
		exit(EXIT_FAILURE);
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

void	exec_builtins(t_shell *shell, t_token *token)
{
/*	if (strcmp(token->value, "echo") == 0)
		ft_echo(token);
	else */if (strcmp(token->value, "cd") == 0)
		ft_cd(token);
	else if (strcmp(token->value, "pwd") == 0)
		ft_pwd(token);
	else if (strcmp(token->value, "env") == 0)
		ft_env(shell->env, token);
	else if (strcmp(token->value, "export") == 0)
		ft_export(shell->env, token);
	else if (strcmp(token->value, "unset") == 0)
		ft_unset(shell->env, token);
	else if (strcmp(token->value, "exit") == 0)
		ft_exit(shell);
	printf("\nfrom builtins\n");
}

void execute_pipe2(t_shell *shell, t_token *tokens)
{
    pid_t id;
    t_pipe *pipes = malloc(sizeof(t_pipe));
    if (!pipes)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    pipes->i = 0;
    pipes->input_fd = 0;
    pipes->ev = shell->ev;
    t_token *cmd_start = tokens;

    while (cmd_start)
    {
        if (cmd_start->type == PIPE || !cmd_start->next)
        {
            if (pipe(pipes->pipe_fd) == -1)
            {
                perror("pipe");
                free(pipes);
                exit(EXIT_FAILURE);
            }
            id = fork();
            if (id == -1)
            {
                perror("fork");
                free(pipes);
                exit(EXIT_FAILURE);
            }
            if (id == 0)
            {
                execute_redirections(tokens);
                if (is_builtin(tokens->type))
                    exec_builtins(shell, tokens);
                else
                    execute_cmd_in_pipe(tokens, pipes, pipes->input_fd, cmd_start->type == PIPE ? pipes->pipe_fd[1] : STDOUT_FILENO);
            }
            if (pipes->input_fd != 0)
                close(pipes->input_fd);
            if (cmd_start->type == PIPE)
            {
                close(pipes->pipe_fd[1]);
                pipes->input_fd = pipes->pipe_fd[0];
            }
            tokens = cmd_start->next;
        }
        cmd_start = cmd_start->next;
    }

    // Wait for all child processes to finish
    while (wait(NULL) > 0);

    free(pipes);
}

