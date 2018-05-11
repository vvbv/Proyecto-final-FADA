#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

// Block of declarations
tuple < vector < tuple < int, int > >, int, int > read_input( string file_name );
vector < string > string_tokenizer( string string_to_tok, char separator );
vector < tuple < int, int > > voracious( tuple < vector < tuple < int, int > >, int, int > input );
vector < int > sort_by_radius( vector < int > input, int p, int q  );
// End block of declarations

// Block of global variables
// End block of global variables

int main( int argc, const char* argv[] ){
    tuple < vector < tuple < int, int > >, int, int > input = read_input( "input.txt" );
    cout << "Ancho: " << get<1>( input ) << ", Alto:" << get<2>( input ) << endl;
    for( int i = 0; i < get<0>( input ).size(); i++ ){
        cout << "[x=" << get<0>( get<0>( input )[i] ) << ", r=" << get<1>( get<0>( input )[i] ) << "] ";
    };
    cout << endl;
    voracious( input );
    return 0;
};

tuple < vector < tuple < int, int > >, int, int > read_input( string file_name ){
    tuple < vector < tuple < int, int > >, int, int > to_return;
    vector < tuple < int, int > > points_info;
    int width, height = 0;
    bool first_line = true;
    ifstream f_input( file_name.c_str() );
    string line = "";
    while( getline( f_input, line ) ){
        if( ( line[0] != '#' ) && ( line[0] != ' ' ) ){
            if( first_line ){
                vector < string > map_info = string_tokenizer( line, ' ' );
                width = stoi( map_info[0] );
                height = stoi( map_info[1] );
                first_line = false;
            }else{
                vector < string > map_info = string_tokenizer( line, ' ' );
                tuple < int, int > point_info = make_tuple( stoi( map_info[0] ), stoi( map_info[1] ) );
                points_info.push_back( point_info );
            };
        };
    };
    f_input.close();
    to_return = make_tuple( points_info, width, height );
    return to_return;
};

vector < string > string_tokenizer( string string_to_tok, char separator ){
    vector < string > to_return;
    string tmp_line = "";
    for( int i = 0; i < sizeof( string_to_tok ); i++ ){
        if( string_to_tok[i] == separator ){
            to_return.push_back( tmp_line );
            tmp_line = "";
        }else{
            tmp_line.push_back( string_to_tok[i] );
        };
    };
    to_return.push_back( tmp_line );
    return to_return;
};

vector < tuple < int, int > > voracious( tuple < vector < tuple < int, int > >, int, int > input ){
    int width = get< 1 >( input );
    int height = get< 2 >( input );
    vector < tuple < int, int > > points =  get< 0 >( input );
    vector < int > radius;
    for( int i = 0; i < points.size(); i++ ){
        radius.push_back( get< 1 >( points[i] ) );
    };
};

vector < int > sort_by_radius( vector < int > input ){
    /*int mid = floor( input.size() / 2 );
    if( input.size() == 1 ){
        return input;
    }else if( input.size() == 2 ){
        if( input[0] > input[1] ){
            iter_swap( input[0], input[1] );
        }else{
            return input;
        };
    }else{
    };*/
    vector < int > to_return = input;
    for( int i = 0; i < to_return.size(); i++ ){
        for( int j = 0; j < input.size(); j++ ){
            if( to_return[i] > input[j] ){
                to_return[i] == input[j];
                input.erase( input.begin() + j );
            };
        };
        cout << to_return[i] << endl;
    };
    return to_return;
};

