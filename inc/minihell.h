/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minihell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:16:19 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/24 13:09:20 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIHELL_H
# define MINIHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../libft/libft.h"

// --------------     ATENÇÃO!  ------------------- /
#include <stdbool.h> 
# include <strings.h>
//--------------------------------------------- Structs -----------------------------------------------------/

typedef enum e_type
{
	HERE_DOC,
	LESSER,
	GREATER,
	APPEND,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	PIPE,
	BUILTINS,
	COMMAND,
	OPTION,
	ARGUMENT,
	IDENTIFIER
}		t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	struct s_token	*next;
	struct s_token	*prev;
}		t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}		t_env;

typedef struct s_pipe
{
	int		i;
	int		pipe_fd[2];
	int		input_fd;
	char	**ev;
	pid_t	id;
}		t_pipe;

typedef struct	s_shell
{
	char	*input;
	char	*path;
	char	**ev;
	char	**array;
	bool	flag;
	int		num_of_cmds;
	t_env	*env;
	t_token	*token;
	t_pipe	*pipe;
}		t_shell;

//--------------------------------------------- Redirections -----------------------------------------------------/


//--------------------------------------------- Pipe -----------------------------------------------------/


//--------------------------------------------- Executer -----------------------------------------------------/
int		arr_size(t_token *token);
char	*file_name(t_token *token);
char	**cmd_args(t_token *token);
char	**copy_array(char *str, char **array, int start);
void	executer(t_shell *shell);
char	*find_path(char *cmd, char **envp);
void	execute_pipe(t_shell *shell, t_token *tokens);
void	here_doc(char *str);
void	execute_pipe2(t_shell *shell, t_token *tokens);
char	*remove_quotes(char *str);

//--------------------------------------------- Tokens -----------------------------------------------------/
void	identify_tokens(t_token *tokens, char *path);
t_token	*tokenize_array(char **array);
bool 	is_command(char *value, char *path);
bool	is_builtin_or_command(t_type type);
bool	is_builtin(t_type type);
t_token	*new_token(char *value, t_type type);
void	token_sequence(t_token *tokens);
char	*token_kind_string(t_type type);
bool	is_append(t_type token);
bool	is_greater(t_type token);
bool	is_lesser(t_type token);
bool 	is_command2(char *value);

//--------------------------------------------- Parse -----------------------------------------------------/
int		word_count(char *input);
char	*extract_quote(char **input);
char	*extract_operator(char **input);
void	parse(t_shell *shell);
bool	validate_quote_number(char *input);
int		number_of_commands(t_token *tokens);
void	expand(char ***matrix, t_env *env);
t_env	*convert_env(char **env);
char	*get_path(t_env *env);
char	*clean_string(char *str);
char	*remove_quotes(char *str);

//--------------------------------------------- Builtins -----------------------------------------------------/
void	ft_cd(t_token *current);
void	ft_pwd(t_token *token);
void	ft_exit(t_shell *shell);
void	ft_env(t_env *env, t_token *token);
void	ft_export(t_env *env, t_token *token);
void	ft_unset(t_env *env, t_token *token);

void	error_message(char *str);
bool	is_redirection(t_type type);
bool 	is_command2(char *value);
bool	is_argument(t_type type);
bool	is_option(t_type type);
int		have_redirections(char *s);
void	error_message(char *str);
char	*eliminate_space_from_str(char *str);
void	free_array(char **arr);



#endif
