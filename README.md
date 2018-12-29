
# Maze generator

Tested on Ubuntu 16.04.5 LTS

```
sudo apt-get install libcairo2-dev libpng12-dev
cmake .
make && ./build/illustrations
```

The new maze will be in maze.png in the current directory.

### To change the maze parameters:

1. Open src/png_boxes.cc
2. Change the parameters on line 90
3. make && ./build/illustrations

For example, 

```
    //very easy
        cells_wide = 6; cells_tall = 6; cell_size = 125; fork_after = 3; thread_length = 4;
```

It will create a maze 6 cells wide by 6 cells high. Each cell size will be 125px wide and tall. It'll fork after 7 moves when creating the maze, and each fork length (called a thread) will be 4 moves deep.

To help you make sure you haven't cut off the maze when you print it, the border cells are red.