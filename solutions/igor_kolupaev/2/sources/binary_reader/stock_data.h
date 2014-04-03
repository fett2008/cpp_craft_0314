#ifndef _BINARY_READER_STOCK_DATA_H_
#define _BINARY_READER_STOCK_DATA_H_

#include <fstream>

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

namespace binary_reader
{
	class stock_data: virtual protected boost::noncopyable
	{
		char stock_name_[ 8 ];
		char date_time_[ 8 ];
		double price_;
		double vwap_;
		boost::uint32_t volume_;
		double f1_;
		double t1_;
		double f2_;
		double f3_;
		double f4_;

		bool eof_;

	public:
		explicit stock_data( std::ifstream& in );

		explicit stock_data( const char* stock_name,
							 const char* date_time,
							 const double price,
							 const double vwap,
							 const boost::uint32_t volume,
							 const double f1,
							 const double t1,
							 const double f2,
							 const double f3,
							 const double f4 );
		~stock_data();

		//
		const char* stock_name() const { return stock_name_; }
		const char* date_time() const { return date_time_; }
		double price() const { return price_; }
		double vwap() const { return vwap_; }
		boost::uint32_t volume() const { return volume_; }
		double f1() const { return f1_; }
		double t1() const { return t1_; }
		double f2() const { return f2_; }
		double f3() const { return  f3_; }
		double f4() const { return f4_; }

		//
		void write( std::ofstream& out ) const;
		void write_raw( std::ofstream& out ) const;

		bool eof() const { return eof_; }
	};
}

#endif // _BINARY_READER_STOCK_DATA_H_

