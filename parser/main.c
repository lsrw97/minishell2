#include <stdio.h>
#include "parser.h"
#include <unistd.h>

int	checkquotes(char *str)
{
	int sq;
	int dq;
	char *tmp;

	sq = 1;
	dq = 1;
	tmp = str;
	if (!tmp[0] || !tmp--)
		return (0);
	while (*(++tmp))
	{
		if (tmp[0] == '\"' && sq == 1)
		{
			if (dq == 1)
				dq = 0;
			else if (tmp[1] == ' ' || tmp[1] == '\0')
				dq = 1;
		}
		if (tmp[0] == '\'' && dq == 1)
		{
			if (sq == 1)
				sq = 0;
			else if (tmp[1] == ' ' || tmp[1] == '\0')
				sq = 1;
		}
	}
	return (sq && dq);
}

int	wordlendel(char *str, char del)
{
	int	len;

	len = 0;
	if (!str)
		return (0);
	while (str[len] != del && str[len])
		len++;
	return (len);
}

// this Funktion will count total amount of strings. Considered "" and ''

static int	totalstrings(char *str)
{
	int	i;
	int	total;

	i = 0;
	total = 0;
	// "'\"\|<>"
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] != '\"' && str[i] != '\'' && str[i] != '>' && str[i] != '<' && str[i] != '|' && str[i])
		{
			while (str[i] != ' ' && str[i] != '>' && str[i] != '<' && str[i] != '|' && str[i])
				i++;
			total++;
		}
		if (str[i] == '\"')
		{
			i++;
			while (str[i] != '\"' && str[i]) // '0123"hello" my name is "Albert     and I love  " pan   cakes      '
				i++;
			total++;
			i++;
		}
		if (str[i] == '\'')
		{
			i++;
			while (str[i] != '\'' && str[i]) // '0123"hello" my name is "Albert     and I love  " pan   cakes      '
				i++;
			total++;
			i++;
		}
		if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
		{
			i+= 2;
			total++;
		}
		else if (str[i] == '>' || str[i] == '<' || str[i] == '|')
		{
			i++;
			total++;
		}
	}
	return (total);
}

void	fillsplit(char **split, char *str)
{
	int	i;
	int	j;
	int	x;

	x = 0;
	j = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] != '\"' && str[i] != '\'' && str[i] != '>' && str[i] != '<' && str[i] != '|' && str[i])
		{
			split[j] = malloc(wordlendel(&str[i], ' ') + 1);
			// printf("wordlen: %d\n\n", wordlendel(&str[i], ' ') + 1);
			while (str[i] != ' ' && str[i] != '>' && str[i] != '<' && str[i] != '|' && str[i])
			{
				split[j][x++] = str[i++];
				// printf("split[j]: %s, j: %d, i: %d, x: %d\n", split[j], j, i, x);
			}
			split[j][x] = '\0';
			j++;
			x = 0;
		}
		if (str[i] == '\"')
		{
			i++;
			split[j] = malloc(wordlendel(&str[i], '\"') + 1);
			// printf("wordlen: %d\n\n", wordlendel(&str[i], '\"') + 1);
			while (str[i] != '\"' && str[i]) // '0123"hello" my name is "Albert     and I love  " pan   cakes      '
			{
				split[j][x++] = str[i++];
				// printf("split[j]: %s, j: %d, i: %d\n", split[j], j, i);
			}			
			split[j][x] = '\0';
			// printf("split[j]: %s, j: %d\n", split[j], j);
			x = 0;
			j++;
			i++;
		}
		if (str[i] == '\'')
		{
			i++;
			split[j] = malloc(wordlendel(&str[i], '\'') + 2);
			// printf("wordlen: %d\n\n", wordlendel(&str[i], '\'') + 1);
			while (str[i] != '\'' && str[i]) // '0123"hello" my name is "Albert     and I love  " pan   cakes      '
			{
				split[j][x++] = str[i++];
				// printf("split[j]: %s, j: %d, i: %d\n", split[j], j, i);
			}
			split[j][x++] = '\"';
			split[j][x] = '\0';
			// printf("split[j]: %s, j: %d\n", split[j], j);
			x = 0;
			j++;
			i++;
		}
		if ((str[i] == '>' && str[i + 1] == '>'))
		{
			split[j] = malloc(3);
			split[j][0] = '>';
			split[j][1] = '>';
			split[j][2] = '\0';
			// printf("split[j]: %s, j: %d, i: %d\n", split[j], j, i);
			i += 2;
			j++;
		}
		else if ((str[i] == '<' && str[i + 1] == '<'))
		{
			split[j] = malloc(3);
			split[j][0] = '<';
			split[j][1] = '<';
			split[j][2] = '\0';
			i += 2;
			j++;
		}
		else if (str[i] == '>')
		{
			split[j] = malloc(2);
			split[j][0] = '>';
			split[j][1] = '\0';
			i++;
			j++;
		}
		else if (str[i] == '<')
		{
			split[j] = malloc(2);
			split[j][0] = '<';
			split[j][1] = '\0';
			i++;
			j++;
		}
		else if (str[i] == '|')
		{
			split[j] = malloc(2);
			split[j][0] = '|';
			split[j][1] = '\0';
			i++;
			j++;
		}
	}
}

