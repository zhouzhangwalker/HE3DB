#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>

namespace HEDB
{
    inline void cal_avg_var(std::vector<double> &nums, double &avg, double &var, double &std)
    {
        double sum = std::accumulate(nums.begin(), nums.end(), 0.0);
        avg = sum / nums.size();
        double accum = 0.0;
        for (size_t i = 0; i < nums.size(); i++)
        {
            accum += (nums[i] - avg) * (nums[i] - avg);
        }
        var = accum / nums.size();
        std = sqrt(var);
    }

    template <typename T>
    void print_array(std::vector<T> array)
    {
        int N = array.size();
        for (int i=0; i<N; i++)
        {
            std::cout << array[i] << " " ;
        }
        std::cout << std::endl;
		    
    }

    /*
    https://github.com/microsoft/SEAL/blob/main/native/examples/examples.h
    Helper function: Prints a vector of floating-point values.
    */
    template <typename T>
    inline void print_vector(std::vector<T> vec, std::size_t print_size = 4, int prec = 3)
    {
        /*
        Save the formatting information for std::cout.
        */
        std::ios old_fmt(nullptr);
        old_fmt.copyfmt(std::cout);

        std::size_t slot_count = vec.size();

        std::cout << std::fixed << std::setprecision(prec);
        std::cout << std::endl;
        if (slot_count <= 2 * print_size)
        {
            std::cout << "    [";
            for (std::size_t i = 0; i < slot_count; i++)
            {
                std::cout << " " << vec[i] << ((i != slot_count - 1) ? "," : " ]\n");
            }
        }
        else
        {
            vec.resize(std::max(vec.size(), 2 * print_size));
            std::cout << "    [";
            for (std::size_t i = 0; i < print_size; i++)
            {
                std::cout << " " << vec[i] << ",";
            }
            if (vec.size() > 2 * print_size)
            {
                std::cout << " ...,";
            }
            for (std::size_t i = slot_count - print_size; i < slot_count; i++)
            {
                std::cout << " " << vec[i] << ((i != slot_count - 1) ? "," : " ]\n");
            }
        }
        std::cout << std::endl;

        /*
        Restore the old std::cout formatting.
        */
        std::cout.copyfmt(old_fmt);
    }

    /*
    https://github.com/microsoft/SEAL/blob/main/native/examples/examples.h
    Helper function: Prints a matrix of values.
    */
    template <typename T>
    inline void print_matrix(std::vector<T> matrix, std::size_t row_size)
    {
        /*
        We're not going to print every column of the matrix (there are 2048). Instead
        print this many slots from beginning and end of the matrix.
        */
        std::size_t print_size = 5;

        std::cout << std::endl;
        std::cout << "    [";
        for (std::size_t i = 0; i < print_size; i++)
        {
            std::cout << std::setw(3) << std::right << matrix[i] << ",";
        }
        std::cout << std::setw(3) << " ...,";
        for (std::size_t i = row_size - print_size; i < row_size; i++)
        {
            std::cout << std::setw(3) << matrix[i] << ((i != row_size - 1) ? "," : " ]\n");
        }
        std::cout << "    [";
        for (std::size_t i = row_size; i < row_size + print_size; i++)
        {
            std::cout << std::setw(3) << matrix[i] << ",";
        }
        std::cout << std::setw(3) << " ...,";
        for (std::size_t i = 2 * row_size - print_size; i < 2 * row_size; i++)
        {
            std::cout << std::setw(3) << matrix[i] << ((i != 2 * row_size - 1) ? "," : " ]\n");
        }
        std::cout << std::endl;
    }
} // namespace HEDB
