

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

#include "Surface.h"


void star_path( Surface &surface ){

    int i;
    double theta = 4 * M_PI / 5.0;

    cairo_move_to( surface.cr, 0, 0 );
    for( i=0; i < 4; i++ ){
    	cairo_rel_line_to( surface.cr, 1.0, 0 );
    	cairo_rotate (surface.cr, theta );
    }
    cairo_close_path( surface.cr );

}



void draw_stars( Surface &surface ){

    cairo_set_source_rgb( surface.cr, 0, 0, 0 );

    cairo_save( surface.cr );
    {
    	cairo_translate( surface.cr, 5, surface.height / 2.6);
    	cairo_scale( surface.cr, surface.height, surface.height);
    	star_path( surface );
    	cairo_set_fill_rule( surface.cr, CAIRO_FILL_RULE_WINDING);
    	cairo_fill( surface.cr );
    }
    cairo_restore( surface.cr );

    cairo_save( surface.cr );
    {
    	cairo_translate( surface.cr, surface.width - surface.height - 5, surface.height / 2.6 );
    	cairo_scale( surface.cr, surface.height, surface.height );
    	star_path( surface );
    	cairo_set_fill_rule( surface.cr, CAIRO_FILL_RULE_EVEN_ODD );
    	cairo_fill( surface.cr );
    }
    cairo_restore( surface.cr );

}



void draw_boxes( Surface &surface ){

    cairo_set_source_rgba( surface.cr, 0, 0, 0, 1 );

    cairo_set_line_width( surface.cr, 2 );

    for( int x = 10; x < surface.width - 10; x += 20 ){

        for( int y = 10; y < surface.height - 10; y += 20 ){

            cairo_rectangle( surface.cr, x, y, 20, 20 );
            cairo_stroke( surface.cr );

        }

    }


    /*
    //cairo_rectangle is equivalient to:
    cairo_move_to (cr, 12, 12);
    cairo_rel_line_to (cr, surface.width, 0);
    cairo_rel_line_to (cr, 0, surface.height);
    cairo_rel_line_to (cr, -surface.width, 0);
    cairo_close_path (cr);
    */

        //cairo_set_source_rgb (cr, 0, 0.7, 0); 
        //cairo_fill_preserve (cr);

}



void draw_random_walk( Surface &surface ){

    cairo_set_source_rgba( surface.cr, 0, 0, 0, 1 );
    cairo_set_line_width( surface.cr, 2 );





    int move_number = 0;

    //center on map
    int current_x = surface.width / 2;
    int current_y = surface.height / 2;

    
    cairo_move_to( surface.cr, current_x, current_y );


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
        //cairo_set_source_rgba(surface.cr, colour_number, colour_number, colour_number, 1);
        //cout << current_x << "," << current_y << "," << colour_number << endl;

        cairo_rel_line_to( surface.cr, horizontal_direction * horizontal_weight, verical_direction * verical_weight );
        //cairo_close_path (cr);
        //cairo_stroke( cr );
        
        if( current_x > surface.width ) break;
        if( current_x < 0 ) break;
        if( current_y > surface.height ) break;
        if( current_y < 0 ) break;

    }

    //cairo_close_path (cr);
    cairo_stroke( surface.cr );

}





void tree_path( Surface& surface ){


    cairo_move_to( surface.cr, 0, 0 );

    cairo_rel_line_to( surface.cr, 12, 24 );
    cairo_rel_line_to( surface.cr, -10, 0 );
    cairo_rel_line_to( surface.cr, 0, 4 );

    cairo_rel_line_to( surface.cr, -4, 0 );
    cairo_rel_line_to( surface.cr, 0, -4 );
    cairo_rel_line_to( surface.cr, -9, 0 );

    cairo_close_path( surface.cr );

}



void draw_forest( Surface& surface ){

    for( int x = 0; x < 500; x++ ){

        int x_pos = rand() % surface.width;
        int y_pos = rand() % surface.height * .75 + surface.height / 4;

        int scale = rand() % 15 + 1;
        int colour = rand() % 200 + 55 ;

        cout << colour << endl;

        cairo_set_source_rgb( surface.cr, 0, (double)colour / 255, 0 );

        cairo_save( surface.cr );
        {            
            cairo_translate( surface.cr, x_pos, y_pos );
            cairo_scale( surface.cr, scale, scale );
            tree_path( surface );
            cairo_set_fill_rule( surface.cr, CAIRO_FILL_RULE_WINDING );
            cairo_fill( surface.cr );
        }
        cairo_restore( surface.cr );

    }

}


