#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.c"

// typedef struct movie {
//     char *tconst;
//     char *primaryTitle;
//     struct movie *next;
//     struct neighbor *node;
//     // TODO :: criar lista de vizinhos
// } movie;

typedef struct neighbor {
    struct movie movie;
    struct neighbor * next;
} neighbor;


// Estrutura para representar um ator
struct actor {
    char *nconst;
    char *name;
    struct movie *movies;  // Lista encadeada de filmes de destaque
};


struct adjacency_list {
    struct movie **nodes;  // Array de nós da lista de adjacências
    size_t size;
    size_t capacity;
};

char* remove_quotes(char* str) {
    char* result = strdup(str);
    char* src = str;
    char* dst = result;
    while (*src) {
        if (*src != '\"') {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
    return result;
}

// Função para liberar a memória alocada para um ator, incluindo sua lista de filmes
void free_actor(struct actor *actor) {
    free(actor->nconst);
    free(actor->name);
    free(actor);
}

// Função de exemplo para imprimir os dados de um ator e seus filmes
void print_actor_info(struct actor *actor) {
    printf("nconst: %s\n", actor->nconst);
    printf("name: %s\n", actor->name);

    printf("Movies:\n");
    struct movie *current_movie = actor->movies;
    while (current_movie != NULL) {
        printf("  tconst: %s\n", current_movie->tconst);
        printf("  primaryTitle: %s\n", current_movie->primaryTitle);
        current_movie = current_movie->next;
    }
}

int actor_movie_list() {
    const char* filename = "arquivos/name.basics.tsv";
    printf("Attempting to open file: %s\n", filename);

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, sizeof(line), file);  // Skip header line

    // Array dinâmico para armazenar os atores
    struct actor **actors_array = malloc(10 * sizeof(struct actor *));
    if (actors_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int num_actors = 0;
    while (fgets(line, sizeof(line), file) && num_actors < 10) {
        struct actor *current_actor = malloc(sizeof(struct actor));
        if (current_actor == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        current_actor->nconst = strdup(strtok(line, "\t"));
        current_actor->name = strdup(strtok(NULL, "\t"));

        // Inicializa a lista de filmes como vazia
        current_actor->movies = NULL;

        // Leitura dos filmes de destaque
        char *t1 = strtok(NULL, "\t");  // Token para os filmes
        char *t2 = strtok(NULL, "\t");  // Token para os filmes
        char *t3 = strtok(NULL, "\t");  // Token para os filmes
        char *movies_token = strtok(NULL, "\t");  // Token para os filmes
        printf("%s", movies_token);
        while (movies_token != NULL) {
            char *token = strtok(movies_token, ",");
            // Itera sobre cada token e imprime o ID do filme
            while (token != NULL) {
                printf("Movie ID: %s\n", token);
                token = strtok(NULL, ",");
            }
            movies_token = strtok(NULL, "\t");
        }

        actors_array[num_actors++] = current_actor;
    }

    fclose(file);

    // // Imprime informações dos atores
    // for (int i = 0; i < num_actors; i++) {
    //     printf("Actor %d:\n", i + 1);
    //     print_actor_info(actors_array[i]);
    //     printf("\n");
    // }

    // Libera memória alocada
    for (int i = 0; i < num_actors; i++) {
        free_actor(actors_array[i]);
    }
    free(actors_array);

    return 0;
}

// Função de hashing para mapear o tconst do filme para um índice na lista de adjacências
size_t hash_function(const char *key, size_t size) {
    size_t hash = 0;
    size_t len = strlen(key);
    for (size_t i = 0; i < len; i++) {
        hash += key[i];
    }
    return hash % size;
}

void init_adjacency_list(struct adjacency_list *adj_list, size_t initial_capacity) {
    adj_list->nodes = malloc(initial_capacity * sizeof(struct movie *));
    if (adj_list->nodes == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    adj_list->size = 0;
    adj_list->capacity = initial_capacity;

    for (size_t i = 0; i < initial_capacity; i++) {
        adj_list->nodes[i] = NULL;
    }
}

// Função para redimensionar a lista de adjacências de filmes
void resize_adjacency_list(struct adjacency_list *adj_list) {
    size_t new_capacity = adj_list->capacity * 2;
    struct movie **new_nodes = realloc(adj_list->nodes, new_capacity * sizeof(struct movie *));
    if (new_nodes == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    adj_list->nodes = new_nodes;
    adj_list->capacity = new_capacity;

    // Inicializa novos slots com NULL
    for (size_t i = adj_list->size; i < new_capacity; i++) {
        adj_list->nodes[i] = NULL;
    }
}

// Função para adicionar um filme à lista de adjacências
void add_movie_to_adj_list(struct adjacency_list *adj_list, struct movie *new_movie) {
    if (adj_list->size == adj_list->capacity) {
        resize_adjacency_list(adj_list);
    }

    size_t index = hash_function(new_movie->tconst, adj_list->capacity);
    new_movie->next = adj_list->nodes[index];
    adj_list->nodes[index] = new_movie;
    adj_list->size++;
}

// Função para liberar a memória alocada
void free_adjacency_list(struct adjacency_list *adj_list) {
    for (size_t i = 0; i < adj_list->capacity; i++) {
        struct movie *current = adj_list->nodes[i];
        while (current != NULL) {
            struct movie *temp = current;
            current = current->next;
            free(temp->tconst);
            free(temp->primaryTitle);
            // free(temp->node);
            free(temp->next);
            free(temp);
        }
    }
    free(adj_list->nodes);
}

// Função para ler e mostrar os filmes
void read_and_print_movies(node * tree, const char *filename, struct adjacency_list *adj_list) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        printf("Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, sizeof(line), file);  // Skip header line

    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        char *tconst = strtok(line, "\t");
        char *titleType = strtok(NULL, "\t");
        char *primaryTitle = strtok(NULL, "\t");
        char *originalTitle = strtok(NULL, "\t");
        int isAdult = atoi(strtok(NULL, "\t"));
        int startYear = atoi(strtok(NULL, "\t"));
        char *endYear = strtok(NULL, "\t");
        char *runtimeMinutes = strtok(NULL, "\t");
        char *genres = strtok(NULL, "\t");

        // Cria um novo filme
        struct movie *new_movie = malloc(sizeof(struct movie));
        //printf("%p", new_movie);
        if (new_movie == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        if (strncmp(tconst, "tt", 2) == 0) {
            // Remove o prefixo "tt" deslocando o ponteiro
            tconst += 2;
        }

        new_movie->tconst = strdup(tconst);
        new_movie->primaryTitle = strdup(primaryTitle);
        // Lista de Filmes Vizinhos 
        new_movie->next = NULL;
        // new_movie->node = NULL;

        // Adicionando o elemento na arvore
        insert(&tree, atoi(new_movie->tconst), (void *) new_movie);

        add_movie_to_adj_list(adj_list, new_movie);

        // printf("tconst: %s\n", new_movie->tconst);
        // printf("primaryTitle: %s\n", new_movie->primaryTitle);
        // printf("\n");

        count++;
    }
    fclose(file);
}

void print_adj_list(struct adjacency_list *adj) {
    FILE *output_file = fopen("input.dot", "w");
    if (output_file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(output_file, "graph { concentrate=true\n");

    for (int i = 0; i < adj->capacity; i++) {
        movie *current = adj->nodes[i];

        while (current != NULL) {
            if (current->next != NULL) {
                fprintf(output_file, "\"%s\" -- \"%s\"\n", current->primaryTitle, current->next->primaryTitle);
            }
            current = current->next;
        }
    }

    fprintf(output_file, "}\n");
    fclose(output_file);
}



int main() {
    const char *filename = "arquivos/title.basics.tsv";
    printf("Attempting to open file: %s\n", filename);

    node * tree = NULL;

    struct adjacency_list movies_adj_list;
    init_adjacency_list(&movies_adj_list, 100);  // Inicializa com capacidade de 100

    read_and_print_movies(tree, filename, &movies_adj_list);

    print_adj_list(&movies_adj_list);

    free_adjacency_list(&movies_adj_list);

    //actor_movie_list();

    return 0;
}
