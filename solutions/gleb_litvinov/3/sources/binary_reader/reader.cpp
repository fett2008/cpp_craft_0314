#include "reader.h"
#include <string>

namespace io
{
	bin_reader::bin_reader()
	{}

	void bin_reader::open(const std::string& name)
	{
		in.open(name,std::ios_base::binary);
	}

	bin_reader::~bin_reader()
	{
		if (in.is_open()) in.close();
	}

	bool bin_reader::is_open()const
	{
		return in.is_open();
	}

	void bin_reader::read(char* const a,const size_t length)
	{
		in.read( a, length );
	}

	bool bin_reader::eof() const
	{
		return !in;
	}
}