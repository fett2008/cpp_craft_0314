//3
#include <fstream>
#include <string>
#include <map>
#include <stdexcept>
#include <reader.h>
#include <vector>
#include <writer.h>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>


class task
{
	struct data
	{
		unsigned dat,volume;
		double f1, t1, f2, f3, f4,price,vwap;
		char* date;
		char* name;
		~data()
		{
			delete[] name;
			delete[] date;

		}
		friend io::bin_reader& operator>>(io::bin_reader &in,data &obj);
		friend io::bin_writer& operator<<(io::bin_writer &out,const data &obj);
	};

	friend io::bin_reader& operator>>(io::bin_reader &in,data &obj)
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
	friend io::bin_writer& operator<<(io::bin_writer &out,const data &obj)
	{
		out.write(obj.name,9);
		out.write(obj.dat);
		out.write(obj.price);
		out.write(obj.volume);
		out.write(obj.f2);
		return out;

	}
	std::map<std::string,std::vector<data*>> my_map;
	std::map<std::string,std::vector<data*>>::iterator it;
	boost::mutex mtx;
	static const unsigned t_count=4;
	io::bin_reader in;
	static const std::string pref;
public:
	

	task()
	{
		in.open(pref+"input.txt");
		if (!in.is_open()) throw(std::logic_error("Can't open file"));

	}


	void solve()
	{
		data current_data;
		in>>current_data;
		while (!in.eof())
		{
			unsigned y,m,d;
			current_data.price=current_data.vwap;
			sscanf(current_data.date,"%4d%2d%2d",&y,&m,&d);
			current_data.dat=((y-1)*372u+(m-1)*31u+d);
			my_map[boost::lexical_cast<std::string>(current_data.name)].push_back(&current_data);
			in>>current_data;
		}
		it=my_map.begin();
		boost::thread_group t;
		for(int i=0;i<my_map.size();++i)
			t.create_thread( boost::bind(&task::process,this));
		t.join_all();

	}
	void process()
	{
		size_t size_;
		const std::vector<data*> *v;
		io::bin_writer out;
		{
			boost::mutex::scoped_lock lock(mtx);
			if (it==my_map.end()) return;
			out.open(pref+"output_"+it->first+".txt");
			if (!out.is_open())
				throw(std::logic_error("Can't open file"));
			v=&it->second;
			size_=it->second.size();
			it++;
		}
		for(unsigned i=0;i<static_cast<unsigned>(size_);++i)
			out<<*v->operator[](i);
	}
	

};

const std::string task::pref=BINARY_DIR"/";

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

