#include "solution.h"

task4_5::solution::solution( const data_type& data )
{
	this->data=data;
	min = std::numeric_limits< int >().max();
	max= std::numeric_limits< int >().min();
}

void task4_5::solution::start() const
{
	if (data.size()==0)
	{
		min=max=0;
		return;
	}
	boost::thread_group t;
	for (int i=0;i<data.size();i++)
		t.create_thread( boost::bind(&task4_5::solution::solve,this,data[i]));
	t.join_all();
}
void task4_5::solution::solve(const std::vector<int> &tmp) const
{
	int min_ = std::numeric_limits< int >().max();
	int max_= std::numeric_limits< int >().min();
	for (int i=0;i<tmp.size();++i)
	{
		if (tmp[i]>max_)max_=tmp[i];
		if (tmp[i]<min_)min_=tmp[i];
	}
	{
		boost::mutex::scoped_lock lock( mtx );
		if(max_>max) max=max_;
		if (min_<min) min=min_;
	}
}

int task4_5::solution::get_min() const
{
	if (min == std::numeric_limits< int >().max())
		start();
	return min;
}
int task4_5::solution::get_max() const
{
	if (max== std::numeric_limits< int >().min())
		start();
	return max;
}
