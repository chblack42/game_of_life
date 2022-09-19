#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "life/coordinate.h"
#include "life/world.h"
#include "life/tests.h"
#include <assert.h>
#include <sstream>

#define TEST false

using namespace life;

int64_t convert_to_int64(char const * int64)
{
    errno = 0;
    int64_t result = strtoll(int64, NULL, 10);
    if (errno == ERANGE)
    {
        throw std::out_of_range("Out of range");
    }
    return result;
}

coordinate parse_coord(std::string const& coord)
{
    //check for correct input format
    if (std::regex_match(coord, std::regex("^\\(-?\\d+, -?\\d+\\)")))
    {

        size_t x_length = coord.find(",") - 1; // find difference between start of the first number and end
        std::string x_coord(coord.substr(1, x_length));

        size_t y_legnth = coord.find(")") - (coord.find(" ") + 1); // find difference between start of the second number and end
        std::string y_coord(coord.substr(coord.find(" ") + 1, y_legnth));

        coordinate coord;
        coord.x = convert_to_int64(x_coord.c_str());
        coord.y = convert_to_int64(y_coord.c_str());
        return coord;
    }
    else
    {
        throw std::exception("Does not match");
    }

}

void gather_input(std::vector<coordinate>& population,std::string const & input)
{
    std::stringstream sentance_stream(input);
    std::string input_coord;
    try {
        while (getline(sentance_stream, input_coord))
        {
            population.push_back(parse_coord(input_coord));
        }
    }
    catch (std::out_of_range e)
    {
        std::cout << "Input is out of range";
        throw;
    }
    catch (...)
    {
        std::cout << "Bad String Input";
        throw std::invalid_argument("Bad Input String");
    }

}


void gather_input(std::vector<coordinate> & population)
{
    std::cout << "Welcome to the game of life!" << std::endl << std::endl;
    std::cout << "Please enter your life coordinates, (-1, 123456), and press enter. Leaving a blank line wil start the simulation:" << std::endl;
    std::string input_coord;
    while (std::getline(std::cin, input_coord))
    {
        if (input_coord.size() == 0)
            break;
        try 
        {
           population.push_back(parse_coord(input_coord));
        }
        catch (...)
        {
            std::cout << "Bad input: try again or press enter to submit" << std::endl;
        }
    }
}


void test()
{
    for (int test = 0; test != test_vector.size(); ++test)
    {
        std::vector<coordinate> population = std::vector<coordinate>();
        gather_input(population, test_vector[test]);
        std::shared_ptr<world> world = create_world(population);
        to_cout(world.get());
        for (int i = 0; i != 10; ++i)
        {
            simulate(world.get());
            std::cout << "GEn:" << i + 1 << std::endl;
            to_cout(world.get());
        }
        std::string result = to_string(world.get());
        to_cout(world.get());
        assert(result == test_result_vector[test]);
    }

}

int main()
{
    // gather input
    if(TEST)
    {
        test();
        return 0;
    }
    
    std::vector<coordinate> population = std::vector<coordinate>();
    gather_input(population);
    std::shared_ptr<world> world = create_world(population);
    for (int i = 0; i != 10; ++i)
    {
        simulate(world.get());
    }
    to_cout(world.get());   

}