char	**minisplit(char *str)
{
	int		count;
	char	**split;

	if (!str || !ft_strlen(str))
		return (NULL);
	count = totalstrings(str);
	split = malloc(sizeof(char *) * (count + 1));
	split[count] = NULL;
	if (!split)
		return (NULL);
	fillsplit(split, str);
	return (split);
}

int	checkforenv(char **envp, char *env)
{
	int	i;
	int	len;

	len = ft_strlen(env);
	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], env, len) && envp[i][len] == '=')
			return (i);
	}
	return (-1);
}

int	envlength(char *env)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (env[i] != '=')
		i++;
	while (env[++i])
		j++;
	return (j);
}

s_string	*newnode(char *value, s_string *prev)
{
	s_string	*list;

	list = malloc(sizeof(s_string));
	if (!list)
		return (NULL);
	list->value = value;
	list->next = NULL;
	list->prev = prev;
	return (list);
}
    // COMMAND,		// 0	cat, /bin/cat
    // REDIRECTION,	// 1	> >> < <<
    // PIPE,			// 2	|
    // ARGUMENT,		// 3	word

// Command can be represented as path, absolute or relative or name.


void	freesplit(char **split)
{
	int	i;

	i = -1;
	if (!split)
		return ;
	while (split[++i])
		free(split[i]);
	free(split);
}

char	*strappendpath(char *str, char *value)
{
	int		i;
	int 	len;
	char	*string;
	int		j;

	j = -1;
	len = ft_strlen(str) + ft_strlen(value) + 2;
	string = malloc(len);
	i = -1;
	while (str[++i])
		string[++j] = str[i];
	string[++j] = '/';
	i = -1;
	while (value[++i])
		string[++j] = value[i];
	string[++j] = '\0';
	free(str);
	return (string);
}

char	*checkforcommand(char *value, char **envp)
{
	char	**paths;
	char	*tmp;
	// char	*str;

	int i = -1;
	if (!value[0])
		return (NULL);
	if (!ft_strncmp(value, "export", ft_strlen(value) + 1) || !ft_strncmp(value, "unset", ft_strlen(value) + 1) || !ft_strncmp(value, "env", ft_strlen(value) + 1) || !ft_strncmp(value, "exit", ft_strlen(value) + 1) || !ft_strncmp(value, "cd", ft_strlen("cd") + 1))
		return (value);
	if (access(value, X_OK) == 0)
		return (value);
	if (!getenv("PATH"))
		return (NULL);
	paths = ft_split(getenv("PATH"), ':');
	// str = ft_strjoin(paths[i], "/");
	// str = ft_strjoin(paths[i], value);
	while (paths[++i])
		paths[i] = strappendpath(paths[i], value);
	// i = -1;
	// while (paths[++i])
	// 	printf("%s\n", paths[i]);
	i = -1;
	while (paths[++i])
	{
		if (access(paths[i], X_OK) == 0)
		{
			// printf("%s\n", paths[i]);
			tmp = malloc(ft_strlen(paths[i]) + 1);
			ft_strlcpy(tmp, paths[i], ft_strlen(paths[i]) + 1);
			// printf("tmp: %s\n", paths[i]);
			freesplit(paths);
			return (tmp);
		}
	}
	freesplit(paths);
	return (NULL);
}

