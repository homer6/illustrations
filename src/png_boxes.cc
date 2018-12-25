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

using std::vector;


#include <stack>
using std::stack;


using std::cout;
using std::endl;


void draw_stars (cairo_t *cr, int width, int height);

void draw_boxes( cairo_t *cr, int width, int height );

void draw_random_walk( cairo_t *cr, int width, int height );

void tree_path( cairo_t *cr );
void draw_forest( cairo_t *cr, int width, int height );


void draw_maze( cairo_t *cr, int width, int height );


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
    //draw_random_walk( cr, WIDTH, HEIGHT );
    //draw_forest( cr, WIDTH, HEIGHT );

    draw_maze( cr, WIDTH, HEIGHT );

    cairo_surface_write_to_png (surface, "maze.png");
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

    for( int x = 10; x < width - 10; x += 20 ){

        for( int y = 10; y < height - 10; y += 20 ){

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



void draw_random_walk( cairo_t *cr, int width, int height ){

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




struct Cell{

    Cell( cairo_t *cr, int x = 0, int y = 0 )
        :cr(cr), x(x), y(y)
    {


    }

    cairo_t *cr;
    int x = 0;
    int y = 0;
    int size = 50;

    int boundary_cell = false;

    bool entrance_cell = false;
    bool exit_cell = false;

    bool isBoundary(){
        return this->boundary_cell;
    }


    int occupied = false;

    bool isOccupied(){
        return this->occupied;
    }

    void print(){

        //cairo_move_to( this->cr, x * this->size, y * this->size );

        if( this->boundary_cell ){
            cairo_set_source_rgb(cr, 1, 0, 0);
        }else{
            cairo_set_source_rgb(cr, 0, 0, 0);
        }


        if( this->top == nullptr && !this->entrance_cell ){
            cairo_move_to( this->cr, x * this->size, y * this->size );
            cairo_rel_line_to( this->cr, this->size, 0 );
            cairo_stroke( this->cr );
        }

        if( this->right == nullptr ){
            cairo_move_to( this->cr, (x + 1) * this->size, y * this->size );
            cairo_rel_line_to( this->cr, 0, this->size );
            cairo_stroke( this->cr );
        }

        if( this->bottom == nullptr && !this->exit_cell ){
            cairo_move_to( this->cr, x * this->size, (y + 1) * this->size );
            cairo_rel_line_to( this->cr, this->size, 0 );
            cairo_stroke( this->cr );
        }

        if( this->left == nullptr ){
            cairo_move_to( this->cr, x * this->size, y * this->size );
            cairo_rel_line_to( this->cr, 0, this->size );
            cairo_stroke( this->cr );
        }

        //cairo_rectangle( this->cr, this->x * this->size, this->y * this->size, this->size, this->size );
        //cairo_stroke( this->cr );

    };


    bool hasTop(){ 
        return this->top != nullptr; 
    };

    bool hasRight(){ 
        return this->right != nullptr; 
    };

    bool hasBottom(){ 
        return this->bottom != nullptr; 
    };
    
    bool hasLeft(){ 
        return this->left != nullptr; 
    };

    Cell* top = nullptr;
    Cell* right = nullptr;
    Cell* bottom = nullptr;
    Cell* left = nullptr;

};





struct Grid{

    Grid( cairo_t *cr, int width = 10, int height = 10 )
        :cr(cr), width(width), height(height)
    {

        for( int x = 0; x < this->width; x++ ){

            vector<Cell> column;

            for( int y = 0; y < this->height; y++ ){

                Cell current_cell{cr,x,y};
                if( x == 0 ) current_cell.boundary_cell = true;
                if( y == 0 ) current_cell.boundary_cell = true;
                if( x == this->width - 1 ) current_cell.boundary_cell = true;
                if( y == this->height - 1 ) current_cell.boundary_cell = true;

                if( x == 5 && y == 0 ) current_cell.entrance_cell = true;
                if( x == (this->width - 6) && y == (this->height - 1) ) current_cell.exit_cell = true;

                column.push_back( std::move(current_cell) );

            }

            this->grid.push_back( std::move(column) );
            
        }

    }


    cairo_t *cr;

    int width = 10;
    int height = 10;

    vector<vector<Cell>> grid;

    
    void print(){

        cairo_set_source_rgba (cr, 0, 0, 0, 1);

        cairo_set_line_width (cr, 2);


        for( auto& column : this->grid ){

            for( auto& cell : column ){

                cell.print();

            }

        }

    };



    Cell* getCell( int x, int y ){

        if( !this->hasCell(x,y) ){
            throw std::runtime_error("getCell: Cell not found.");
        }

        return & this->grid[x][y];

    };


    bool hasCell( int x, int y ){
        if( x >= this->width ) return false;
        if( x < 0 ) return false;
        if( y >= this->height ) return false;
        if( y < 0 ) return false;
        return true;
    }



    bool moveUp(){

        if( this->hasCellUp() ){

            Cell *up_cell = this->getCell( this->current_cell->x, this->current_cell->y - 1 );

            if( !up_cell->isOccupied() ){

                this->current_cell->top = up_cell;
                up_cell->bottom = this->current_cell;

                up_cell->occupied = true;

                this->current_cell = up_cell;

                this->cell_stack.push( this->current_cell );

                return true;

            }

        }

        return false;

    };



    bool moveRight(){

        if( this->hasCellRight() ){

            Cell *right_cell = this->getCell( this->current_cell->x + 1, this->current_cell->y );

            if( !right_cell->isOccupied() ){

                this->current_cell->right = right_cell;
                right_cell->left = this->current_cell;

                right_cell->occupied = true;

                this->current_cell = right_cell;

                this->cell_stack.push( this->current_cell );

                return true;

            }

        }

        return false;

    };



    bool moveDown(){

        if( this->hasCellDown() ){

            Cell *down_cell = this->getCell( this->current_cell->x, this->current_cell->y + 1 );

            if( !down_cell->isOccupied() ){

                this->current_cell->bottom = down_cell;
                down_cell->top = this->current_cell;

                down_cell->occupied = true;

                this->current_cell = down_cell;

                this->cell_stack.push( this->current_cell );

                return true;

            }

        }

        return false;


    };



    bool moveLeft(){

        if( this->hasCellLeft() ){

            Cell *left_cell = this->getCell( this->current_cell->x - 1, this->current_cell->y );

            if( !left_cell->isOccupied() ){

                this->current_cell->left = left_cell;
                left_cell->right = this->current_cell;

                left_cell->occupied = true;

                this->current_cell = left_cell;

                this->cell_stack.push( this->current_cell );

                return true;

            }

        }

        return false;

    };






    bool hasCellUp(){
        return this->hasCell( this->current_cell->x, this->current_cell->y - 1 );
    };

    bool hasCellRight(){
        return this->hasCell( this->current_cell->x + 1, this->current_cell->y );
    };

    bool hasCellDown(){
        return this->hasCell( this->current_cell->x, this->current_cell->y + 1 );
    };

    bool hasCellLeft(){
        return this->hasCell( this->current_cell->x - 1, this->current_cell->y );
    };





    bool moveNext(){

        int tries = 0;

        bool failed_up = false;
        bool failed_right = false;
        bool failed_down = false;
        bool failed_left = false;

        //Cell *previous_cell = this->current_cell;

        while( tries < 100 ){

            int next_direction = rand() % 4;

            if( next_direction == 0 ){
                if( this->moveUp() ){
                    return true;
                }
                failed_up = true;
            }

            if( next_direction == 1 ){
                if( this->moveRight() ){
                    return true;
                }
                failed_right = true;
            }

            if( next_direction == 2 ){
                if( this->moveDown() ){
                    return true;
                }
                failed_down = true;
            }

            if( next_direction == 3 ){
                if( this->moveLeft() ){
                    return true;
                }
                failed_left = true;
            }

            tries++;

            if( failed_up && failed_right && failed_down && failed_left ){
                //dead end; move back                
                return this->unwind();
            }

        }

        return false;

    };


    bool unwind(){
        //walks back until not stuck; return false if breadcrumb trail is empty

        while( this->cell_stack.size() > 1 ){
            
            this->cell_stack.pop();

            Cell* previous_cell = this->cell_stack.top();

            this->current_cell = previous_cell;

            if( this->canMove() ){
                return true;
            }

        }

        return false;

    };



    bool canMove(){

        if( this->hasCellUp() ){
            Cell *up_cell = this->getCell( this->current_cell->x, this->current_cell->y - 1 );
            if( !up_cell->isOccupied() ){
                return true;
            }
        }

        if( this->hasCellRight() ){
            Cell *right_cell = this->getCell( this->current_cell->x + 1, this->current_cell->y );
            if( !right_cell->isOccupied() ){
                return true;
            }
        }

        if( this->hasCellDown() ){
            Cell *down_cell = this->getCell( this->current_cell->x, this->current_cell->y + 1 );
            if( !down_cell->isOccupied() ){
                return true;
            }
        }

        if( this->hasCellLeft() ){
            Cell *left_cell = this->getCell( this->current_cell->x - 1, this->current_cell->y );
            if( !left_cell->isOccupied() ){
                return true;
            }
        }

        return false;

    };



    void enterMaze( int x, int y ){

        if( !this->hasCell(x,y) ){
            throw std::runtime_error("enterMaze: Cell not found.");
        }

        Cell *cell = this->getCell( x, y );

        if( !cell->isBoundary() ){
            throw std::runtime_error("enterMaze: must be a boundary cell.");
        }

        cell->occupied = true;
        this->current_cell = cell;

        this->cell_stack.push( cell );

    };


    void generateMaze(){

        this->enterMaze( 0, 5 );
        while( this->moveNext() ){
            //done
        }

    };

    Cell* current_cell = nullptr;


    stack<Cell*> cell_stack;



};



void draw_maze(cairo_t *cr, int width, int height){


    cairo_translate( cr, 200, 200 );

    Grid grid( cr, 50, 50 );
    grid.generateMaze();
    grid.print();


    //int horizontal_direction = rand() % 3 - 1 ;
    //int verical_direction = rand() % 3 - 1 ;






}