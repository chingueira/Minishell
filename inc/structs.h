/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:50:46 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/13 16:19:58 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_shell	t_shell;

typedef enum e_group_type
{
	G_SIGNAL,
	G_SINGLE_QUOTE,
	G_DOUBLE_QUOTE,
	G_IDENTIFIER,
	G_COMMAND,
	G_REDIRECTION,
	G_OPERATOR,
	G_ENDOFFILE
}	t_group_type;

typedef enum e_type
{
	PIPE,
	DOUBLE_AND,
	DOUBLE_PIPE,
	DOUBLE_LESSER,
	GREATER,
	DOUBLE_GREATER,
	LESSER,
	COMMAND,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	STAR,
	IDENTIFIER,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	PATH,
	OPTION,
	VARIABLE,
	ARGUMENT,
	ENDOFFILE,
}	t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	t_group_type	group_type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_pipe
{
	int		pipe_fd[2];
	int		input_fd;
	int		i;
	char 	**ev;
}		t_pipe;

typedef struct	s_command
{
	char	*command;
	char	**args;
	char	*path;
	struct s_command	*next;
}	t_command;

struct s_shell
{
	t_token	*token;
	t_env	*env;
	t_pipe	*pipe;
	t_command	*command_groups;
	char	**matrix;
	char	*input;
	char	*path;
	int		pipe_fd[2];
	bool	flag;
	int		result;
	char	*current_dir;
	char	**anv;
	int		number_of_commands;
};

#endif
