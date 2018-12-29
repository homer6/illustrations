#pragma once

#include "Surface.h"


void draw_stars( Surface& surface );

void draw_boxes( Surface& surface );

void draw_random_walk( Surface& surface );

void tree_path( Surface& surface );

void draw_forest( Surface& surface );

void star_path( Surface& surface );






struct Cell{

    Cell( cairo_t *cr, int x = 0, int y = 0, int cell_size = 100 )
        :cr(cr), x(x), y(y), size(cell_size)
    {


    }

    cairo_t *cr;
    int x = 0;
    int y = 0;
    int size = 100;

    int boundary_cell = false;

    bool entrance_cell = false;
    bool exit_cell = false;

    bool premature_fork_halt = false;

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

    Grid( cairo_t *cr, int width = 10, int height = 10, int cell_size = 100, int fork_after = 10, int thread_length = 10 )
        :cr(cr), width(width), height(height), cell_size(cell_size), fork_after(fork_after), thread_length(thread_length)
    {

        for( int x = 0; x < this->width; x++ ){

            vector<Cell> column;

            for( int y = 0; y < this->height; y++ ){

                Cell current_cell{cr,x,y,cell_size};
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

                this->cell_stack.push_back( this->current_cell );
                this->walked_list.push_back( this->current_cell );

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

                this->cell_stack.push_back( this->current_cell );
                this->walked_list.push_back( this->current_cell );

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

                this->cell_stack.push_back( this->current_cell );
                this->walked_list.push_back( this->current_cell );

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

                this->cell_stack.push_back( this->current_cell );
                this->walked_list.push_back( this->current_cell );

                return true;

            }

        }

        return false;

    };






    bool linkUp(){

        if( this->hasCellUp() ){

            Cell *up_cell = this->getCell( this->current_cell->x, this->current_cell->y - 1 );

            this->current_cell->top = up_cell;
            up_cell->bottom = this->current_cell;

            return true;

        }

        return false;

    };



    bool linkRight(){

        if( this->hasCellRight() ){

            Cell *right_cell = this->getCell( this->current_cell->x + 1, this->current_cell->y );

            this->current_cell->right = right_cell;
            right_cell->left = this->current_cell;

            return true;

        }

        return false;

    };



    bool linkDown(){

        if( this->hasCellDown() ){

            Cell *down_cell = this->getCell( this->current_cell->x, this->current_cell->y + 1 );

            this->current_cell->bottom = down_cell;
            down_cell->top = this->current_cell;

            return true;

        }

        return false;


    };



