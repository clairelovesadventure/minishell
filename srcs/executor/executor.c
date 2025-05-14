/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/14 15:58:38 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Forward declare the new function if needed, or ensure filename_utils.h */
/* exists and is included */
char	*expand_filename_quotes(const char *raw_filename);

static void	smart_close(int *fd_ptr)
{
	if (fd_ptr && *fd_ptr != -1)
	{
		close(*fd_ptr);
		*fd_ptr = -1;
	}
}

/*
** Checks if the given command is a built-in command.
*/
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

/*
** Executes a built-in command.
*/
int	exec_builtin(t_cmd *cmd, t_env **env_list, t_shell *shell)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	/* Ensure shell is not NULL */
	if (!shell)
	{
		fprintf(stderr, "Shell structure is NULL in exec_builtin\n");
		return (1);
	}
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(cmd->args, *env_list));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(cmd->args, env_list));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(cmd->args, env_list));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(cmd->args, env_list));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(*env_list));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(cmd->args, shell));
	return (0);
}

/*
** Creates pipes for the command list.
*/
static int	create_pipes(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current && current->next)
	{
		if (pipe(current->pipe_fd) == -1)
		{
			perror("pipe");
			return (0);
		}
		current = current->next;
	}
	return (1);
}

/* Helper function to process and validate the filename for a redirection */
static int	process_redir_filename(t_redirect *redirect_item,
								char **processed_filename_out)
{
	if (redirect_item->type != T_HEREDOC)
		*processed_filename_out = expand_filename_quotes(redirect_item->file);
	else
		*processed_filename_out = ft_strdup(redirect_item->file);
	if (!*processed_filename_out)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (0);
	}
	if ((*processed_filename_out)[0] == '\0')
	{
		/* TODO: Use print_error here for consistency if available and normed */
		ft_putstr_fd("minishell: ambiguous redirect\n", STDERR_FILENO);
		free(*processed_filename_out);
		*processed_filename_out = NULL;
		return (0);
	}
	return (1);
}

/* Helper function to handle input redirection (T_REDIR_IN) */
static int	apply_redir_input(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_error("minishell", filename, strerror(errno));
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 stdin failed");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

/* Helper function to handle output/append (T_REDIR_OUT, T_REDIR_APPEND) */
static int	apply_redir_output(const char *filename, int redirect_type)
{
	int	fd;
	int	flags;

	if (redirect_type == T_REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else /* T_REDIR_APPEND */
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		print_error("minishell", filename, strerror(errno));
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 stdout failed");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

/* Helper function to handle heredoc redirection (T_HEREDOC) */
static int	apply_redir_heredoc(const char *delimiter)
{
	int	fd;

	fd = handle_heredoc((char *)delimiter); /* handle_heredoc needs char* */
	if (fd == -1)
		return (0); /* handle_heredoc should print its own errors */
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 stdin for heredoc failed");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

/*
** Sets up file redirections for a command.
*/
static int	setup_redirections(t_redirect *redirects)
{
	char	*processed_filename;
	int		status;

	status = 1;
	processed_filename = NULL;
	while (redirects && status)
	{
		if (!process_redir_filename(redirects, &processed_filename))
			return (0); /* Error in filename processing */
		if (redirects->type == T_REDIR_IN)
			status = apply_redir_input(processed_filename);
		else if (redirects->type == T_REDIR_OUT)
			status = apply_redir_output(processed_filename, T_REDIR_OUT);
		else if (redirects->type == T_REDIR_APPEND)
			status = apply_redir_output(processed_filename, T_REDIR_APPEND);
		else if (redirects->type == T_HEREDOC)
			status = apply_redir_heredoc(processed_filename);
		free(processed_filename);
		processed_filename = NULL;
		if (!status) /* If any redirection failed, stop and return failure */
			return (0);
		redirects = redirects->next;
	}
	return (status); /* 1 for success, 0 for failure */
}

/* Helper function to iterate through PATH entries and check for command */
static char	*search_cmd_in_paths(const char *cmd, char **paths)
{
	int		i;
	char	*cmd_path;
	char	*tmp_join;

	i = 0;
	cmd_path = NULL;
	while (paths && paths[i])
	{
		tmp_join = ft_strjoin(paths[i], "/");
		if (!tmp_join)
			return (NULL); /* Malloc error, paths will be freed by caller */
		cmd_path = ft_strjoin(tmp_join, cmd);
		free(tmp_join);
		if (!cmd_path)
			return (NULL); /* Malloc error, paths will be freed by caller */
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path); /* Found */
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (NULL); /* Not found in paths */
}

/*
** Finds the full path of an executable command.
*/
char	*find_executable(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*found_path;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	/* If cmd is an absolute or relative path */
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK) != 0)
			return (NULL); /* File does not exist */
		return (ft_strdup(cmd)); /* Let execve determine executability */
	}
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL); /* Malloc check for ft_split */
	found_path = search_cmd_in_paths(cmd, paths);
	free_array(paths); /* free_array handles if paths is NULL */
	return (found_path); /* Can be NULL if not found or if search_cmd_in_paths had malloc error */
}

