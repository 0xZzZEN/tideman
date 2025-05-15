#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j 
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void swap(pair *ppairs_i, pair *ppairs_ipl1);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// dfs
int dfs(void);
void StackPush(int stack[], int *top, int element);
void StackPop(int stack[], int *top);
int StackFindEl(int stack[], int element);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // if find candidate name in an array of candidates, than rank of i-th candidate
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i; // example: candidates[Bob, Alice]: ranks[0] - rank 1, ranks[1] - rank 2: [0, 1]
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    /* find pos of candidate. If pos is less, than others, then it's preferenced, else it's not **
    ** ex: Rank 0 alice, 1 - bob, 2 - charlie; ranks[3] == [0,1,2]; preferences[0][1] +=1; preferences[0][2] +=1; preferences[1][2]
    +=1; **
    ** ex 2: Rank 1 - alice, 2 - bob; ranks[2] == [0,1]; preferences[0,1] +=1;
    ** ex 3 best: ranks[2]: ranks[0,1]; 0 prefered over 1, meaning preferences[0,1] += 1;
    ** ranks[i] == i-th preference : if rank and (for i): i > i+1, then update preferences[Alice][Alice+1]
    **debug: Bob Alice ranks[1,0]
    **Number of voters: 2
    Preferences[0,0]: 0
    Preferences[0,1]: 0
    Preferences[1,0]: 2 **
    Preferences[1,1]: 0
    */
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // ranks[3] : [0,1,2] [1,0,2] [2,0,1] [2,1,0] [0,2,1] [1,2,0]
            // rank 1 = i; rank 2 = j
            if (i < j)
            {
                preferences[ranks[i]][ranks[j]] += 1;
            }
            /*printf("Preferences[%i][%i]: %i\n", ranks[i], ranks[j], preferences[ranks[i]][ranks[j]]); //debug*/
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if ((preferences[i][j] != 0) && (preferences[i][j] != preferences[j][i]))
            {
                // pair consists of winner and loser (count +1), pairs is an array

                /*Preferences all from 3 votes: [i,j]
                Preferences[0][0]: 0 X
                Preferences[0][2]: 1 V win 2; lose 0
                Preferences[0][1]: 2 V win 0; lose 1
                Preferences[2][0]: 2 V win 2; lose 0
                Preferences[2][2]: 0 X
                Preferences[2][1]: 2 V win 2; lose 1
                Preferences[1][0]: 1 V win 0; lose 1
                Preferences[1][2]: 1 V win 2; lose 1
                Preferences[1][1]: 0 X*/
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count += 1;
                    /*debug printf("prepairs[%i].winner: %i\n", pair_count, i);
                    printf("prepairs[%i].loser: %i\n", pair_count, j);
                    */
                }
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory (bubble sort)
void sort_pairs(void)
{
    int strongest;
    for (int i = 0; i < pair_count - 1; i++) // move boundaries of unsorted 1 by 1 each iteration
    {
        // find strongest victory in the unsorted array
        strongest = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] != preferences[pairs[j].winner][pairs[j].loser])
            {
                if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
                {
                    strongest = j;
                    // swap strongest element with the first element of the unsorted part
                    if (strongest != i)
                    {
                        swap(&pairs[i], &pairs[strongest]);
                    }
                }
            }
        }
    }
    /*for (int i = 0; i < pair_count; i++)
    {
        printf("pairs[%i].winner: %i\n", i, pairs[i].winner);
        printf("pairs[%i].loser: %i\n", i, pairs[i].loser);
    }*/
    return;
}

void swap(pair *ppairs_i, pair *ppairs_ipl1)
{
    pair temp = *ppairs_i; // temporary store and dereference pointer of pairs_i
    *ppairs_i = *ppairs_ipl1;
    *ppairs_ipl1 = temp;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int return_dfs;
    for (int i = 0; i < pair_count; i++)
    {
        // we lock pairs, when we check if locked pairs make a cycle within graph using dfs and if it's true, we unlock pair
        locked[pairs[i].winner][pairs[i].loser] = true;

        // cycle checker, dfs should return 2 to indicate that cycle detected, therefore we unlock pair
        return_dfs = dfs();
        if (return_dfs == 2)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
        // printf("locked[%i][%i] == %i\n", pairs[i].winner, pairs[i].loser, locked[pairs[i].winner][pairs[i].loser]);
        // to do in future: add if statement for not enough memory
    }

    // debug
    /*for (int x = 0; x < candidate_count; x++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("locked[%i][%i] == %i\n", x, j, locked[x][j]);
        }
    }*/
    return;
}

// check cycle using DFS approach

// initialize and fill arrays with empty elements

void StackPush(int stack[], int *top, int element)
{
    // if stack is full, can't push
    if (*top == (candidate_count - 1))
    {
        return;
    }

    // not full
    else
    {
        (*top)++;
        stack[*top] = element;
        return;
    }
}

void StackPop(int stack[], int *top)
{
    // if stack is empty, nothing to pop
    if (*top == -1)
    {
        return;
    }

    // if not, delete the last element
    else
    {
        stack[*top] = -2; // delete element
        (*top)--;
        return;
    }
}

int StackFindEl(int stack[], int element)
{
    for (int x = 0; x < candidate_count; x++)
    {
        if (stack[x] == element) // linear search isn't the best solution, but 56 max candidates is cheap
        {
            return 1;
        }
    }
    return 0;
}

