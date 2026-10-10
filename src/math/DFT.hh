
#pragma once

#include <complex>
using namespace std::complex_literals;

#include "lib/arr.hh"
#include "math/vec.hh"
#include "math/complex.hh"


double complex_magnitude(std::complex<double> _num)
{
    double r = _num.real();
    double i = _num.imag();

    return sqrt( r*r + i*i);
}

Vec<double> complex_vec_to_mag_vec(Vec<std::complex<double>> vec)
{
    Vec<double> ret_vec;
    ret_vec.set_count(vec.size());

    for(uint i = 0; i < vec.size(); i++)
    {
        ret_vec[i] = complex_magnitude(vec[i]);
    }
    
    return ret_vec;
}

Vec<double> complex_vec_to_real_vec(Vec<std::complex<double>> vec)
{
    Vec<double> ret_vec;
    ret_vec.set_count(vec.size());

    for(uint i = 0; i < vec.size(); i++)
        ret_vec[i] = vec[i].real();
    
    return ret_vec;
}

Vec<double> complex_vec_to_imag_vec(Vec<std::complex<double>> vec)
{
    Vec<double> ret_vec;

    ret_vec.set_count(vec.size());

    for(uint i = 0; i < vec.size(); i++)
        ret_vec[i] = vec[i].imag();
    
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

void print_complex_vec_head(Vec<std::complex<double>> vec, uint _count = 10)
{
    print("\n");
    for(uint i = 0; i < _count; i++)
    {
        print(Str::FL(vec[i].real(), 3, Str::FloatRep::Fixed));
        print(" + ");
        print(Str::FL(vec[i].imag(), 3, Str::FloatRep::Fixed));
        print(" i \n");
    }
    print("\n");
};

void print_complex_vec_tail(Vec<std::complex<double>> vec, uint _count = 10)
{
    print("\n");
    for(uint i = vec.size()-_count; i < vec.size(); i++)
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

void print_vecs(Arr<Vec<double>>& arr_vec, Arr<Str>& col_names)
{

    print("\n");

	if(arr_vec.count() != col_names.count())
	{
		Print::ln("ERROR: 'arr_vec.count() != col_names.count()' in print_vecs.");
		return;
	}

	uint data_length = arr_vec[0].size();
	uint vector_count = arr_vec.count();

	// Print column names
	for(uint i = 0; i < vector_count; i++)
	{
		Print::buf(col_names[i]);

		if(i != vector_count-1)
			Print::buf(", ");
	}
	Print::buf("\n");

	// Out loop steps through all the entries of the data arrays
    for(uint i = 0; i < data_length; i++)
    {
		for(uint j = 0; j < vector_count; j++)
		{
        	Print::buf(Str::FL(arr_vec[j][i], 3, Str::FloatRep::Fixed));
			if(j != vector_count-1)
				Print::buf(", ");
		}
		Print::buf("\n");
    }
    print("\n");
};

Str print_vecs_to_str(Arr<Vec<double>>& arr_vec, Arr<Str>& col_names)
{
	Str str;

	if(arr_vec.count() != col_names.count())
	{
		Print::ln("ERROR: 'arr_vec.count() != col_names.count()' in print_vecs.");
		return Str{};
	}

	uint data_length = arr_vec[0].size();
	uint vector_count = arr_vec.count();

	// Print column names
	for(uint i = 0; i < vector_count; i++)
	{
		str += col_names[i];

		if(i != vector_count-1)
			str += ", ";
	}
	str += "\n";

	// Out loop steps through all the entries of the data arrays
    for(uint i = 0; i < data_length; i++)
    {
		for(uint j = 0; j < vector_count; j++)
		{
        	str += Str::FL(arr_vec[j][i], 3, Str::FloatRep::Fixed);
			if(j != vector_count-1)
				str += ", ";
		}
		if(i != data_length-1)
			str += "\n";
    }

	return str;
};






class DFT
{
public:
	Vec<std::complex<double>> time_data;
	Vec<std::complex<double>> frequency_data;
	Vec<double> T;
	Vec<double> F;

	uint sample_count = 0;
	f64 sample_count_f64 = 0;
	double Fs; // sampling frequency
	double dt; // time between samples

	DFT() {}

	void set_time_domain_data(Vec<std::complex<double>>& input_vec, double _sampling_frequency)
	{
		sample_count = input_vec.count();
		sample_count_f64 = (f64) sample_count;
		Fs = _sampling_frequency;
		dt = 1 / Fs;

		time_data = input_vec;
		T.set_count(input_vec.count());
		F.set_count(input_vec.count());

		// populate T and F
		for(uint i = 0; i < sample_count; i++)
		{
			double i_d = (double) i;
			double t = i_d * dt;
			T[i] = t;
			
			// The BELOW div(index, total_time) is not the confirmed correct way to get the frequencies!
			// if(i > sample_count / 2)
			// 	F[i] = (double) (sample_count - i) / Dt;
			// else
			// 	F[i] = (double) i / Dt;
			if(i > sample_count / 2)
				F[i] = (double) (sample_count - i) * Fs / sample_count_f64;
			else
				F[i] = (double) i * Fs / sample_count_f64;
		}
	}

	void calculate()
	{
		frequency_data = DFT::calculate(time_data);
	}

	Str to_csv()
	{
		Vec<double> real_vec = complex_vec_to_real_vec(frequency_data);
		Vec<double> imag_vec = complex_vec_to_imag_vec(frequency_data);

		Arr<Vec<double>> arr_vecs;

		arr_vecs.push_back(F);
		arr_vecs.push_back(real_vec);
		arr_vecs.push_back(imag_vec);

		Arr<Str> col_names;
		col_names.push_back("Freq");
		col_names.push_back("Cos/real");
		col_names.push_back("Sin/imag");

		return print_vecs_to_str(arr_vecs, col_names);
	}

	void print_frequency_data()
	{
		print_complex_vec(frequency_data);
	}
	void print_time_data()
	{
		print_complex_vec(frequency_data);
	}

    static Vec<std::complex<double>> calculate(Vec<std::complex<double>> input)
	{
		Vec<std::complex<double>> output;

		uint N = input.size();

        output.set_count(N);
        

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