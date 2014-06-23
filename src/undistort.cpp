
#include <stdlib.h>
#include <getopt.h>

#include <iostream>
#include <string>
using namespace std;


// Quick and dirty

bool verbose = false;
string xml_file, image_file, output_file;

static void parse_args( int argc, char **argv )
{
  int c;
  static struct option long_options[] = {
    {"verbose", 0,0,'v'},
    {"xml", 1, 0, 'x'},
    {"output", 1, 0, 'o'},
    {"help", 0, 0, 'h'},
    {NULL, 0, NULL, 0}
  };

  int option_index = 0;
  while( (c=getopt_long( argc, argv, "x:vh", 
          long_options, &option_index)) != -1 ) {
    int this_option_optind = optind ? optind : 1;
    switch( c ) {
      case 'x':
        xml_file = optarg;
        break;
      case 'v':
        verbose = true;
        break;
      case 'o':
        output_file = optarg;
        break;
      case 'h':
        cout << "undistort --xml xml_file [--output output_file] image_file" << endl;
        exit(0);
      default:
        cout << "Getopt returned illegal character" << endl;
    }
  }

  if( optind >= argc ) {
    cout << "Need to specify image file on command line" << endl;
    exit(0);
  }

  image_file = argv[optind];

  if( output_file.length() == 0 ) {
    // Automatically determine output file name
    size_t ext = image_file.find_last_of( '.' );
    if( ext == string::npos ) {
      cout << "Couldn't find extension of source file to determine output file name" << endl;
      exit(-1);
    }

    output_file.assign( image_file, 0, ext );
    output_file += "_undistort.jpg";

  }


}


int main( int argc, char **argv )
{
  parse_args( argc, argv );

  if( verbose ) {
    cout << "XML file: " << xml_file << endl;
    cout << "Image file: " << image_file << endl;
    cout << "Output to: " << output_file << endl;
  }



}
