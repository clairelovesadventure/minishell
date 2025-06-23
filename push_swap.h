#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdbool.h> //To use bool flags, e.g, to print or not to print
# include <limits.h> //To define MIN and MAX macros
# include <stdlib.h>
# include <stddef.h>
# include <unistd.h> //To use write() function

typedef struct s_stack_node //A container of data enclosed in {} braces. `s_` for struct
{
	int					nbr; //The number to sort
	int					current_position; //The number's position in the stack
	int					push_cost; //How many commands in total
	bool				above_median; //Used to calculate `push_cost`
	bool				cheapest; //The node that is the cheapest to do commands
	struct s_stack_node	*target_node; //The target node of a node in the opposite stack
	struct s_stack_node	*next; //A pointer to the next node
	struct s_stack_node	*prev; //A pointer to the previous node
}	t_stack_node; //The "shortened name", "t_stack_node". `t_` for type

//***Input check
long	ft_atol(const char *str);
bool	check_input(char *str);
bool    check_duplicates(t_stack_node *stack);
void	error_exit(t_stack_node **a, t_stack_node **b,
	char **split, bool is_split);
char	**ft_split(char const *str, char c);

//***Stack initiation
void		init_stack_a(t_stack_node **a, char **argv); //Initiate stack `a` before processing
void		init_stack_b(t_stack_node *a, t_stack_node *b);
int			stack_init_a(t_stack_node **a, char **argv, bool is_split);
char		**split(char *s, char c); //To handle input of numbers as a string argument, e.g. enclosed in " "

//***Nodes initiation
void			init_nodes_a(t_stack_node *a, t_stack_node *b); //To prep all nodes for pushing `a` to `b`
void			init_nodes_b(t_stack_node *a, t_stack_node *b); //To prep all nodes for pushing `b` back to `a`
void			set_current_position(t_stack_node *stack); //Set the node's current index
void			set_price(t_stack_node *a, t_stack_node *b);  // Now takes two args
void			set_cheapest(t_stack_node *stack); //Set the stack's cheapest node
t_stack_node	*return_cheapest(t_stack_node *stack);//Get the cheapest node of a stack
void			prep_for_push(t_stack_node **s, t_stack_node *n, char c); //Prep the required nodes on top for pushing 
t_stack_node	*find_smallest(t_stack_node *stack); // Returns the node with the smallest `nbr`


//***Stack utils
void			free_stack(t_stack_node **stack);
void			free_split(char **split);
int				stack_len(t_stack_node *stack); //Calculate the length of a stack
t_stack_node	*find_last_node(t_stack_node *stack); //Find the last node of a stack
bool			stack_sorted(t_stack_node *stack); //To check whether a stack is sorted
t_stack_node	*find_smallest(t_stack_node *stack); //Find the smallest number
t_stack_node	*find_max_node(t_stack_node *stack); //Find the biggest number
void			*ft_calloc(size_t count, size_t size);
char			*ft_strdup(const char *src);
int 			ft_strlen(const char *str);
char			*ft_substr(char const *string, unsigned int start, size_t len);

//***Commands
void			sa(t_stack_node **a, bool print);
void			sb(t_stack_node **b, bool print);
void			ss(t_stack_node **a, t_stack_node **b, bool print);
void			ra(t_stack_node **a, bool print);
void			rb(t_stack_node **b, bool print);
void			rr(t_stack_node **a, t_stack_node **b, bool print);
void			rra(t_stack_node **a, bool print);
void			rrb(t_stack_node **b, bool print);
void			rrr(t_stack_node **a, t_stack_node **b, bool print);
void			pa(t_stack_node **a, t_stack_node **b, bool print);
void			pb(t_stack_node **b, t_stack_node **a, bool print);

//***Algorithm
void	sort_three(t_stack_node **a);
void	sort_stacks(t_stack_node **a, t_stack_node **b); //Turk algorithm

#endif