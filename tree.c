#include <stdio.h>
#include <stdlib.h>

struct node;
typedef struct node node;


typedef struct movie {
    char *tconst;
    char *primaryTitle;
    struct movie *next;
    //struct neighbor *node;
    // TODO :: criar lista de vizinhos
} movie;

struct node
{
	int    idFilme;
    movie ** index;
	int    level;
	node * left;
	node * right;
};

void visit(node * n)
{
	if(n)
	{
		visit(n->left);
		printf("%d %d", n->idFilme, n->index);
        printf("\n");
		visit(n->right);
	}
}

// int search(node * n, int id){
//     if(!n) {
//         printf("Árvore vazia.");
//         return -1;
//     }

//     if (id == n->idFilme){
//         return n->index;
//     } 
//     if (id < n->idFilme) {
//         search(n->left, id);
//     }
//     if(id > n->idFilme){
//         search(n->right, id);
//     }
// }

void destruct(node * n)
{
	if(n)
	{
		destruct(n->left);
		destruct(n->right);
		free(n);
	}
}

int get_weight(node * n)
{
	if(!n)
	{
		return 0;
	}
	int const left_level  = n->left  ? n->left->level  : -1;
	int const right_level = n->right ? n->right->level : -1;
	return right_level - left_level;
}

void update_level(node * n)
{
	if(n)
	{
		int const left_level  = n->left  ? n->left->level  : -1;
		int const right_level = n->right ? n->right->level : -1;
		int const max_level = left_level > right_level ? left_level : right_level;
		n->level = max_level + 1;
	}
}

node * rotate_left(node *, int);
node * rotate_right(node *, int);

node * rotate_left(node * n, int weight)
{
	if(!n)
	{
		return NULL;
	}
	
	int const right_weight = get_weight(n->right);
	
	if((weight < 0 && right_weight > 0) || (weight > 0 && right_weight < 0))
	{
		n->right = rotate_right(n->right, right_weight);
	}
	
	node * temp = n->right;
	n->right = temp->left;
	temp->left = n; 
	
	update_level(temp->left);
	update_level(temp);
	
	return temp;
}

node * rotate_right(node * n, int weight)
{
	if(!n)
	{
		return NULL;
	}
	
	int const left_weight = get_weight(n->left);
	
	if((weight < 0 && left_weight > 0) || (weight > 0 && left_weight < 0))
	{
		n->left = rotate_left(n->left, left_weight);
	}
	
	node * temp = n->left;
	n->left = temp->right;
	temp->right = n;
	
	update_level(temp->right);
	update_level(temp);
	
	return temp;
}

void balance(node ** n)
{
	if(*n)
	{
		int const weight = get_weight(*n);
		if(weight > 1)
		{
			*n = rotate_left(*n, weight);
		}
		else if(weight < -1)
		{
			*n = rotate_right(*n, weight);
		}
		else
		{
			update_level(*n);
		}
	}
}

void insert_node(node ** n, node * m)
{
	if(!m)
	{
		return;
	}
	
	if(!*n)
	{
		*n = m;
		return;
	}
	
	if(m->idFilme < (*n)->idFilme)
	{
		insert_node(&(*n)->left, m);
	}
	else
	{
		insert_node(&(*n)->right, m);
	}
	
	balance(n);
}

void insert(node ** n, int id, movie ** index)
{
	node * m = (node *) malloc(sizeof(node));
	if(m)
	{
		m->idFilme  = id;
        m->index = index;
		m->level = 0;
		m->left  = NULL;
		m->right = NULL;
	}
	insert_node(n, m);
}

void erase(node ** n, int id)
{
	if(!(*n))
	{
		return;
	}
	
	if(id < (*n)->idFilme)
	{
		erase(&(*n)->left, id);
	}
	else if(id > (*n)->idFilme)
	{
		erase(&(*n)->right, id);
	}
	else
	{
		insert_node(&((*n)->left), (*n)->right);
		node * temp = (*n)->left;
		free(*n);
		(*n) = temp;
	}
	
	balance(n);
}