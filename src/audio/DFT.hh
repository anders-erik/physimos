
#pragma once

#include <complex>
using namespace std::complex_literals;

#include "lib/arr.hh"
#include "math/vec.hh"

class DFT
{
public: 
	// Arr<std::complex<float>> input;
	// Arr<std::complex<float>> output;

	// DFT(Arr<std::complex<float>> _input, Arr<std::complex<float>> _output)
	// 	: 	input {_input},
	// 		output {_output}
	// {}

	static Arr<std::complex<double>> calculate(Arr<std::complex<double>> input)
	{
		Arr<std::complex<double>> output = {};

		int N = input.count();

		for(int k = 0; k < N; k++)
		{
			std::complex<double> X = 0.0 + 0.0i;

			for(int n = 0; n < N; n++)
			{
				double n_db = (double) n;
				double k_db = (double) k;
				double N_db = (double) N;
				std::complex<double> exponent = 0.0 + -1.0i * 2.0 * 3.1415 * n_db * k_db / N_db;
				// std::complex<double> exponent = 0.0 + 1.0i;
				X += input[n] * std::pow(2.718, exponent);
			}

			output.push_back(X);

		}
		// std::complex<double> A =
		return output;
	}

    static Vec<std::complex<double>> calculate(Vec<std::complex<double>> input)
	{
		Vec<std::complex<double>> output;

		uint N = input.size();

        output.set_size(N);
        

		for(uint k = 0; k < N; k++)
		{
			std::complex<double> X = 0.0 + 0.0i;

			for(int n = 0; n < N; n++)
			{
				double n_db = (double) n;
				double k_db = (double) k;
				double N_db = (double) N;
				std::complex<double> exponent = 0.0 + -1.0i * 2.0 * 3.1415 * n_db * k_db / N_db;
				// std::complex<double> exponent = 0.0 + 1.0i;
				X += input[n] * std::pow(2.718, exponent);
			}

			// output.push_back(X);
            output[k] = X;

		}
		// std::complex<double> A =
		return output;
	}
};

double complex_magnitude(std::complex<double> _num)
{
    double r = _num.real();
    double i = _num.imag();

    return sqrt( r*r + i*i);
}

Vec<double> complex_vec_to_mag_vec(Vec<std::complex<double>> vec)
{
    Vec<double> ret_vec;
    ret_vec.set_size(vec.size());

    for(uint i = 0; i < vec.size(); i++)
    {
        ret_vec[i] = complex_magnitude(vec[i]);
    }
    
    return ret_vec;
}

void print_complex_vec(Vec<std::complex<double>> vec)
{
    print("\n");
    for(uint i = 0; i < vec.size(); i++)
    {
        print(Str::FL(vec[i].real(), 3, Str::FloatRep::Fixed));
        print(" + ");
        print(Str::FL(vec[i].imag(), 3, Str::FloatRep::Fixed));
        print(" i \n");
    }
    print("\n");
};

void print_vec(Vec<double> vec)
{
    print("\n");
    for(uint i = 0; i < vec.size(); i++)
    {
        Print::ln(Str::FL(vec[i], 3, Str::FloatRep::Fixed));
    }
    print("\n");
};