#include <stdio.h>
#include "parser.h"
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
	s_string	*list;
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
		while (list)
		{
			if (list->value[ft_strlen(list->value) - 1] != '\"')
				expandvars(list, envp);
			setnodetype(list, envp);
			printtype(list->nodeType);
			printf("%s\n", list->value);
			// char **str = cmdwithargs(list);
			// execve("/bin/echo", str, envp);
			list = list->next;
		}
        if (!input) 
            break ;
        free(input);
        rl_on_new_line();
    }
	freesplit(ss);
}