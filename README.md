
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
2. Change the WIDTH and HEIGHT at the top, along with the draw_maze() function on line 975.

For example, 

```
    //easy (1000,1000)
    cairo_translate( cr, 50, 50 );
    Grid grid( cr, 20, 20, 45, 7, 5 );
```

This means it'll build a 1000px by 1000px PNG file.
`cairo_translate` will move the maze by 50px from the left of the page and 50px from the top of the page.
`Grid grid( cr, 20, 20, 45, 5, 5 );` will create a maze 20 cells wide by 20 cells high. Each cell size will be 45px wide and tall. It'll fork after 7 moves when creating the maze, and each fork length will be 5 moves deep.

