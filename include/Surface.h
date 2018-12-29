#pragma once


#include <cstdlib>

#include <string>
using std::string;


struct Surface{


	Surface( int width = 1000, int height = 1000 )
		:width(width), height(height)
	{

		int stride = width * 4;

		this->image = (unsigned char*) malloc( stride * height );

		if( this->image == nullptr ){
			throw std::runtime_error( "Failed to allocate memory for image." );
		}


		this->surface = cairo_image_surface_create_for_data( image, CAIRO_FORMAT_ARGB32, width, height, stride );
    	this->cr = cairo_create( this->surface );

  	    cairo_rectangle( this->cr, 0, 0, width, height );
	    cairo_set_source_rgb( cr, 1, 1, 1 );
    	cairo_fill( this->cr );

	};



	void writeToFile( const string filename = "maze.png" ){

		cairo_surface_write_to_png( this->surface, filename.c_str() );

	};



	~Surface(){
	    
	    cairo_destroy( this->cr );
	    cairo_surface_destroy( this->surface );

	    free( this->image );

	}


    cairo_surface_t *surface;
    cairo_t *cr;

    int width;
    int height;

	unsigned char *image = nullptr;

};