void	setnodetype(s_string *node, char **envp)
{
	if (node->value[0] == '|' && ft_strlen(node->value) == 1)
		node->nodeType = PIPE;
	else if (node->value[0] == '>' || !ft_strncmp(node->value, ">>", 2) || node->value[0] == '<' || !ft_strncmp(node->value, "<<", 2))
		node->nodeType = REDIRECTION;
	else if (checkforcommand(node->value, envp))
		node->nodeType = COMMAND;
	else
		node->nodeType = ARGUMENT;
}

int	checkforenvinnode(char *string)
{
	int	i;

	i = -1;
	while (string[++i])
		if (string[i] == '$')
			return (1);
	return (0);
}

int	lenvar(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\"' && str[i] != '$' && str[i] != ' ' && str[i] != '\n' && ft_isalpha(str[i]))
	{
		i++;
	}
	return (i);
}

int	getindexenvp(char *str, char **envp)
{
	int		i;
	char	*tmp;

	i = -1;
	tmp = malloc(lenvar(str) + 1);
	ft_strlcpy(tmp, str, lenvar(str) + 1);
	// printf("tmp: %s\n", tmp);
	while (envp[++i])
	{
		if (!strncmp(envp[i], tmp, lenvar(str)) && envp[i][ft_strlen(tmp)] == '=')
		{
			free(tmp);
			return (i);
		}
	}
	free(tmp);
	return (-1);
}

int	getlenexpandedvar(char *str, char **envp)
{
	int		i;
	char	*tmp;

	i = -1;
	tmp = malloc(lenvar(str) + 1);
	ft_strlcpy(tmp, str, lenvar(str) + 1);
	// printf("tmp: %s\n", tmp);
	while (envp[++i])
	{
		if (!strncmp(envp[i], tmp, lenvar(str)) && envp[i][ft_strlen(tmp)] == '=')
		{
			// printf("%s\n", envp[i]);
			// free(tmp);
			i = ft_strlen(envp[i]) - lenvar(tmp) - 1;
			free(tmp);
			// printf("len: %d\n", i);
			return (i);
		}
	}
	free(tmp);
	return 0;
}

int getlenexpandedvars(char *str, char **envp)
{
	int	i;
	int	len;
	int	x;

	x = 0;
	len = 0;
	i = 0;
	if (!str[i])
		return (0);
	while (str[i] && str[i] != '\"')
	{
		if (str[i] == '$')
		{
			i++;
			if (!str[i])
				break ;
			if (!ft_isalpha(str[i]))
			{
				++i;
				x += 2;
				continue ;
			}
			// if (getlenexpandedvar(&str[i], envp))
			// printf("lenvar: %d, x: %d, %d , %s\n", len, x, ft_strlen(str), &str[i]);
			x += lenvar(&str[i]) + 1;
			len += getlenexpandedvar(&str[i], envp);
			while (str[i] && str[i] != ' ' && str[i] != '$')
				i++;
			continue ;
		}
		i++;
	}
	// printf("lenvar: %d, x: %d, %d , %s\n", len, x, ft_strlen(str), &str[i]);
	// printf("%d\n", ft_strlen(str) - x + len);
	return (ft_strlen(str) - x + len);
}

// 			"argument something $PATH PATH $PATH"   / 35
//			expanded its 							/ 35 + 134 * 2 - 2 * $!something$ home42eugenhome42-

// char	*getexpandedvar(char *envp, )
// {
// 	int		i;
// 	char	*tmp;
	
// 	i = -1;
// 	while ([])
// }

