
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <opencv2/core.hpp>
using namespace cv;

#include "rapidxml-1.13/rapidxml.hpp"
using namespace rapidxml;

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

static char *read_xml( const string &xmlfile )
{
  // Load XML
  ifstream xml_fstream( xmlfile.c_str() );
  if( !xml_fstream.good() ) {
    cout << "Couldn't open XML for reading." << endl;
    exit(-1);
  }
  xml_fstream.seekg( 0, ios_base::end );
  int xmlsize = xml_fstream.tellg();
  char *text = new char[ xmlsize+1 ];
  xml_fstream.seekg( 0, ios_base::beg);
  xml_fstream.read( text, xmlsize );
  xml_fstream.close();

  return text;
}

struct CalibrationResult {
  CalibrationResult( double _fx, double _fy, double _px, double _py, double _d1, double _d2, double _d3, double _d4 )
    : fx(_fx), fy(_fy), px(_px), py(_py), d1(_d1), d2(_d2), d3(_d3), d4(_d4) {;}

  double fx, fy, px, py, d1, d2, d3, d4;
};

static CalibrationResult parse_xml( const string &xmlfile )
{
  char *text = read_xml( xml_file );


  xml_document<> doc;
  doc.parse<0>( text );

  xml_node<> *calproj = doc.first_node( "CalibrationProject" );
  xml_node<> *results = calproj->first_node( "results" );
  if( results == NULL ) {
    cout << "Couldn't find results in XML file" << endl;
    exit(-1);
  }

  double fx = atof(results->first_node("focus_lenX")->value()),
         fy = atof(results->first_node("focus_lenY")->value()),
         px = atof(results->first_node("PrincipalX")->value()),
         py = atof(results->first_node("PrincipalY")->value()),
         d1 = atof(results->first_node("Dist1")->value()),
         d2 = atof(results->first_node("Dist2")->value()),
         d3 = atof(results->first_node("Dist3")->value()),
         d4 = atof(results->first_node("Dist4")->value());




  delete text;

  return CalibrationResult( fx, fy, px ,py ,d1, d2, d3, d4 );
}



int main( int argc, char **argv )
{
  parse_args( argc, argv );

  if( verbose ) {
    cout << "XML file: " << xml_file << endl;
    cout << "Image file: " << image_file << endl;
    cout << "Output to: " << output_file << endl;
  }

  CalibrationResult cal(  parse_xml( xml_file ) );
  if( verbose ) {
    cout << "Focal length (x,y) " << cal.fx << "," << cal.fy << endl;
    cout << "Principal point (x,y) " << cal.px << "," << cal.py << endl;
    cout << "Distortion params " << cal.d1 << "," << cal.d2 << "," << cal.d3 << "," << cal.d4 << endl;
  }
}
