#ifndef MODULE_H
#define MODULE_H

#include <stdlib.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../ft_printf/libft/libft.h"

typedef enum s_token
{
    COMMAND,		// 0	cat, /bin/cat
    REDIRECTION,	// 1	> >> < <<
    PIPE,			// 2	|
    ARGUMENT,		// 3	word
}	s_token;

typedef struct s_string
{
    struct s_string	*next;
    struct s_string	*prev;
    int		        nodeType;
	char	        *value;
}	s_string;

typedef struct s_treeNode
{
    void	*parent;
    void	*left;
    void	*right;
    int		nodeType;
	char	*value;
}	s_treeNode;

#endif

