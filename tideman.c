#include <cs50.h>
#include <stdio.h>
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
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool is_cycle(int, int, int);

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

    for (int x = 0; x < candidate_count; x++)
    {
        for (int z = 0; z < candidate_count; z++)
        {
            printf("%i ", preferences[x][z]);
        }
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
        //vote is for a valid candidate
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            //printf("candidate: %s, rank[%i]: %i\n",candidates[i],rank,i);
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
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
            if (preferences[i][j] > 0 && preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    pair temp;
    int i = 0;

    for (i = 0; i < pair_count - 1; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[i].winner][pairs[i].loser])
            {
                //swap the two values
                temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //Step through the sorted_pairs array
    for (int i = 0; i < pair_count; i++)
    {
        //If no cycle will be created, lock the pairs in
        if (!is_cycle(pairs[i].winner, pairs[i].loser, i))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    /*
    //Testing
    printf("Locked Table: \n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i, ", locked[i][j]);
        }
        printf("\n");
    }
    */

}

//recursively checks if there will be a cycle between a node and the pair at index pair_n
bool is_cycle(int winner, int loser, int pair_n)
{
    //base case, current node is locked and connected to the starting node
    if (locked[winner][loser] && loser == pairs[pair_n].winner)
    {
        return true;
    }

    //recursive case
    //try to build a path from another node in the locked matrix to the starting node at pair_n
    for (int i = 0; i < candidate_count; i++)
    {
        //if the current node is locked, check if there is a path that continues to pair_n
        if (locked[loser][i])
        {
            return is_cycle(loser, i, pair_n);
        }
    }

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    bool filled;

    for (int i = 0; i < candidate_count; i++)
    {
        filled = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                filled = true;
            }

        }
        if (!filled)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }

    return;
}
