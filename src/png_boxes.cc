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

using std::cout;
using std::endl;


void draw_stars (cairo_t *cr, int width, int height);

void draw_boxes( cairo_t *cr, int width, int height );

void draw_maze( cairo_t *cr, int width, int height );

void tree_path( cairo_t *cr );
void draw_forest( cairo_t *cr, int width, int height );



void star_path (cairo_t *cr);

#define WIDTH 4000
#define HEIGHT 3000
#define STRIDE (WIDTH * 4)

unsigned char image[STRIDE*HEIGHT];

int main( void ){

    /* initialize random seed: */
    srand( time(NULL) );

    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create_for_data (image, CAIRO_FORMAT_ARGB32,
						   WIDTH, HEIGHT, STRIDE);

    cr = cairo_create (surface);

    cairo_rectangle (cr, 0, 0, WIDTH, HEIGHT);
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_fill (cr);

    //draw_stars (cr, WIDTH, HEIGHT);
    //draw_boxes( cr, WIDTH, HEIGHT );
    //draw_maze( cr, WIDTH, HEIGHT );
    draw_forest( cr, WIDTH, HEIGHT );

    cairo_surface_write_to_png (surface, "forest.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    return 0;

}



void star_path( cairo_t *cr ){

    int i;
    double theta = 4 * M_PI / 5.0;

    cairo_move_to (cr, 0, 0);
    for (i=0; i < 4; i++) {
    	cairo_rel_line_to (cr, 1.0, 0);
    	cairo_rotate (cr, theta);
    }
    cairo_close_path (cr);

}

void draw_stars (cairo_t *cr, int width, int height){

    cairo_set_source_rgb (cr, 0, 0, 0);

    cairo_save (cr);
    {
    	cairo_translate (cr, 5, height / 2.6);
    	cairo_scale (cr, height, height);
    	star_path (cr);
    	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
    	cairo_fill (cr);
    }
    cairo_restore (cr);

    cairo_save (cr);
    {
    	cairo_translate (cr, width - height - 5, height / 2.6);
    	cairo_scale (cr, height, height);
    	star_path (cr);
    	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
    	cairo_fill (cr);
    }
    cairo_restore (cr);

}



void draw_boxes( cairo_t *cr, int width, int height ){

    cairo_set_source_rgba (cr, 0, 0, 0, 1);

    cairo_set_line_width (cr, 2);

    for( int x = 10; x < width - 10; x += 30 ){

        for( int y = 10; y < height - 10; y += 30 ){

            cairo_rectangle (cr, x, y, 20, 20);
            cairo_stroke (cr);

        }

    }


    /*
    //cairo_rectangle is equivalient to:
    cairo_move_to (cr, 12, 12);
    cairo_rel_line_to (cr, width, 0);
    cairo_rel_line_to (cr, 0, height);
    cairo_rel_line_to (cr, -width, 0);
    cairo_close_path (cr);
    */

        //cairo_set_source_rgb (cr, 0, 0.7, 0); 
        //cairo_fill_preserve (cr);

}



void draw_maze( cairo_t *cr, int width, int height ){

    cairo_set_source_rgba (cr, 0, 0, 0, 1);
    cairo_set_line_width (cr, 2);





    int move_number = 0;

    //center on map
    int current_x = width / 2;
    int current_y = height / 2;

    
    cairo_move_to( cr, current_x, current_y );


    int total_moves = 100000;

    while( move_number < total_moves ){

        int horizontal_direction = rand() % 3 - 1 ;
        int verical_direction = rand() % 3 - 1 ;

        int horizontal_weight = rand() % 25 + 1;
        int verical_weight = rand() % 25 + 1 ;

        move_number++;

        current_x += horizontal_direction * horizontal_weight;
        current_y += verical_direction * verical_weight;

        //int colour_number = ((double)move_number / (double)total_moves) * 255;
        //cairo_set_source_rgba(cr, colour_number, colour_number, colour_number, 1);
        //cout << current_x << "," << current_y << "," << colour_number << endl;

        cairo_rel_line_to( cr, horizontal_direction * horizontal_weight, verical_direction * verical_weight );
        //cairo_close_path (cr);
        //cairo_stroke( cr );
        
        if( current_x > width ) break;
        if( current_x < 0 ) break;
        if( current_y > height ) break;
        if( current_y < 0 ) break;

    }

    //cairo_close_path (cr);
    cairo_stroke( cr );

}





void tree_path( cairo_t *cr ){


    cairo_move_to( cr, 0, 0 );

    cairo_rel_line_to( cr, 12, 24 );
    cairo_rel_line_to( cr, -10, 0 );
    cairo_rel_line_to( cr, 0, 4 );

    cairo_rel_line_to( cr, -4, 0 );
    cairo_rel_line_to( cr, 0, -4 );
    cairo_rel_line_to( cr, -9, 0 );

    cairo_close_path( cr );

}



void draw_forest(cairo_t *cr, int width, int height){

    for( int x = 0; x < 500; x++ ){

        int x_pos = rand() % width;
        int y_pos = rand() % height * .75 + height / 4;

        int scale = rand() % 15 + 1;
        int colour = rand() % 200 + 55 ;

        cout << colour << endl;

        cairo_set_source_rgb(cr, 0, (double)colour / 255, 0);

        cairo_save (cr);
        {            
            cairo_translate (cr, x_pos, y_pos);
            cairo_scale (cr, scale, scale);
            tree_path (cr);
            cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
            cairo_fill (cr);
        }
        cairo_restore (cr);

    }

}