//2
#include <fstream>
#include <string>
#include <stdexcept>
#include <map>
#include <set>
#include <reader.h>
#include <writer.h>
#include<iostream>
#include <boost/thread.hpp>
#include <boost/ref.hpp>
#include <boost/cstdint.hpp>

typedef  std::map<boost::uint32_t,int> mmap;

class task
{
	int file_number;
	boost::mutex mtx;
	static const unsigned t_count=1000;
	static const unsigned f_count=1000;
	static const std::string pref;
	static const size_t max_memory=2048ul;
	struct data
	{
		boost::uint32_t type,time,length;
		char *msg;
		friend io::bin_reader& operator>>(io::bin_reader &in,data &obj);
		size_t get_size()
		{
			return sizeof(boost::uint32_t)*3+length;
		}
		data():msg(NULL){}
		~data()
		{
			delete []msg;
		}
	};

	friend io::bin_reader& operator>>(io::bin_reader &in,data &obj)
		{
			in.read(obj.type);
			in.read(obj.time);
			in.read(obj.length);
			if ( obj.length )
			{
				obj.msg=new char[obj.length+1];
				in.read(obj.msg,obj.length);
			}
			return in;
		}
	void solve()
	{
		io::bin_reader in;
		io::bin_writer out;
		{
			boost::mutex::scoped_lock lock(mtx);
			char c[4];
			if (file_number==f_count) return;
			while(!in.is_open() && file_number<f_count)
			{
				sprintf(c,"%03d",file_number);
				in.open(pref+"input_"+c+".txt");
				out.open(pref+"output_"+c+".txt");
				file_number++;
			}
			if (!in.is_open())
				throw(std::logic_error("Can't open file"));
		}
		mmap met,times;
		std::set<boost::uint32_t> last;
		std::map<boost::uint32_t,size_t> size;
		data current_data;
		in>>current_data;
		boost::uint32_t curr_time=0;
		while (!in.eof())
		{
			if ( current_data.time != curr_time || last.find(current_data.type) == last.end() )
			{
				if (current_data.time!=curr_time)
				last.clear();
				if (current_data.get_size()<=max_memory)
				{
					size[current_data.type]=current_data.get_size();
					if (last.find(current_data.type)==last.end())
					{
						//boost::mutex::scoped_lock lock(mtx);
						times[current_data.type]++;
					}
					last.insert(current_data.type);
					{
						//boost::mutex::scoped_lock lock(mtx);
						met[current_data.type]++;
					}
				}
				curr_time=current_data.time;
				in>>current_data;
				continue;
			}
			if (current_data.get_size()+size[current_data.type]<=max_memory)
			{
				{
					//boost::mutex::scoped_lock lock(mtx);
					met[current_data.type]++;
				}
				size[current_data.type]+=current_data.get_size();
			}			
			curr_time=current_data.time;
			in>>current_data;
		}
		for (mmap::iterator it=met.begin();it!=met.end();it++)
			{
				boost::uint32_t i=it->first;
				out.write(i);
				const double temp=double(it->second)/times[it->first];
				out.write(temp);
			}
	}

public:
	task()
	{}

	void start()
	{
		file_number=1;
		boost::thread_group t;
		for(int i=0;i<t_count;++i)
			t.create_thread( boost::bind(&task::solve,this));
		t.join_all();
	}

};

const std::string task::pref=BINARY_DIR"/";
	
void main()
{
	try
	{	
		task realization;
		realization.start();
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

