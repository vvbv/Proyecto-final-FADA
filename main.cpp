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
vector < tuple < int, int > > divide_n_conquer2_( vector < tuple < int, int > > input, vector < tuple < int, int > > fixed_points, int width, int height );
vector < tuple < int, int > > divide_n_conquer3_( vector < tuple < int, int > > input, int width, int height );
vector < tuple < int, int > > dynamic( tuple < vector < tuple < int, int > >, int, int > input );
vector < tuple < int, int > > dynamic_( vector < tuple < int, int > > input, vector < tuple < int, int > > fixed_points, int width, int height );
vector < tuple < int, int > > dynamic2_( vector < tuple < int, int > > input, int width, int height );
vector < tuple < int, int > > dynamic3_( vector < tuple < int, int > > input, int width, int height );
vector < int > sort_by_radius( vector < int > input );
bool are_intersected( tuple < int, int > new_point, vector < tuple < int, int > > fixed_points, int  height, int width );
vector < tuple < int, int > > divide_n_conquer_merge_( vector < tuple < int, int > > input_0, vector < tuple < int, int > > input_1, int width, int height );
string get_geogebra_plot_command( vector < tuple < int, int > > points, int  height, int width );
vector < vector < tuple < int, int > > > split_vector( vector < tuple < int, int > > vector_, int split_point );
vector < tuple < int, int > > merge_vectors( vector < tuple < int, int > > vector_0, vector < tuple < int, int > > vector_1 );
vector < tuple < int, int > > max( vector < tuple < int, int > > vector_0, vector < tuple < int, int > > vector_1 );
double circle_area( double radius );
void merge_sort(int arr[], int l, int r);
void merge(int arr[], int l, int m, int r);
double calculate_input_area( vector < tuple < int, int > > input );
// End block of declarations