void	expandvars(s_string *node, char **envp)
{
	char	*tmp;
	int		i;
	int		j;
	int		t;

	i = -1;
	t = 0;
	if (!node->value[0])
		return ;
	
	tmp = malloc(getlenexpandedvars(node->value, envp) + 1);
	// printf("len path: %d\n", getlenexpandedvars(node->value, envp) + 1);
	while (node->value[++i] && node->value[i] != '\"')
	{
		j = -1;
		if (node->value[i] == '$')
		{
			// if (node->value[i + 1] == '$')
			// {
			// 	i++;
			// 	continue ;
			// }
			if (node->value[i + 1] == ' ' || node->value[i + 1] == '\0')
			{
				tmp[t++] = node->value[i];
				continue ;
			}
			if (!ft_isalpha(node->value[i + 1]))
			{
				i++;
				continue ;
			}
			// else if (!ft_isalpha(node->value[i + 1]))
			// {
			// 	while (node->value[i + 1] && node->value[i] != ' ' && node->value[i] != '$')
			// 	{
			// 		tmp[t++] = node->value[i];
			// 		i++;
			// 	}
			// 	if (!node->value[i])
			// 		break ;
			// }
			else if (getlenexpandedvar(&node->value[++i], envp) && getindexenvp(&node->value[i], envp) >= 0)
			{
				while (envp[getindexenvp(&node->value[i], envp)][j] != '=')
					j++;
				while (envp[getindexenvp(&node->value[i], envp)][++j])
				{
					tmp[t++] = envp[getindexenvp(&node->value[i], envp)][j];
					// printf("%d, %c\n", j, envp[getindexenvp(&node->value[i], envp)][ j]);
				}
			}
			while (node->value[i] && node->value[i] != ' ' && node->value[i] != '$')
				i++;
			if (!node->value[i])
				break ;
		}
		if (node->value[i] == '$')
		{
			--i;
			continue ;
		}	
		tmp[t] = node->value[i];
		t++;		
	}
	tmp[t] = node->value[i];
	// tmp[++t] = '\0';
	// printf("tmp: %s\n", tmp);
	node->value = malloc(ft_strlen(tmp) + 1);
	ft_strlcpy(node->value, tmp, ft_strlen(tmp) + 1);
	node->value[ft_strlen(tmp)] = '\0';
	free(tmp);
	// printf("%s\n", node->value);
}

// Think about the expanding of the $VARS

s_string	*createlist(char **split)
{
	int			i;
	s_string 	*tmp;
	s_string	*stack;

	if (!split)
		return NULL;
	stack = newnode(split[0], NULL);
	tmp = stack;
	i = 0;
	while (split[++i])
	{
		tmp->next = newnode(split[i], tmp);
		tmp = tmp->next;
	}
	tmp->next = NULL;
	return (stack);
}

// PATH=/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:/System/Cryptexes/App/usr/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Library/Apple/usr/bin |'eelisaro grep' eelisaroeelisaro

void	printtype(int type)
{
	if (!type)
		printf("[COMMAND]	");
	else if (type == 1)
	
		printf("[REDIRECTION]	");
	else if (type == 2)
		printf("[PIPE]		");
	else if (type == 3)
		printf("[ARGUMENT]	");
}

char	**cmdwithargs(s_string *list)
{
	s_string	*tmp;
	char		**str;
	int			i;
	int			j;

	tmp = list;
	i = 0;
	j = 0;
	while (list && list->nodeType != PIPE)
	{
		i++;
		list = list->next;
	}
	// printf("i: %d\n", i);
	str = malloc(sizeof(char *) * (i + 1));
	while (i-- > 0)
	{
		str[j++] = tmp->value;
		tmp = tmp->next;
	}
	str[j] = NULL;
	// printf("%s, %s, %d\n", str[0], str[1], j);
	return (str);
}

// char	*getcmdpath(char *value)
// {
// 	char	**paths;
// 	int		i;

// 	i = -1;
// 	paths = ft_split(getenv("PATH"), ':');

// 	// str = ft_strjoin(paths[i], "/");
// 	// str = ft_strjoin(paths[i], value);
// 	while (paths[++i])
// 		paths[i] = strappendpath(paths[i], value);
// 	return 
// }

