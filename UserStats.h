#include <map>
#include <string>

namespace IRC
{
	class UserStats
	{
		struct Stats
		{
			unsigned int word_count;
			
			Stats() : word_count(0) { };
		};
		
		typedef std::map<std::string, Stats> user_map_type;
		
		user_map_type users;
		
	public:
		
		void increase_word_count(std::string user, unsigned int count);
		unsigned int get_word_count(std::string user);
	};
}