/*
** Frees a null-terminated array of strings.
*/
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/* Handles error printing and exit if execve fails */
static void	handle_execve_error(t_cmd *cmd, char *executable_path,
							char **envp_array)
{
	/* execve failed, print error based on errno */
	if (cmd && cmd->args && cmd->args[0])
		print_error("minishell", cmd->args[0], strerror(errno));
	else if (executable_path) /* Fallback if args[0] is somehow null */
		print_error("minishell", executable_path, strerror(errno));
	else /* Should not happen: both exec_path and args[0] are null */
		print_error("minishell", "(unknown)", strerror(errno));

	free(executable_path); /* Free path as execve failed */
	free_array(envp_array);
	if (errno == EACCES)
		exit(126);
	else if (errno == ENOENT)
		exit(127); /* Should have been caught by find_executable or NULL check */
	else
		exit(EXIT_FAILURE); /* General failure */
}

/*
** Executes an external command.
** This function is intended to be called from a child process.
*/
static int	execute_external_cmd_child(t_cmd *cmd,
							char *executable_path, char **envp_array)
{
	reset_signals(); /* Restore default signal handlers for the child */

	if (!executable_path)
	{
		if (cmd && cmd->args && cmd->args[0])
		{
			if (ft_strchr(cmd->args[0], '/'))
				print_error("minishell", cmd->args[0], "No such file or directory");
			else
				print_error("minishell", cmd->args[0], "command not found");
		}
		else
			print_error("minishell", "(null)", "command not found");
		free_array(envp_array);
		/* Do not free executable_path here as it's NULL */
		exit(127);
	}

	execve(executable_path, cmd->args, envp_array);

	/* If execve returns, it's an error. This function will exit. */
	handle_execve_error(cmd, executable_path, envp_array);
	return (EXIT_FAILURE); /* Should not be reached as handle_execve_error exits */
}

/*
** Counts the number of commands in a command list.
*/
static int	count_commands(t_cmd *cmd_list)
{
	int	count;
	t_cmd	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/*
** Checks if a built-in command MUST run in the parent process
** (e.g., cd, export, unset, exit).
*/
static int	is_parent_builtin(const char *cmd_name)
{
	if (!cmd_name)
		return (0);
	return (ft_strcmp(cmd_name, "cd") == 0
			|| ft_strcmp(cmd_name, "export") == 0
			|| ft_strcmp(cmd_name, "unset") == 0
			|| ft_strcmp(cmd_name, "exit") == 0);
}

/*
** Closes all pipe file descriptors in a command list.
*/
static void	close_all_pipes(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		smart_close(&(current->pipe_fd[0]));
		smart_close(&(current->pipe_fd[1]));
		current = current->next;
	}
}

/*
** Waits for all child processes to complete and returns the exit status
** of the last command in the pipeline.
*/
static int	wait_for_children(pid_t *pids, int num_cmds)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	status = 0;
	last_status = 0;
	if (!pids) /* Should not happen if num_cmds > 0 */
		return (1);
	while (i < num_cmds)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			perror("waitpid"); /* Log error but continue */
		if (i == num_cmds - 1) /* Store status of the last command */
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
			else
				last_status = 1; /* Default error for unknown termination */
		}
		i++;
	}
	return (last_status);
}

