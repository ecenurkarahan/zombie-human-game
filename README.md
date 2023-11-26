# zombie-human-game
Grid system
1. There are several maps under the maps folder. For each map, first line gives the width (column
number) and second line gives the height of the grid (row number).
2. You need to dynamically allocate memory for the grid.
3. Points are zero-indexed from the top left corner. Thus, top left corner is (0, 0), bottom right
corner which is also exit corner is (Height-1, Width-1).
4. Dashes (-) stands for empty cells. H stands for humans, and Z for zombies. A cell can only
be one of these three elements. During the simulation, you should make necessary adjustments to
maintain this condition (i.e. checking for collisions).
Movements
Humans: Humans move first to the right then to the down. You can think it as follows, in the
odd steps (1,3,5..) they will try to go right, and in the even steps they will try to go down. They
can only get to their destination if it is empty.
Zombies: Similar to humans, but they have four movements with the following order:
Right, Down, Left, Up. Again, they can only move if the destination is empty.
Rules
1. You should first move the humans, then zombies. These movements should be in a dynamically
allocated temporary grid. After the movements, check and apply the necessary rules from below.
2. Interactions. It is important to apply these rules in the following order so that we
get the desired simulation.
2.1. If a zombie is surrounded by at least two people, it dies. Mark that cell as empty.
2.2. If a human is surrounded by at least one zombie, s/he turns into a zombie.
3. After applying these rules, you need to move temporary grid to original grid at each step.
