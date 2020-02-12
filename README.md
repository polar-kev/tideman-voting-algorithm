# Tideman Voting Algorithm
A Tideman (Ranked Pairs) voting algorithm implemented in C as part of CS50X.

In a ranked-choice system, voters can vote for more than one candidate. Instead of just voting for their top choice, they can rank the candidates in order of preference.

The Tideman voting method consists of three parts:

- Tally: Once all of the voters have indicated all of their preferences, determine, for each pair of candidates, who the preferred candidate is and by what margin they are preferred.
- Sort: Sort the pairs of candidates in decreasing order of strength of victory, where strength of victory is defined to be the number of voters who prefer the preferred candidate.
- Lock: Starting with the strongest pair, go through the pairs of candidates in order and “lock in” each pair to the candidate graph, so long as locking in that pair does not create a cycle in the graph.

Purpose of this project:
- Revisit the programming language C
- Practice implementing an algorithm I haven't seen before