int main(int argc, char **argv, char **envp)
{
	s_string	*list;
	s_string	*tmp;
	char		**ss;
	int			id;
	int			i;

	list = NULL;
	i = -1;

    while (1)
    {
    	char *input = readline("minishell >$ ");
		ss = minisplit(input);
		list = createlist(ss);
		tmp = list;
		while (list)
		{
			if (list->value[ft_strlen(list->value) - 1] != '\"')
				expandvars(list, envp);
			setnodetype(list, envp);
			// printtype(list->nodeType);
			// printf("%s\n", list->value);
			// char **str = {"echo", "hello", "world", NULL};
			// execve("/bin/echo", str, envp);
			list = list->next;
		}
		id = fork();
		if (id != 0)
			wait(0);
		else
			execve(checkforcommand(tmp->value, envp), cmdwithargs(tmp), envp);
        if (!input) 
            break ;
        free(input);
        rl_on_new_line();
    }
	freesplit(ss);
}


// int main(int argc, char **argv, char **envp)
// {
//     // if (argc != 2)
//     //     return 0;
// 	char *s = "echo export cd \'$USER\' $ my name is grep>><<><>0  Makefile  \"|\'$USER grep\' $USER$USER $$\"| \'grep x@ $PATH\' >| file $USE | $USER$user$1eugen$USER$1USER--$-";
// 	// char *s = "$1hello world";
// 	// char *s = "<";
// 	// if(!checkquotes(s))
// 	// 	return 0; 
// 	s_string *list;
// 	list = NULL;
// 	int i = -1;
// 	char **ss;
// 	// ss = minisplit(s);
// 	// while (ss[++i])
// 	// 	printf("%s \n", ss[i]);
// 	i = -1;
// 	// while (envp[++i])
// 	// 	printf("%s\n", envp[i]);
// 	// printf("%d, len: %d\n", checkforenv(envp, "PATH"), envlength(envp[3]));home42eugenhome42$
// 	// printf("%s", s);
// 	// printf("\n\n");
// 	// list = createlist(ss);
// 	// // printf("%s\n", list->value);
// 	// while (list)
// 	// {
// 	// 	if (list->value[ft_strlen(list->value) - 1] != '\"')
// 	// 		expandvars(list, envp);
// 	// 	setnodetype(list, envp);
// 	// 	// printf("%d is file ", checkforcommand(list->value, envp));
// 	// 	printtype(list->nodeType);
// 	// 	printf("%s\n", list->value);
// 	// 	list = list->next;
// 	// }
// 	// printf("%s\n", getenv("PATH"));

// 	// printf("%d length\n", getlenexpandedvars("$PATH", envp));


// 	int id;
//     while (1)
//     {
//     	char *input = readline("minishell >$ ");
// 		ss = minisplit(input);
// 		list = createlist(ss);
// 		// while (list)
// 		// {
// 			if (list->value[ft_strlen(list->value) - 1] != '\"')
// 				expandvars(list, envp);
// 			setnodetype(list, envp);
// 			// printf("%d is file ", checkforcommand(list->value, envp));
// 			printtype(list->nodeType);
// 			printf("%s\n", list->value);
// 			char **str = cmdwithargs(list);
// 			execve("/bin/echo", str, envp);
// 			list = list->next;
// 		// }
//         if (!input) 
//             // If the user entered EOF (Ctrl+D), exit the loop
//             break ;
//         free(input);
//         rl_on_new_line();
//     }
// 	freesplit(ss);
// 	// printf("%d\n", checkforargument("cat", envp));
// }

// ok wir haben alle elemente mit expanded vars in einer liste
// jetzt müssen wir denen noch Tokens geben. 
// isfile
// iscmd
// isargument
// isempty

// ◦ echo with option -n
// ◦ cd with only a relative or absolute path ◦ pwd with no options
// ◦ export with no options
// ◦ unset with no options
// ◦ env with no options or arguments
// ◦ exit with no options
// what is the exit status?
// extra type for buildins?

// $USER$user$1eugen$USER-

// $USER$user$1eugen$USER$ home42home42