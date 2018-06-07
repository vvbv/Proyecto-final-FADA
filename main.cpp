#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <fstream>
#include <algorithm>
#include <limits>
#include <iterator>
#include <armadillo>
#include <cmath>
#include <iomanip>

using namespace std;

// Block of global vars
const long double PI = 3.141592653589793238L;
// Block of global vars

// Block of declarations
tuple < vector < tuple < int, int > >, int, int > read_input( string file_name );
vector < string > string_tokenizer( string string_to_tok, char separator );
vector < tuple < int, int > > greedy( tuple < vector < tuple < int, int > >, int, int > input );
vector < tuple < int, int > > divide_n_conquer( tuple < vector < tuple < int, int > >, int, int > input );
vector < tuple < int, int > > divide_n_conquer_( vector < tuple < int, int > > input, vector < tuple < int, int > > fixed_points, int width, int height );
vector < tuple < int, int > > dynamic( tuple < vector < tuple < int, int > >, int, int > input );
vector < tuple < int, int > > dynamic_( vector < tuple < int, int > > input, vector < tuple < int, int > > fixed_points, int width, int height );
vector < tuple < int, int > > dynamic2_( vector < tuple < int, int > > input, int width, int height );
vector < tuple < int, int > > dynamic3_( vector < tuple < int, int > > input, int width, int height );
vector < int > sort_by_radius( vector < int > input );
bool are_intersected( tuple < int, int > new_point, vector < tuple < int, int > > fixed_points, int  height, int width );
string get_geogebra_plot_command( vector < tuple < int, int > > points, int  height, int width );
vector < vector < tuple < int, int > > > split_vector( vector < tuple < int, int > > vector_, int split_point );
vector < tuple < int, int > > merge_vectors( vector < tuple < int, int > > vector_0, vector < tuple < int, int > > vector_1 );
vector < tuple < int, int > > max( vector < tuple < int, int > > vector_0, vector < tuple < int, int > > vector_1 );
double circle_area( double radius );
// End block of declarations

