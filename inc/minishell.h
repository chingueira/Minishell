/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:21:36 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/16 16:32:34 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PATH_MAX   4096

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "./structs.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>


// ----------------- Redirections & Here Document -------------------/

void	redirect_input(char *cmd, char *file, char **env);
void	redirect_output(char *cmd, char *file, char **env);

void	here_doc(char *str);
int		check_doc(char *input);

//------------------- Builtins --------------------/
void	ft_pwd(t_token *token);
void	ft_exit(t_shell *shell, t_token *token);
void	ft_echo(t_token *token);
void	ft_cd(t_token *current);
void	ft_exit(t_shell *shell, t_token *token);
void	ft_unset(t_env *env, t_token *token);
void	ft_export(t_env *env, t_token *token);
void	ft_env(t_env *env, t_token *token);

char	*get_path(t_env *env);
char	*cmd_path(char *cmd, char *pat);
char	*remove_quotes(char *str);
char	**split_input(char *input, t_shell *shell);
char	*TokenKindString(t_type type);
char	*extract_variable(char **input);
char	*GroupKindString(t_group_type type);
char	*fill_quote(char **input, char quote, int i);

bool	validate_quote_number(char *input);
bool	extract_operator_util(char **input);
bool	is_command(char *value, char *path);

void	repl(t_shell *shell);
void	debug(t_token *token, int number_of_commands);
void	clean(t_shell *shell);
void	token_sequence(t_token *tokens);
void	clean_and_exit(t_shell *shell);
void	expand(char ***matrix, t_env *env);
void	init_shell(t_shell *shell, char **env);
void	identify_tokens(t_token *tokens, char *path);
void	execute_pipe(t_shell *shell);
char	*clean_string(char *str);
char	**clean_args(char **args);

int		word_count_util(char *i, int count);
int		count_char(char *input, char c);

t_type	find_type(char *value);

t_env	*convert_env(char **env);

t_token	*tokenize_matrix(char **matrix);
t_token	*get_token_lowest_precedence(t_token *tokens);
t_token	*new_token(char *value, t_type type, t_group_type group_type);


// ----------------- repl -------------------/
bool	pipe_check(t_shell *shell);

void	debug_commands(t_command *command_groups);

void	execute(t_shell *shell);

void	exec_builtins(t_shell *shell);

void	exec_cmd(t_shell *shell);

// ----------------- utils -------------------/
bool	is_builtin(t_type type);
bool	is_operator(t_type type);
bool	is_argument(t_type type);
bool	is_redirection(t_type type);
bool	is_builtin_or_command(t_type type);

void	error_message(char *str);

int	count_args(t_token *tmp_token);
int	number_of_commands(t_token *tokens);

#endif
