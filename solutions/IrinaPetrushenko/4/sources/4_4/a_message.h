#ifndef _TASK4_4_A_MESSAGE_H_
#define _TASK4_4_A_MESSAGE_H_

#include "message.h"
#include <boost/cstdint.hpp>

namespace task4_4
{
	class a_message : public message
	{
		static const boost::uint32_t content_size = 16; 
		char content_[content_size+1];

		explicit a_message( std::istream& inp );
	public:
		static message_ptr create_message( std::istream& inp );
		~a_message();

		const char type() const;
		const std::string str() const;
	};

}

#endif // _TASK4_4_A_MESSAGE_H_