int main( int argc, const char* argv[] ){

    tuple < vector < tuple < int, int > >, int, int > input = read_input( "input.txt" );

    int width = get< 1 >( input );
    int height = get< 2 >( input );
    vector < tuple < int, int > > points =  get< 0 >( input );

    cout << endl << "Comandos para Geogebra [ Entrada ]: ";
    cout << get_geogebra_plot_command( points, height, width ) << endl;

    vector < tuple < int, int > > greedy_solution = greedy( input );
    cout << endl << "Comandos para Geogebra [ Salida: Voraz ]: ";
    cout << get_geogebra_plot_command( greedy_solution, height, width ) << endl;

    vector < tuple < int, int > > divide_n_conquer_solution = divide_n_conquer( input );
    cout << endl << "Comandos para Geogebra [ Salida: Divide y vencerás ]: ";
    cout << get_geogebra_plot_command( divide_n_conquer_solution, height, width ) << endl;

    vector < tuple < int, int > > dynamic_solution = dynamic( input );
    cout << endl << "Comandos para Geogebra [ Salida: Programación dinámica ]: ";
    cout << get_geogebra_plot_command( dynamic_solution, height, width ) << endl;

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

vector < tuple < int, int > > greedy( tuple < vector < tuple < int, int > >, int, int > input ){
    vector < tuple < int, int > > to_return; 
    int width = get< 1 >( input );
    int height = get< 2 >( input );
    vector < tuple < int, int > > points =  get< 0 >( input );
    vector < int > radius;
    for( int i = 0; i < points.size(); i++ ){
        radius.push_back( get< 1 >( points[i] ) );
    };
    radius = sort_by_radius( radius );
    vector < tuple < int, int > > sorted_points;
    for( int i = 0; i < radius.size(); i++ ){
        for( int j = 0; j < points.size(); j++ ){
            if( radius[i] == get< 1 >( points[j] ) ){
                sorted_points.push_back( points[j] );
                points.erase( points.begin() + j );
            };
        };
    };
    reverse( sorted_points.begin(), sorted_points.end() );
    for( int i = 0; i < sorted_points.size(); i++ ){
        bool are_intersected_ = true;
        are_intersected_ = are_intersected( sorted_points[i], to_return, height, width );
        if( !are_intersected_ ){
            to_return.push_back( sorted_points[i] );
        };
        //cout << "P: " << get<0>( sorted_points[i] ) << " - R:" << get<1>( sorted_points[i] ) << " - TR: " << are_intersected_ << endl;
    };
    return to_return;
};

vector < tuple < int, int > > divide_n_conquer( tuple < vector < tuple < int, int > >, int, int > input ){
    int width = get< 1 >( input );
    int height = get< 2 >( input );
    vector < tuple < int, int > > fixed_points;
    return divide_n_conquer_( get<0>( input ), fixed_points, width, height );
};

vector < tuple < int, int > > divide_n_conquer_( vector < tuple < int, int > > input, vector < tuple < int, int > > fixed_points, int width, int height ){
    if( input.size() == 0 ){
        return fixed_points;
    }else if( input.size() > 0 ){
        int major_radius_index = 0;
        for( int i = 0; i < input.size(); i++ ){
            if( get<1>( input[i] ) >= get<1>( input[major_radius_index] ) ){
                major_radius_index = i;
            };
        };
        bool a_i = are_intersected( input[major_radius_index], fixed_points, height, width );
        if( !a_i ){
            fixed_points.push_back( input[major_radius_index] );
            input.erase(input.begin() + major_radius_index);
            return divide_n_conquer_( input, fixed_points, width, height );
        }else{
            input.erase(input.begin() + major_radius_index);
            return divide_n_conquer_( input, fixed_points, width, height );
        };
    };
};

vector < tuple < int, int > > dynamic( tuple < vector < tuple < int, int > >, int, int > input ){
    vector < tuple < int, int > > points = get<0>( input );
    vector < tuple < int, int > > empty;
    int width = get< 1 >( input );
    int height = get< 2 >( input );
    int area = width * height;
    //return dynamic_( points, empty, width, height );
    //return dynamic2_( points, width, height );
    return dynamic3_( points, width, height );
};

vector < tuple < int, int > > dynamic_( vector < tuple < int, int > > input, vector < tuple < int, int > > fixed_points, int width, int height ){
    vector < tuple < int, int > > to_return;
    if( input.size() == 2 ){
        vector < vector < tuple < int, int > > > v_s = split_vector( input, 1 );
        bool a_i = are_intersected( v_s[0][0], v_s[1], height, width );
        if( !a_i ){
            return input;
        }else{
            vector < tuple < int, int > > empty;
            return empty;
        };
    }else{
        int s_p = input.size()/2;
        vector < vector < tuple < int, int > > > vector_splited = split_vector( input, s_p );
        fixed_points = merge_vectors( max( max (dynamic_( vector_splited[0], fixed_points, width, height ), fixed_points), max( dynamic_( vector_splited[1], fixed_points, width, height ), fixed_points ) ), fixed_points );
        return fixed_points;
    };
};

vector < tuple < int, int > > dynamic2_( vector < tuple < int, int > > input, int width, int height ){
    cout << endl << "DEBUG" << endl << endl;
    vector < tuple < int, int > > to_return;
    vector < tuple < int, int > > empty;
    vector < double > total_areas;
    int input_size = input.size();
    //Limpiando la entrada
    for( int i = 0; i < input_size; i++ ){
        if( are_intersected( input[i], empty, height, width ) ){
            input.erase(input.begin() + i);
        };
    };
    input_size = input.size();


    vector < int > radius;
    for( int i = 0; i < input.size(); i++ ){
        radius.push_back( get< 1 >( input[i] ) );
    };
    radius = sort_by_radius( radius );
    vector < tuple < int, int > > sorted_input;
    for( int i = 0; i < radius.size(); i++ ){
        for( int j = 0; j < input.size(); j++ ){
            if( radius[i] == get< 1 >( input[j] ) ){
                sorted_input.push_back( input[j] );
                input.erase( input.begin() + j );
            };
        };
    };
    reverse( sorted_input.begin(), sorted_input.end() );
    input = sorted_input;

    double areas[ input_size ][ input_size ];
    int id_major_area_row = 0;
    for( int i = 0; i < input_size; i++ ){
        double total_area_row = 0;
        for( int j = 0; j < input_size; j++ ){
            if( i == j ){
                if( !are_intersected( input[i], empty, height, width ) ){
                    areas[i][j] =PI * pow( get<1>( input[j] ), 2 ); 
                };
            }else if( i != j ){
                bool flag_valid = true;
                if( are_intersected( input[i], empty, height, width ) ){
                    flag_valid = false;
                };
                if( are_intersected( input[j], empty, height, width ) ){
                    flag_valid = false;
                };
                if( flag_valid ){
                    vector < tuple < int, int > > fixed_points;
                    fixed_points.push_back( input[j] );
                    flag_valid = !are_intersected( input[i], fixed_points, height, width );
                    cout << i << " " << j << " " << flag_valid << endl;
                };
                if( flag_valid ){
                    areas[i][j] = ( PI * pow( get<1>( input[i] ), 2 ) ) + ( PI * pow( get<1>( input[j] ), 2 ) ); 
                }else{
                    areas[i][j] = 0;
                };
            };
            total_area_row += areas[i][j];
        };
        total_areas.push_back( total_area_row );
        if( total_area_row > total_areas[id_major_area_row] ){
            id_major_area_row = i;
        };
    };

    for( int x = 0; x < input_size; x++ ){
        int total = 0;
        for( int y = 0; y < input_size; y++ ){
            cout << areas[x][y] << " # ";
            total += areas[x][y];
        };
        cout << "   => TOTAL:" << total;
        cout << endl;
    };

    int pivot_row = id_major_area_row;
    int filter_row[ input_size ][ input_size ];
    cout << endl;
    for( int i = 0; i < input_size; i++ ){
        cout << "Pivot: " << pivot_row << endl;
        for( int j = 0; j < input_size; j++ ){
            if( areas[pivot_row][j] != 0 ){
                filter_row[ i ][ j ] = 1;
            }else{
                filter_row[ i ][ j ] = 0;
            };
        };
        for( int k = i; k < input_size; k++ ){
            if( ( filter_row[ 0 ][k] == 1 ) ){
                pivot_row = k;
                break;
            };
        };
    };
    cout << endl << endl;
    for( int i = 0; i < input_size; i++ ){
        for( int j = 0; j < input_size; j++ ){
            cout << filter_row[i][j] << " ";
        };
        cout << endl;
    };
    cout << endl << "Resultado" << endl;
    int result[ input_size ];
    for( int i = 0; i < input_size; i++ ){
        bool zero_flag = false;
        for( int j = 0; j < input_size; j++ ){
            if( filter_row[j][i] == 0 ){
                zero_flag = true;
                break;
            };
        };
        if( zero_flag ){
            result[i] = 0;
        }else{
            result[i] = 1;
            to_return.push_back( input[i] );
        };
    };

    for( int i = 0; i < input_size; i++ ){
        cout << result[i] << " ";
    };
    cout << endl;

    for( int i = 0; i < input_size; i++ ){
        cout << get<0>(input[i]) << " - " << get<1>(input[i]) << " ||| ";
    };

    cout << endl << "END DEBUG" << endl;
    return to_return;
};

vector < tuple < int, int > > dynamic3_( vector < tuple < int, int > > input, int width, int height ){
    vector < tuple < int, int > > to_return;
    int input_size = input.size();
    vector < tuple < int, int, double > > vec_start_end_n_area;
    for( int i = 0; i < input_size; i++ ){
        int point = get<0>( input[i] );
        int radius = get<1>( input[i] );
        tuple < int, int, double > tmp = make_tuple( (point - radius), (point + radius), circle_area( radius ) );
        vec_start_end_n_area.push_back( tmp );
    };
    std::cout << endl;
    for(int i = 0; i < input_size; i++){
        std::cout << "Start Point: " << get<0>( vec_start_end_n_area[i] ) << " end point " << get<1>( vec_start_end_n_area[i] ) << std::endl;
    };

    return to_return;
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
    vector < int > to_return;
    int input_size = input.size();
    for( int i = 0; i < input_size; i++ ){
        int index_minor = -1;
        int value = std::numeric_limits<int>::max();;
        for( int j = 0; j < input.size(); j++ ){
            if( value > input[j] ){
                value = input[j];
                index_minor = j;
            };
        };
        to_return.push_back( input[index_minor] );
        input.erase( input.begin() + index_minor );
    };
    return to_return;
};

bool are_intersected( tuple < int, int > new_point, vector < tuple < int, int > > fixed_points, int  height, int width ){
    if( fixed_points.size() == 0 ){
        int point_x = get< 0 >( new_point );
        int radius_ = get< 1 >( new_point );
        double y_max = (double) radius_ + (double) ( (double) height / (double) 2 );
        double y_min = (double) ( (double) height / (double) 2 ) - (double) radius_;
        int x_max = point_x + radius_;
        int x_min = point_x - radius_;
        if( (y_min < 0)||(x_min < 0) ){
            //cout << "C1: " << y_min << " " << x_min << " =>";
            return true;
        }else if( (y_max > height)||(x_max > width) ){
            //cout << "C2" << endl;
            return true;
        }else{
            //cout << "CT: " << y_min << " " << x_min << " =>";
            return false;
        };
    }else{
        for( int i = 0; i < fixed_points.size(); i++ ){
            if( get< 0 >( fixed_points[i] ) ==  get< 0 >( new_point ) ){
                return true;
            }else{
                int fixed_point_x_max = get< 0 >( fixed_points[i] ) + get< 1 >( fixed_points[i] );
                int fixed_point_x_min = get< 0 >( fixed_points[i] ) - get< 1 >( fixed_points[i] ); 
                int point_x = get< 0 >( new_point );
                int radius_ = get< 1 >( new_point );
                double y_max = (double) radius_ + (double) ( (double) height / (double) 2 );
                double y_min =  (double) ( (double) height / (double) 2 ) - (double) radius_;
                int x_max = point_x + radius_;
                int x_min = point_x - radius_;
                if( (y_min < 0)||(x_min < 0) ){
                    return true;
                }else if( (y_max > height)||(x_max > width) ){
                    return true;
                }else if( 
                            //Casos 1 sentido
                            ( (x_max > fixed_point_x_min) && (x_max < fixed_point_x_max) ) || 
                            ( (x_min < fixed_point_x_max) && (x_min > fixed_point_x_min) ) || 
                            ( (x_max == fixed_point_x_max) && ( x_min == fixed_point_x_min ) ) ||
                            ( (x_max < fixed_point_x_max) && ( x_min == fixed_point_x_min ) ) ||
                            ( (x_max == fixed_point_x_max) && ( x_min > fixed_point_x_min ) )  ||
                            // Casos 2 sentido
                            ( (x_max > fixed_point_x_min) && (x_min < fixed_point_x_max) ) || 
                            ( (x_max > fixed_point_x_max) && (x_min < fixed_point_x_min) ) || 
                            ( (x_max > fixed_point_x_max) && ( x_min == fixed_point_x_min ) ) ||
                            ( (x_max == fixed_point_x_max) && ( x_min < fixed_point_x_min ) ) 
                        ){
                    //cout << "C3: " << x_max << " " << fixed_point_x_min << " - " << x_min << " " << fixed_point_x_max << " - " << " =>";
                    return true;
                };
            };
        };
        return false;
    };
};

string get_geogebra_plot_command( vector < tuple < int, int > > points, int  height, int width ){
    string command = "";
    command += "Execute[{\" x = " + to_string(0) + " \",";
    command += " \" x = " + to_string((double) width) + " \",";
    command += " \" y = " + to_string((double) height / (double) 2) + " \",";
    command += " \" y = -" + to_string((double) height / (double) 2) + " \",";
    for( int i = 0; i < points.size(); i++ ){
        command += " \" Circle(( " +  to_string(get<0>( points[i] )) + " ,0), " +  to_string(get<1>( points[i] )) + ")\"";
        if( i < ( points.size() - 1) ){
            command += ",";
        }else{
            command += "}]";
        };
    };
    return command;
};

vector < vector < tuple < int, int > > > split_vector( vector < tuple < int, int > > vector_, int split_point ) {
    vector < vector < tuple < int, int > > > to_return;
    vector < tuple < int, int > > vector_0;
    vector < tuple < int, int > > vector_1;
    bool vector_toggle = false;
    for( int i = 0; i < vector_.size(); i++ ){
        if( !vector_toggle ){
            vector_0.push_back( vector_[i] );
            if( i == ( split_point - 1 ) ){
                vector_toggle = true;
            };
        }else{
            vector_1.push_back( vector_[i] );
        };
    };
    to_return.push_back( vector_0 );
    to_return.push_back( vector_1 );
    return to_return;
};

vector < tuple < int, int > > merge_vectors( vector < tuple < int, int > > vector_0, vector < tuple < int, int > > vector_1 ) {
    vector < tuple < int, int > > to_return;
    for( int i = 0; i < vector_0.size(); i++ ){
        to_return.push_back( vector_0[i] );
    };
    for( int i = 0; i < vector_1.size(); i++ ){
        to_return.push_back( vector_1[i] );
    };
    return to_return;
};

vector < tuple < int, int > > max( vector < tuple < int, int > > vector_0, vector < tuple < int, int > > vector_1 ) {
    int area_0;
    int area_1;
    for( int i = 0; i < vector_0.size(); i++ ){
        area_0 += PI * pow( get<1>( vector_0[i] ), 2);
    };
    for( int i = 0; i < vector_1.size(); i++ ){
        area_1 += PI * pow( get<1>( vector_1[i] ), 2);
    };
    if( area_0 >= 1 ){
        return vector_0;
    }else{
        return vector_1;
    };
};

double circle_area( double radius ){
    return PI * pow( radius, 2 );
};
