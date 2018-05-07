#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <fstream>

using namespace std;

// Block of declarations
tuple < vector < tuple < int, int > >, int, int > read_input( string file_name );
vector < string > string_tokenizer( string string_to_tok, char separator );
vector < tuple < int, int > > solve_by_divide_and_conquer( tuple < vector < tuple < int, int > >, int, int > map_info );
// End block of declarations

int main( int argc, const char* argv[] ){
    tuple < vector < tuple < int, int > >, int, int > input = read_input( "input.txt" );
    cout << "Ancho: " << get<1>( input ) << ", Alto:" << get<2>( input ) << endl;
    for( int i = 0; i < get<0>( input ).size(); i++ ){
        cout << "[x=" << get<0>( get<0>( input )[i] ) << ", r=" << get<1>( get<0>( input )[i] ) << "] ";
    };
    cout << endl;
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

vector < tuple < int, int > > solve_by_divide_and_conquer( tuple < vector < tuple < int, int > >, int, int > map_info ){
    vector < tuple < int, int > > to_return;
    
    return to_return;
};