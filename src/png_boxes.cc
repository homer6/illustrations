/*
 * Copyright © 2003 USC, Information Sciences Institute
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of the
 * University of Southern California not be used in advertising or
 * publicity pertaining to distribution of the software without
 * specific, written prior permission. The University of Southern
 * California makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * THE UNIVERSITY OF SOUTHERN CALIFORNIA DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL THE UNIVERSITY OF
 * SOUTHERN CALIFORNIA BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Carl D. Worth <cworth@isi.edu>
 */


//https://gitlab.com/cairo/cairo-demos/blob/master/png/stars.c

#include <cairo/cairo.h>
#include <math.h>

#include <cstdlib>
#include <time.h>

#include <iostream>
#include <vector>

#include <stdexcept>

#include <algorithm>

using std::vector;


using std::cout;
using std::endl;


#include "actions.h"

#include "Surface.h"

int main(){


    /* initialize random seed: */
    srand( time(NULL) );

    int cells_wide = 24;
    int cells_tall = 18;
    int cell_size = 35;

    int fork_after = 5;
    int thread_length = 5;

    int border_width = 100;

    int png_width = 1000;
    int png_height = 1000;

    const string filename = "maze.png";





    //monster-sized
        //cells_wide = 225; cells_tall = 150; cell_size = 32; fork_after = 7; thread_length = 20;
    //nightmare
        //cells_wide = 300; cells_tall = 300; cell_size = 32; fork_after = 25; thread_length = 20;
    //very hard
        //cells_wide = 60; cells_tall = 60; cell_size = 32; fork_after = 15; thread_length = 10;
    //medium
        //cells_wide = 60; cells_tall = 45; cell_size = 32; fork_after = 10; thread_length = 15;

    //easy
        //lucas/emily (age 4)
        cells_wide = 14; cells_tall = 10; cell_size = 50; fork_after = 5; thread_length = 5; border_width = 30;

        //ben/oliver (age 6)
        cells_wide = 28; cells_tall = 21; cell_size = 50; fork_after = 10; thread_length = 15; border_width = 25;

    //very easy
        //cells_wide = 6; cells_tall = 6; cell_size = 125; fork_after = 3; thread_length = 3;




    png_width = 2 * border_width + cells_wide * cell_size;
    png_height = 2 * border_width + cells_tall * cell_size;

    Surface surface( png_width, png_height );

    cairo_translate( surface.cr, border_width, border_width );
    Grid grid( surface.cr, cells_wide, cells_tall, cell_size, fork_after, thread_length );

    grid.generateMaze();
    grid.print();

    surface.writeToFile( filename );

    cout << "Created maze in " << filename << "." << endl;
    
    return 0;

}



    //draw_stars( surface );
    //draw_boxes( surface );
    //draw_random_walk( surface );
    //draw_forest( surface );