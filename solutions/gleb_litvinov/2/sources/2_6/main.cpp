//3
#include <fstream>
#include <string>
#include <stdexcept>
#include <reader.h>
#include <writer.h>
#include <string>
#include <iostream>

struct data
	{
		unsigned dat,volume;
		double f1, t1, f2, f3, f4,price,vwap;
		char* date;
		char* name;
		friend io::bin_reader& operator>>(io::bin_reader &in,data &obj);
		friend io::bin_writer& operator<<(io::bin_writer &out,const data &obj);
		~data()
		{
			delete[] date;
			delete[] name;
		}
	};

	io::bin_reader& operator>>(io::bin_reader &in,data &obj)
	{
		obj.name=new char[9];
		in.read(obj.name,8);
		obj.date=new char[9];
		in.read(obj.date,8);
		in.read(obj.price);
		in.read(obj.vwap);
		in.read(obj.volume);
		in.read(obj.f1);
		in.read(obj.t1);
		in.read(obj.f2);
		in.read(obj.f3);
		in.read(obj.f4);
		return in;

	}
	io::bin_writer& operator<<(io::bin_writer &out,const data &obj)
	{
		out.write(obj.name,9);
		out.write(obj.dat);
		out.write(obj.price);
		out.write(obj.volume);
		out.write(obj.f2);
		return out;

	}
class task
{
	data current_data;
	io::bin_reader in;
	io::bin_writer out;
public:
	

	task()
		: in(BINARY_DIR"/input.txt" ),out(BINARY_DIR"/output.txt")
	{
		if (!in.is_open()) throw(std::logic_error("Can't open file"));
		if (!out.is_open()) throw(std::logic_error("Can't open file "));
		
	}


	void solve()
	{
		in>>current_data;
		while (!in.eof())
		{
			unsigned y,m,d;
			current_data.price=current_data.vwap;
			sscanf(current_data.date,"%4d%2d%2d",&y,&m,&d);
			current_data.dat=((y-1)*372u+(m-1)*31u+d);
			out<<current_data;
			in>>current_data;
		}
	}
	

};



void main()
{
	try
	{
		task realization;
		realization.solve();
	}
	catch(const std::logic_error& message)
	{
		std::cout<<message.what()<<"\n";
	}
	catch(const std::exception& e)
	{
		std::cout<<e.what();
	}
	catch( ... )
	{
		std::cout<<"Unknown error";
	}
	

}

