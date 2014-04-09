#ifndef _TASK4_5_SOLUTION_H_
#define _TASK4_5_SOLUTION_H_

#include <vector>
#include <boost/thread.hpp>
#include <limits>

namespace task4_5
{
	typedef std::vector< std::vector< int > > data_type;

	class solution
	{
		mutable boost::mutex  mtx;
		data_type data;
		mutable int max,min;
		void start() const;
		void solve(const std::vector<int> &tmp)const ;
	public:
		explicit solution( const data_type& data );
		int get_min() const;
		int get_max() const;
	};
}

#endif // _TASK4_5_SOLUTION_H_