/*
** Configures STDIN and STDOUT for the child process based on pipeline position.
** STDIN is taken from prev->pipe_fd[0] if prev exists.
** STDOUT is directed to current->pipe_fd[1] if current->next exists.
** Exits on dup2 failure.
*/
static void	setup_child_pipes(t_cmd *current, t_cmd *prev)
{
	if (prev)
	{
		if (dup2(prev->pipe_fd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 stdin pipe");
			exit(EXIT_FAILURE);
		}
	}
	if (current->next)
	{
		if (dup2(current->pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 stdout pipe");
			exit(EXIT_FAILURE);
		}
	}
}

/*
** Iterates through the entire command list (pipeline) and closes
** both read and write ends of all pipes in the child process.
** This is crucial to prevent processes from hanging.
*/
static void	close_all_pipes_in_child(t_cmd *cmd_list_head)
{
	t_cmd	*iter;

	iter = cmd_list_head;
	while (iter)
	{
		smart_close(&iter->pipe_fd[0]);
		smart_close(&iter->pipe_fd[1]);
		iter = iter->next;
	}
}

/*
** Executes the given command (builtin or external) in the child process.
** Retrieves environment, finds executable for external commands.
** This function ultimately calls exit().
*/
static void	execute_child_command(t_cmd *current, t_shell *shell)
{
	int		exit_status;
	char	**envp_array;
	char	*executable_path;

	if (current->args && current->args[0])
	{
		if (is_builtin(current->args[0]))
		{
			exit_status = exec_builtin(current, &shell->env_list, shell);
			exit(exit_status);
		}
		else
		{
			envp_array = env_to_array(shell->env_list);
			executable_path = find_executable(current->args[0],
					shell->env_list);
			execute_external_cmd_child(current, executable_path, envp_array);
			/* execute_external_cmd_child always exits */
			exit(127);
		}
	}
	else
	{
		/* No command, e.g., only redirections like '> out', success. */
		exit(0);
	}
}

/*
** Child process logic for executing a single command in a pipeline.
** It configures pipe FDs, closes unused FDs, applies redirections,
** and executes the command. This function always exits.
*/
static void	execute_pipeline_child(t_cmd *current, t_cmd *prev, t_shell *shell)
{
	int	redir_status_child;

	setup_child_pipes(current, prev);
	close_all_pipes_in_child(shell->cmd_list);

	redir_status_child = setup_redirections(current->redirects);
	if (!redir_status_child)
	{
		/* Error message should have been printed by setup_redirections */
		exit(1);
	}
	execute_child_command(current, shell);
}

/*
** Main function to execute a list of commands.
** Handles pipelines, redirections, builtins, and external commands.
*/

/* Helper for single parent-run builtins: sets up redir, runs, restores IO */
static int	manage_single_parent_builtin(t_cmd *cmd, t_env **env_list,
								 t_shell *shell, int stdin_backup,
								 int stdout_backup)
{
	int	exit_status;
	int	redir_status;

	redir_status = setup_redirections(cmd->redirects);
	if (!redir_status)
		exit_status = 1; /* Redirection failed */
	else
		exit_status = exec_builtin(cmd, env_list, shell);

	if (dup2(stdin_backup, STDIN_FILENO) == -1)
		perror("minishell: dup2 restore stdin parent_builtin");
	if (dup2(stdout_backup, STDOUT_FILENO) == -1)
		perror("minishell: dup2 restore stdout parent_builtin");
	return (exit_status);
}

/* Helper for pipelines: creates pipes, forks, waits, returns last status */
static int	execute_pipeline_of_commands(t_cmd *cmd_list, t_env **env_list,
								  t_shell *shell, int num_cmds)
{
	pid_t	*pids_array;
	int		i;
	t_cmd	*current_cmd;
	t_cmd	*prev_cmd;
	int		pipeline_setup_error;
	int		exit_status_overall;

	pids_array = NULL;
	pipeline_setup_error = 0;
	exit_status_overall = 0;
	if (num_cmds == 0)
		return (0);
	pids_array = malloc(sizeof(pid_t) * num_cmds);
	if (!pids_array)
	{
		perror("minishell: malloc for pids_array");
		return (1); /* Allocation failure */
	}
	if (num_cmds > 1 && !create_pipes(cmd_list))
	{
		/* create_pipes should print its own error */
		free(pids_array);
		return (1); /* Pipe creation failure */
	}
	current_cmd = cmd_list;
	prev_cmd = NULL;
	i = 0;
	while (current_cmd && i < num_cmds)
	{
		pids_array[i] = fork();
		if (pids_array[i] == -1)
		{
			perror("minishell: fork in pipeline");
			pipeline_setup_error = 1;
			break ; /* Stop creating children */
		}
		if (pids_array[i] == 0) /* Child process */
			execute_pipeline_child(current_cmd, prev_cmd, shell);
		prev_cmd = current_cmd;
		current_cmd = current_cmd->next;
		i++;
	}
	close_all_pipes(cmd_list); /* Parent closes all its copies */
	if (pipeline_setup_error)
	{
		if (i > 0)
			wait_for_children(pids_array, i); /* Wait for already forked */
		exit_status_overall = 1; /* General error for pipeline setup */
	}
	else
		exit_status_overall = wait_for_children(pids_array, num_cmds);
	free(pids_array);
	return (exit_status_overall);
}

static int	execute_commands(t_cmd *cmd_list, t_env **env_list, t_shell *shell)
{
	t_cmd	*current_cmd_node;
	t_cmd	*prev_cmd_node;
	int		exit_status;
	int		stdin_backup;
	int		stdout_backup;
	pid_t	*pids_array;
	int		i_cmd_idx;
	int		num_cmds_total;
	char	*exit_status_str;
	int		pipeline_creation_error;  /* To be removed or localized */
	int		redirection_status_parent; /* To be removed or localized */

	/* Initialize variables */
	exit_status = 0;
	stdin_backup = -1;
	stdout_backup = -1;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
	{
		perror("minishell: dup initial fds");
		exit_status = 1;
	}

	if (exit_status == 0)
	{
		num_cmds_total = count_commands(cmd_list);
		if (num_cmds_total == 1 && cmd_list->args && cmd_list->args[0]
			&& is_parent_builtin(cmd_list->args[0]))
		{
			exit_status = manage_single_parent_builtin(cmd_list, env_list, shell,
												 stdin_backup, stdout_backup);
		}
		else if (num_cmds_total > 0) /* Pipelines or single child commands */
		{
			exit_status = execute_pipeline_of_commands(cmd_list, env_list, shell,
													 num_cmds_total);
		}
		/* If num_cmds_total is 0, exit_status remains 0 (no commands) */
	}

	/* Restore original FDs in parent if not already restored by builtin handler */
	/* (manage_single_parent_builtin already restores them) */
	/* However, for pipeline cases or if initial dup failed, ensure restoration. */
	if (stdin_backup != -1 && dup2(stdin_backup, STDIN_FILENO) == -1)
		perror("minishell: dup2 restore stdin final");
	if (stdout_backup != -1 && dup2(stdout_backup, STDOUT_FILENO) == -1)
		perror("minishell: dup2 restore stdout final");

	/* Cleanup */
	smart_close(&stdin_backup);
	smart_close(&stdout_backup);
	/* pids_array is freed within execute_pipeline_of_commands */
	close_all_pipes(cmd_list); /* Final safety close for pipes if any error paths missed it */

	/* Set $? */
	exit_status_str = ft_itoa(exit_status);
	if (exit_status_str)
	{
		set_env_value(env_list, "?", exit_status_str);
		free(exit_status_str);
	}
	else
		set_env_value(env_list, "?", "1"); /* Fallback if itoa fails */

	if (shell)
		shell->exit_status = exit_status;
	return (exit_status);
}

/*
** Executor main function.
** Initializes shell structure elements related to command execution
** and calls execute_commands.
*/
int	executor(t_cmd *cmd_list, t_env **env_list, t_shell *shell)
{
	if (!cmd_list)
		return (0); /* No commands to execute */
	/* Ensure shell pointer is valid */
	if (!shell)
	{
		ft_putstr_fd("minishell: Internal error: shell pointer is NULL in executor\n", STDERR_FILENO);
		return (1); /* Or some other error code */
	}
	/* Ensure env_list pointer is valid if needed by execute_commands */
	if (!env_list || !*env_list)
	{
		ft_putstr_fd("minishell: Internal error: env_list is NULL in executor\n", STDERR_FILENO);
		return (1);
	}
	shell->env_list = *env_list;
	shell->cmd_list = cmd_list;
	/* Do NOT reset shell->exit_status here */
	/* Call execute_commands with the correct shell pointer */
	return (execute_commands(cmd_list, env_list, shell));
}