    bool linkLeft(){

        if( this->hasCellLeft() ){

            Cell *left_cell = this->getCell( this->current_cell->x - 1, this->current_cell->y );

            this->current_cell->left = left_cell;
            left_cell->right = this->current_cell;

            return true;

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





    bool moveNext( int fork_length = -1 ){

        //cout << "moveNext: " << this->current_cell->x << ", " << this->current_cell->y << " fork_length: " << fork_length << endl;

        vector<int> possible_moves = this->getPossibleMoves();  //randomized

        if( possible_moves.size() == 0 ){

            if( fork_length > -1 ){
                this->unwind();
                this->moveNext( fork_length - 1 );
                return false;  //end recursion; no possible moves; not premature_fork_halt
            }

            //dead end (on primary thread); move back
            return this->unwind();
        }


        if( fork_length > -1 ){
            //this is a forked thread (recursion)

            if( fork_length == 0 ){
                this->current_cell->premature_fork_halt = true;
                return false;
            }else{

                int next_direction = possible_moves[1];

                if( next_direction == 0 ){
                    if( this->moveUp() ){
                        this->moveNext( fork_length - 1 );  //recursion
                        return false;
                    }
                }

                if( next_direction == 1 ){
                    if( this->moveRight() ){
                        this->moveNext( fork_length - 1 );  //recursion
                        return false;
                    }
                }

                if( next_direction == 2 ){
                    if( this->moveDown() ){
                        this->moveNext( fork_length - 1 );  //recursion
                        return false;
                    }
                }

                if( next_direction == 3 ){
                    if( this->moveLeft() ){
                        this->moveNext( fork_length - 1 );  //recursion
                        return false;
                    }
                }
                
                this->unwind();
                return false;
            }

        }


        //the remainder is for main thread only

        bool can_fork = possible_moves.size() > 1;

        if( this->cell_stack.size() % this->fork_after == 0 ){
            //every 10th cell, fork (by default)

            if( this->ready_to_fork == false ){
                this->ready_to_fork = true;
            }

        }


        int next_direction;

        if( this->ready_to_fork && can_fork ){
            //fork here
            //int forked_direction = possible_moves[0];
            this->ready_to_fork = false;

            Cell* saved_cell = this->current_cell;
            vector<Cell*> saved_stack = this->cell_stack;

            //recursion
            next_direction = possible_moves[1];

            if( next_direction == 0 ) this->moveUp();
            if( next_direction == 1 ) this->moveRight();
            if( next_direction == 2 ) this->moveDown();
            if( next_direction == 3 ) this->moveLeft();

            this->moveNext( this->thread_length );

            //restore cell before continuing with main thread
            this->current_cell = saved_cell;
            this->cell_stack = saved_stack;

            //fall through to main thread after returning from forking

        }


        //check again for possible moves because the forked thread may have used it
        possible_moves = this->getPossibleMoves();

        if( possible_moves.size() == 0 ){
            //dead end
            return this->unwind();
        }

        next_direction = possible_moves[0];
        

        if( next_direction == 0 ){
            if( this->moveUp() ){
                return true;
            }
        }

        if( next_direction == 1 ){
            if( this->moveRight() ){
                return true;
            }
        }

        if( next_direction == 2 ){
            if( this->moveDown() ){
                return true;
            }
        }

        if( next_direction == 3 ){
            if( this->moveLeft() ){
                return true;
            }
        }

        return false;

    };


    bool unwind(){
        //walks back until not stuck; return false if breadcrumb trail is empty

        while( this->cell_stack.size() > 1 ){
            
            this->cell_stack.pop_back();

            Cell* previous_cell = this->cell_stack.back();

            this->current_cell = previous_cell;

            if( this->canMove() ){
                return true;
            }

        }

        return false;

    };



    bool canMove(){

        if( this->canMoveUp() ) return true;
        if( this->canMoveRight() ) return true;
        if( this->canMoveDown() ) return true;
        if( this->canMoveLeft() ) return true;

        return false;

    };


    bool canFork(){

        return this->getPossibleMoves().size() > 1;

    };


    vector<int> getPossibleMoves(){

        vector<int> possible_moves;

        if( this->canMoveUp() ) possible_moves.push_back(0);
        if( this->canMoveRight() ) possible_moves.push_back(1);
        if( this->canMoveDown() ) possible_moves.push_back(2);
        if( this->canMoveLeft() ) possible_moves.push_back(3);

        std::random_shuffle( possible_moves.begin(), possible_moves.end() );

        return possible_moves;

    };



    vector<int> getPossibleLinkableMoves(){

        vector<int> possible_moves;

        if( this->canLinkUp() ) possible_moves.push_back(0);
        if( this->canLinkRight() ) possible_moves.push_back(1);
        if( this->canLinkDown() ) possible_moves.push_back(2);
        if( this->canLinkLeft() ) possible_moves.push_back(3);

        std::random_shuffle( possible_moves.begin(), possible_moves.end() );

        return possible_moves;

    };




    bool canMoveUp(){

        if( this->hasCellUp() ){
            Cell *up_cell = this->getCell( this->current_cell->x, this->current_cell->y - 1 );
            if( !up_cell->isOccupied() ){
                return true;
            }
        }
        return false;
        
    };


    bool canMoveRight(){

        if( this->hasCellRight() ){
            Cell *right_cell = this->getCell( this->current_cell->x + 1, this->current_cell->y );
            if( !right_cell->isOccupied() ){
                return true;
            }
        }
        return false;
        
    };


    bool canMoveDown(){

        if( this->hasCellDown() ){
            Cell *down_cell = this->getCell( this->current_cell->x, this->current_cell->y + 1 );
            if( !down_cell->isOccupied() ){
                return true;
            }
        }
        return false;
        
    };


    bool canMoveLeft(){

        if( this->hasCellLeft() ){
            Cell *left_cell = this->getCell( this->current_cell->x - 1, this->current_cell->y );
            if( !left_cell->isOccupied() ){
                return true;
            }
        }
        return false;

    };




    //linking is when you break through a wall to join an isolated cell
    //the link target must be present, occupied, and cannot exist in walked_list

    bool canLinkUp(){

        if( this->hasCellUp() ){
            Cell *up_cell = this->getCell( this->current_cell->x, this->current_cell->y - 1 );

            auto p = std::find( this->walked_list.begin(), this->walked_list.end(), up_cell );

            if( up_cell->isOccupied() && p == this->walked_list.end() ){
                return true;
            }
        }
        return false;
        
    };


    bool canLinkRight(){

        if( this->hasCellRight() ){
            Cell *right_cell = this->getCell( this->current_cell->x + 1, this->current_cell->y );

            auto p = std::find( this->walked_list.begin(), this->walked_list.end(), right_cell );

            if( right_cell->isOccupied() && p == this->walked_list.end() ){
                return true;
            }
        }
        return false;
        
    };


    bool canLinkDown(){

        if( this->hasCellDown() ){
            Cell *down_cell = this->getCell( this->current_cell->x, this->current_cell->y + 1 );

            auto p = std::find( this->walked_list.begin(), this->walked_list.end(), down_cell );

            if( down_cell->isOccupied() && p == this->walked_list.end() ){
                return true;
            }
        }
        return false;
        
    };


    bool canLinkLeft(){

        if( this->hasCellLeft() ){
            Cell *left_cell = this->getCell( this->current_cell->x - 1, this->current_cell->y );

            auto p = std::find( this->walked_list.begin(), this->walked_list.end(), left_cell );

            if( left_cell->isOccupied() && p == this->walked_list.end() ){
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

        this->cell_stack.push_back( cell );
        this->walked_list.push_back( cell );

    };


    void generateMaze(){

        this->enterMaze( 5, 0 );
        while( this->moveNext() ){
            //done main thread
        }


        //continue any of the premature_forks
            int premature_forks_count = 0;

            do{

                premature_forks_count = 0;

                for( int x = 0; x < this->width; x++ ){

                    for( int y = 0; y < this->height; y++ ){

                        Cell* this_cell = this->getCell( x, y );

                        if( this_cell->premature_fork_halt ){

                            premature_forks_count++;

                            this_cell->premature_fork_halt = false;

                            //clear the stack
                            this->cell_stack.clear();
                            this->walked_list.clear();
                            
                            this->cell_stack.push_back( this_cell );
                            this->walked_list.push_back( this_cell );
                            this->current_cell = this_cell;

                            while( this->moveNext() ){
                                //treat this premature_fork_halt as a main thread
                            }

                        }

                    }
                    
                }

            }while( premature_forks_count > 0 );


        //continue any of the premature_forks
            int unoccupied_count = 0;

            do{

                unoccupied_count = 0;

                for( int x = 0; x < this->width; x++ ){

                    for( int y = 0; y < this->height; y++ ){

                        Cell* this_cell = this->getCell( x, y );

                        if( !this_cell->occupied ){

                            unoccupied_count++;

                            this_cell->occupied = true;

                            //clear the stack
                            this->cell_stack.clear();
                            this->walked_list.clear();
                            
                            this->cell_stack.push_back( this_cell );
                            this->walked_list.push_back( this_cell );

                            this->current_cell = this_cell;

                            //join the rest of the unoccupied adjacent cells
                            while( this->moveNext() ){
                                //treat this unoccupied as a main thread
                            }



                            //link up to the closest occupied cell (check all cells in the walked list)

                            bool has_linked = false;

                            for( Cell* active_cell : this->walked_list ){

                                if( has_linked ){
                                    break;
                                }

                                this->current_cell = active_cell;

                                vector<int> possible_moves = this->getPossibleLinkableMoves();

                                if( possible_moves.size() > 0 ){
                                    has_linked = true;

                                    int link_direction = possible_moves[0];

                                    if( link_direction == 0 ) this->linkUp();
                                    if( link_direction == 1 ) this->linkRight();
                                    if( link_direction == 2 ) this->linkDown();
                                    if( link_direction == 3 ) this->linkLeft();

                                }

                            }

                            if( !has_linked ){
                                throw std::runtime_error( "No possible linkable cells." );
                            }


                        }

                    }
                    
                }

            }while( unoccupied_count > 0 );



    };

    Cell* current_cell = nullptr;


    vector<Cell*> cell_stack;

    vector<Cell*> walked_list;

    bool ready_to_fork = false;

    int cell_size = 100;
    int fork_after = 10;
    int thread_length = 10;

};