int dfs(void)
{
    /*
    after we make some locked[i][j] = true, we have a graph to traverse
    we start with the initial state (stackFrontier and exploredSet = {0})
    repeat:
        if frontier is empty, no solution
        remove a node from frontier
        add the node to the explored set
        if node contains goal state, return the solution
        expand node, add resulting nodes to the frontier if they aren't already in the frontier or explored set
    ex: 4 pairs, 4 * 2 = 8 vertices

function DFS(graph, start):
    while the stack is not empty:
        current_node = pop the top node from the stack
        if current_node is not in the visited set:
            add current_node to the visited set
            for each neighbor of current_node:
                if neighbor is not in the visited set:
                    push neighbor onto the stack

    */

    // stackFrontier contains edges to be explored
    // int stackFrontier[pair_count*2] = {0};
    int *stackFrontier = malloc(candidate_count * sizeof(int)); // white set
    if (stackFrontier == NULL)
    {
        printf("Can't allocate memory to the stackFrontier, sorry.");
        return 1;
    }
    // exploredSet contains explored edges (starting from locked[0][0])
    // int exploredSet[pair_count*2] = {0};
    int *exploredSet = malloc(candidate_count * sizeof(int)); // black set
    if (exploredSet == NULL)
    {
        printf("Can't allocate memory to the exploredSet, sorry.");
        free(stackFrontier);
        return 1;
    }

    int *pathStack = malloc(candidate_count * sizeof(int)); // gray set
    if (pathStack == NULL)
    {
        printf("Can't allocate memory to the pathStack, sorry.");
        free(stackFrontier);
        free(exploredSet);
        return 1;
    }

    // populate arrays with empty values
    for (int x = 0; x < candidate_count; x++)
    {
        exploredSet[x] = -2;
        stackFrontier[x] = -2;
        pathStack[x] = -2;
    }

    // to keep track of the top element
    int topOfFrontier = -1, topOfExplored = -1, topOfPath = -1; //-1 means it's empty

    int visited_count = 0, parent_node = 0, recent_neighbor = 0, last_traversed_node;

    for (int i = 0; i < pair_count; i++)
    {
        if (locked[pairs[i].winner][pairs[i].loser]) // if pairs exist we can try dfs
        {
            // starting with i
            parent_node = pairs[i].winner;
            if (StackFindEl(exploredSet, parent_node) == 0) // if current_node isn't in exploredSet
            {
                // can explore
                StackPush(stackFrontier, &topOfFrontier, parent_node); // push current_node to the top of the stack
                while (topOfFrontier != -1)                            // while stackFrontier isn't empty
                {
                    parent_node = stackFrontier[topOfFrontier];
                    StackPop(stackFrontier, &topOfFrontier); // pop current_node from the stackFrontier

                    if (StackFindEl(pathStack, parent_node) == 0) // if current_node isn't in pathStack
                    {
                        StackPush(pathStack, &topOfPath, parent_node); // push current_node to the pathStack
                        last_traversed_node = pathStack[topOfPath];

                        // expand with neighbours, traversing locked pairs inside 2d locked array
                        bool all_neighbors_explored = true;

                        for (int j = 0; j < candidate_count; j++)
                        {
                            if (locked[parent_node][j]) // neighbor exist if i->j
                            {
                                recent_neighbor = j;
                                if (StackFindEl(exploredSet, recent_neighbor) == 0 &&
                                    (StackFindEl(stackFrontier, recent_neighbor) ==
                                     0)) // if neighbor isn't in exploredSet and isn't in stackFrontier
                                {
                                    StackPush(stackFrontier, &topOfFrontier, recent_neighbor); // add neighbor to the frontier
                                    all_neighbors_explored = false;
                                }
                            }
                        }
                        if (all_neighbors_explored) // if all neighbors are checked
                        {
                            StackPop(pathStack, &topOfPath);
                            StackPush(exploredSet, &topOfExplored, last_traversed_node); // add last neighbor to the exploredSet
                        }
                    }
                    else if (StackFindEl(pathStack, last_traversed_node) == 1) // cycle detected if in gray set
                    {
                        printf("Dfs detected cycle on the step locked[%i][%i]. Making "
                               "locked[%i][%i] = 'false'\n", parent_node, recent_neighbor, parent_node, recent_neighbor);
                        free(stackFrontier);
                        free(exploredSet);
                        free(pathStack);
                        return 2;
                    }
                }
                if (topOfPath != -1)
                {
                    last_traversed_node = pathStack[topOfPath];
                    StackPop(pathStack, &topOfPath);
                    StackPush(exploredSet, &topOfExplored, last_traversed_node); // add last traversed node to the exploredSet
                }
            }
        }
    }
    // print explored set
    /*printf("Dfs finished. No cycle detected from starting point %i to point %i. ExploredSet: ", exploredSet[0], last_traversed_node);
    for (int i = candidate_count; i > 0; i--) // we start from the top of the stack
    {
        printf("%i\b", exploredSet[i]);
    }*/
    printf("\n");
    // don't forget to free allocated memory
    free(stackFrontier);
    free(exploredSet);
    free(pathStack);
    return 0;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i]) // if there's a locked pair such as loser->winner
            {
                is_winner = false;
                break;
            }
        }
        if (is_winner) // check if there's a candidate who doesn't lose to anyone
        {
            printf("Winner is %s\n", candidates[i]);
            return;
        }
    }
    return;
}
