# Eight-Tile Puzzle Solver
A demonstration of the following search space techniques applied to an eight-tile puzzle:
1. Breadth-First-Search
2. Greedy Breadth-First-Serach (Manahattan Distance Heuristic)
3. A* (Misplaced Tile heuristic)
4. A* (Manhattan Distance heuristic)

Takes an 8-tile puzzle and determines if it is solvable. If so, finds a path to the goal state using the selected search algorithm. User may use default puzzle or enter their own via graphical and/or textual input.

## Usage
Compile with `g++ eight_tile.cpp -o eight_tile.exe`, run with `./eight_tile.exe`, then follow prompts.