int main( int argc, const char* argv[] ){

    tuple < vector < tuple < int, int > >, int, int > input = read_input( "input.txt" );

    int width = get< 1 >( input );
    int height = get< 2 >( input );
    vector < tuple < int, int > > points =  get< 0 >( input );

    cout << endl << "Comandos para Geogebra [ Entrada ]: ";
    cout << get_geogebra_plot_command( points, height, width ) << endl;

    vector < tuple < int, int > > greedy_solution = greedy( input );
    cout << endl << "Comandos para Geogebra [ Salida: Voraz, Área: " << calculate_input_area( greedy_solution ) << " ]: ";
    cout << get_geogebra_plot_command( greedy_solution, height, width ) << endl;

    vector < tuple < int, int > > divide_n_conquer_solution = divide_n_conquer( input );
    cout << endl << "Comandos para Geogebra [ Salida: Divide y vencerás, Área: " << calculate_input_area( divide_n_conquer_solution ) << " ]: ";
    cout << get_geogebra_plot_command( divide_n_conquer_solution, height, width ) << endl;

    vector < tuple < int, int > > dynamic_solution = dynamic( input );
    cout << endl << "Comandos para Geogebra [ Salida: Programación dinámica, Área: " << calculate_input_area( dynamic_solution ) << " ]: ";
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
    for( int i = 0; i < string_to_tok.size(); i++ ){
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
        bool are_intersected_ = are_intersected_ = are_intersected( sorted_points[i], to_return, height, width );
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
    vector < tuple < int, int > > input_x = get<0>( input );
    //Limpiando la entrada
    for( int i = 0; i < input_x.size(); i++ ){
        if( are_intersected( input_x[i], fixed_points, height, width ) ){
            input_x.erase(input_x.begin() + i);
        };
    };
    int input_x_size = input_x.size();
    vector < int > radius;
    vector < tuple < int, int > > sorted_points;
    for( int i = 0; i < input_x.size(); i++ ){
        radius.push_back( get< 1 >( input_x[i] ) );
    };
    int radius_size = radius.size();
    radius = sort_by_radius( radius );
    for( int i = 0; i < radius_size; i++ ){
        for( int j = 0; j < input_x_size; j++ ){
            if( radius[i] == get< 1 >( input_x[j] ) ){
                sorted_points.push_back( input_x[j] );
                input_x.erase( input_x.begin() + j );
            };
        };
    };
    reverse( sorted_points.begin(), sorted_points.end() );

    //return divide_n_conquer_( get<0>( input ), fixed_points, width, height );
    //return divide_n_conquer2_( input_x, fixed_points, width, height );
    vector < tuple < int, int > > to_return = divide_n_conquer3_( sorted_points, width, height );
    int to_return_size = to_return.size();
    for(int i = 0; i < to_return_size; i++){
        if( ( get<0>( to_return[i] ) == 0) && ( get<1>( to_return[i] ) == 0 ) ){
            to_return.erase( to_return.begin() + i );
        };
    };
    return to_return;
};

vector < tuple < int, int > > divide_n_conquer3_( vector < tuple < int, int > > input, int width, int height ){
    int input_size = input.size();
    if( input_size == 1 ){
        return input;
    }else{
        int s_p = input_size/2;
        vector < vector < tuple < int, int > > > vector_splited = split_vector( input, s_p );
        return divide_n_conquer_merge_(
            divide_n_conquer3_( vector_splited[0], width, height ),
            divide_n_conquer3_( vector_splited[1], width, height ),
            width,
            height
        );
    };
};

vector < tuple < int, int > > divide_n_conquer_merge_( vector < tuple < int, int > > input_0, vector < tuple < int, int > > input_1, int width, int height ){
    vector < tuple < int, int > > to_return;
    int size_input_0 = input_0.size();
    int size_input_1 = input_1.size();
    int i = 0;
    int j = 0; 
    int k;
    if( size_input_0 >= size_input_1 ){
        k = size_input_0 + ( size_input_0 - size_input_1 );
    }else{
        k = size_input_1 + ( size_input_1 - size_input_0 );
    };
    while( k >= 0 ){
        if( ( size_input_0 > 0 ) && ( size_input_1 > 0 ) ){
            int area_i = circle_area( get<1>( input_0[i] ) );
            int area_j = circle_area( get<1>( input_1[j] ) );
            if( area_i >= area_j ){
                if( !are_intersected( input_0[i], to_return, height, width ) ){
                    to_return.push_back( input_0[i] );
                };
                i++;
            }else{
                if( !are_intersected( input_1[j], to_return, height, width ) ){
                    to_return.push_back( input_1[j] );
                };
                j++;
            };
        }else if( ( size_input_0 == 0 ) && ( size_input_1 > 0 ) ){
            if( !are_intersected( input_1[j], to_return, height, width ) ){
                to_return.push_back( input_1[j] );
            };
            j++;
        }else if( ( size_input_0 > 0 ) && ( size_input_1 == 0 ) ){
            if( !are_intersected( input_0[i], to_return, height, width ) ){
                to_return.push_back( input_0[i] );
            };
            i++;
        };
        k--;
    };
    return to_return;
};

vector < tuple < int, int > > divide_n_conquer2_( vector < tuple < int, int > > input, vector < tuple < int, int > > fixed_points, int width, int height ){

     if( input.size() == 1 ){
         if( !are_intersected( input[0], fixed_points, height, width  ) ){
            fixed_points.push_back( input[0] );
            return fixed_points;
         };
    }else if( input.size() == 2 ){
        vector < tuple < int, int > > points;
        vector < tuple < int, int > > to_return;
        points.push_back( input[1] );
        if( !are_intersected( input[0], points, height, width  ) ){
            points.push_back( points[0] );
            return points;
        }else{
            if( get<1>(input[0]) > get<1>(input[1]) ){
                to_return.push_back( input[0] );
            }else{
                to_return.push_back( input[1] );
            };
            return to_return;
        };
    }else{
        int s_p = input.size()/2;
        vector < vector < tuple < int, int > > > vector_splited = split_vector( input, s_p );
        //vector < tuple < int, int > > max( vector < tuple < int, int > > vector_0, vector < tuple < int, int > > vector_1 );
        return merge_vectors( 
            max ( 
                divide_n_conquer2_( vector_splited[0], fixed_points, width, height ), fixed_points ), 
                merge_vectors( 
                        max( divide_n_conquer2_( vector_splited[1], fixed_points, width, height ),fixed_points ), fixed_points )
                );
    };

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
    // int area = width * height;
    //Limpiando la entrada
    for( int i = 0; i < points.size(); i++ ){
        if( are_intersected( points[i], empty, height, width ) ){
            points.erase(points.begin() + i);
        };
    };
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
            }else{
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
    vector < tuple < int, int > > empty;
    int input_size = input.size();
    //Limpiando la entrada
    for( int i = 0; i < input_size; i++ ){
        if( are_intersected( input[i], empty, height, width ) ){
            input.erase(input.begin() + i);
        };
    };
    input_size = input.size();
    vector < tuple < int, int, double > > vec_start_end_n_area;
    int array_end_points[ input_size ];
    for( int i = 0; i < input_size; i++ ){
        int point = get<0>( input[i] );
        int radius = get<1>( input[i] );
        tuple < int, int, double > tmp = make_tuple( (point - radius), (point + radius), circle_area( radius ) );
        vec_start_end_n_area.push_back( tmp );
        array_end_points[ i ] = (point + radius);
    };
    merge_sort( array_end_points, 0, input_size - 1 );
    // std::cout << endl;
    // for(int i = 0; i < input_size; i++){
    //     std::cout << "Start Point: " << get<0>( vec_start_end_n_area[i] ) << " end point " << get<1>( vec_start_end_n_area[i] ) << std::endl;
    //     std::cout << array_end_points[i] << std::endl; 
    // };

    vector < tuple < int, int, double > > sorted_vec_start_end_n_area;
    for(int i = 0; i < input_size; i++){
        int vec_start_end_n_area_size = vec_start_end_n_area.size();
        for(int j = 0; j < vec_start_end_n_area_size; j++){
            int end = get<1>( vec_start_end_n_area[j] );
            if( array_end_points[i] == end ){
                sorted_vec_start_end_n_area.push_back( vec_start_end_n_area[j] );
                vec_start_end_n_area.erase( vec_start_end_n_area.begin() + j );
            };
        };
    };

    std::cout << std::endl << "Sorted circles" << std::endl;
    for(int i = 0; i < input_size; i++){
        std::cout << "Start Point: " << get<0>( sorted_vec_start_end_n_area[i] ) << ", " << get<1>( sorted_vec_start_end_n_area[i] ) << ", " << get<2>( sorted_vec_start_end_n_area[i] )  << std::endl;
    };

    double total_areas[ input_size ]; 
    // Fist case
    total_areas[ 0 ] = get<2>( sorted_vec_start_end_n_area[ 0 ] );

    vector < tuple < vector < tuple < int, int, double > >, int > > sub_solutions;
    int index_of_circles[ input_size ][ input_size ];
    for( int i = 0; i < input_size; i++ ){
        for( int j = 0; j < input_size; j++ ){
            index_of_circles[i][j] = 0;
        };
    };

    index_of_circles[0][0] = 1;

    for( int i = 1; i < input_size; i++ ){
        
        int start = get<0>( sorted_vec_start_end_n_area[ i ] );
        double acumulated_area = get<2>( sorted_vec_start_end_n_area[i] );
        int new_j = -1;
        //vector < tuple < int, int, double > > sub_solution;
        for( int j = i - 1; j >= 0; j-- ){
            int sub_end = get<1>( sorted_vec_start_end_n_area[ j ] );
            if( sub_end <= start ){
                new_j = j;
                break;
            };
        };
        if( new_j != -1 ){
            acumulated_area += total_areas[ new_j ];
        };
        
        // CASO 1 AUMENTA Y ENCUENTRA
        // CASO 2 NO AUMENTA PERO ENCUENTRA
        // CASO 3 NO AUMENTA NO ENCUENTRA
        
        total_areas[i] = std::max( total_areas[ i - 1 ], acumulated_area );
        /*if( ( total_areas[i] == acumulated_area ) && (  new_j != -1 ) ){
            for( int x = 0; x < input_size; x++ ){
                //index_of_circles[i][x] = index_of_circles[i-1][x];
                index_of_circles[i][x] = 0;
             };
            index_of_circles[i][new_j] = 1;
            index_of_circles[i][i] = 1;
        }else if( ( total_areas[i] == total_areas[ i - 1 ] ) && (  new_j != -1 ) ){
            for( int x = 0; x < input_size; x++ ){
                //index_of_circles[i][x] = index_of_circles[i-1][x];
                index_of_circles[i][x] = 0;
            };
            //index_of_circles[i][new_j] = 1;
            index_of_circles[i][new_j] = 1;
            index_of_circles[i][i] = 1;
        }else if( ( total_areas[i] == total_areas[ i - 1 ] ) && (  new_j == -1 ) ){
            for( int x = 0; x < input_size; x++ ){
                index_of_circles[i][x] = index_of_circles[i-1][x];
             };
            //index_of_circles[i][new_j] = 1;
        }else if( ( total_areas[i] == acumulated_area ) && (  new_j == -1 ) ){
            for( int x = 0; x < input_size; x++ ){
                index_of_circles[i][x] = 0;
             };
            index_of_circles[i][i] = 1;
        };*/
        
        if( new_j != -1 ){
            index_of_circles[i][new_j] = 2;
            index_of_circles[i][i] = 1;
        }else{
            index_of_circles[i][i] = 1;
        };

        //tuple < vector < tuple < int, int, double > >, int > sub_solution_with_area = make_tuple( sub_solution, acumulated_area );
        //sub_solutions.push_back( sub_solution_with_area );
    };

    std::cout << "\n" ;
    double matrix_areas[ input_size ];
    int id_max_matrix_areas = 0;
    for( int i = 0; i < input_size; i++ ){
        matrix_areas[ i ] = 0;
        for( int j = 0; j < input_size; j++ ){
            std::cout << index_of_circles[i][j] << " ";
            if( ( index_of_circles[i][j] == 1 ) || ( index_of_circles[i][j] == 2 ) ){
                matrix_areas[ i ] += get<2>( sorted_vec_start_end_n_area[ j ] );
            };
        };
        if( matrix_areas[ i ] >= matrix_areas[ id_max_matrix_areas ] ){
            id_max_matrix_areas = i;
        };
        std::cout << "  <= BA: " << total_areas[i] << " A: " << matrix_areas[i];
        std::cout << endl;
    };

    std::cout << "\nÁrea máxima, prog dinámica: " << total_areas[ input_size - 1 ] << std::endl;

    // std::cout << std::endl << "Areas" << std::endl;
    // int id_max_subsolution = 0;
    // int total_area_solution = 0;
    // for(int i = 0; i < sub_solutions.size(); i++){
    //     total_area_solution += get<1>(sub_solutions[i]);
    //     std::cout << "Sub_sol Area: " << get<1>( sub_solutions[ i ] ) << " T:" << get<0>( sub_solutions[ i ] ).size() << std::endl;
    //     int i_total_area_sub_solution = get<1>( sub_solutions[ i ] );
    //     int current_total_area_solution = get<1>( sub_solutions[ id_max_subsolution ] );
    //     if( i_total_area_sub_solution >= current_total_area_solution ){
    //         id_max_subsolution = i;
    //     };
    // };
    
    // vector < tuple < int, int, double > > best_solution = get<0>( sub_solutions[ id_max_subsolution ] );
    // int total_area_best_solution = get<1>( sub_solutions[ id_max_subsolution ] );
    // int size_of_elements_in_solution = best_solution.size();
    // for(int i = 0; i < size_of_elements_in_solution; i++){
    //     std::cout << "Start Point: " << get<0>( best_solution[i] ) << " end point " << get<1>( best_solution[i] ) << std::endl;
    // };

    // Build solution
    vector < int > index_sol_circles;
    int i = id_max_matrix_areas;
    int sorted_vec_start_end_n_area_size = sorted_vec_start_end_n_area.size();
    while( i >= 0 ){
        bool flag_two = false;
        int pivot = -1;
        for( int j = 0; j < sorted_vec_start_end_n_area_size; j++ ){
            if( index_of_circles[ i ][ j ] == 1 ){
                int index_sol_circles_size = index_sol_circles.size();
                bool exist = false;
                for( int k = 0; k < index_sol_circles_size; k++ ){
                    if( index_sol_circles[k] == j ){
                        exist = true;
                        break;
                    };
                };
                if( !exist ){
                    index_sol_circles.push_back( j );
                };
            };
            if( index_of_circles[ i ][ j ] == 2 ){
                int index_sol_circles_size = index_sol_circles.size();
                bool exist = false;
                for( int k = 0; k < index_sol_circles_size; k++ ){
                    if( index_sol_circles[k] == j ){
                        exist = true;
                        break;
                    };
                };
                if( !exist ){
                    index_sol_circles.push_back( j );
                };
                flag_two = true;
                pivot = j;
            };
        };
        if( !flag_two ){
            break;
        }else{
            i = pivot;
        };
    };

    int index_sol_circles_size = index_sol_circles.size();
    for( int i = 0; i < index_sol_circles_size; i++ ){
        int radius = ( get<1>( sorted_vec_start_end_n_area[ index_sol_circles[i] ] ) - get<0>( sorted_vec_start_end_n_area[ index_sol_circles[i] ] ) ) / 2;
        int point = ( ( get<1>( sorted_vec_start_end_n_area[ index_sol_circles[i] ] ) - get<0>( sorted_vec_start_end_n_area[ index_sol_circles[i] ] ) ) / 2 ) + get<0>( sorted_vec_start_end_n_area[ index_sol_circles[i] ] );
        tuple < int, int > circle_r = make_tuple( point, radius );
        to_return.push_back( circle_r );
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
    int area_0 = 0;
    int area_1 = 0;
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

void merge_sort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l+(r-l)/2;
 
        merge_sort(arr, l, m);
        merge_sort(arr, m+1, r);
 
        merge(arr, l, m, r);
    };
};

void merge(int arr[], int l, int m, int r){

    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    int L[n1], R[n2];
 
    for (i = 0; i < n1; i++){
        L[i] = arr[l + i];
    };
    for (j = 0; j < n2; j++){
        R[j] = arr[m + 1+ j];
    };

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2){
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    };
};

double calculate_input_area( vector < tuple < int, int > > input ){
    double to_return = 0;
    int input_size = input.size();
    for( int i = 0; i < input_size; i++ ){
        to_return += circle_area( get<1>( input[ i ] ) );
    };
    return to_return;
};
