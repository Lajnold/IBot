#ifndef IRC_UTILS_H
#define IRC_UTILS_H

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace IRC
{
namespace utils
{

	// to_lower

	template <typename StringT>
	StringT to_lower(const StringT& s)
	{
		StringT copy = s;
		boost::algorithm::to_lower(copy);
		return copy;
	}

	
	// split_whitespace

	template <typename ContT, typename StringT>
	void split_whitespace(ContT &out, const StringT& input)
	{
		boost::split(out, input, boost::algorithm::is_space(), boost::token_compress_on);
	}

	template <typename ContT, typename StringT>
	ContT split_whitespace(const StringT& input)
	{
		ContT res;
		split_whitespace(res, input);
		return res;
	}


	// find_nth

	template <typename IterT, typename ValT>
	IterT find_nth(IterT first, IterT last, const ValT& val, int n)
	{
		assert(n > 0);

		int count = 0;
		IterT search_it = first;
		while (count < n && search_it != last)
		{
			if (count > 0)
			{
				// Start search after last match.
				std::advance(search_it, 1);
			}

			search_it = std::find(search_it, last, val);
			if (search_it != last)
			{
				++count;
			}
		}
		return search_it;
	}

	template <typename SearchT, typename ValT>
	typename SearchT::iterator find_nth(SearchT& cont, const ValT& val, int n)
	{
		return find_nth(std::begin(cont), std::end(cont), val, n);
	}

	template <typename SearchT, typename ValT>
	typename SearchT::const_iterator find_nth(const SearchT& cont, const ValT& val, int n)
	{
		return find_nth(std::begin(cont), std::end(cont), val, n);
	}


	// contains

	template <typename IterT, typename ValT>
	bool contains(IterT first, IterT last, const ValT& val)
	{
		return std::find(first, last, val) != last;
	}

	template <typename ContT, typename ValT>
	bool contains(const ContT& cont, const ValT& val)
	{
		return contains(std::begin(cont), std::end(cont), val);
	}


	// remove_val
	// remove_pred

	template <typename ContT, typename ValT>
	void remove_val(ContT& cont, const ValT& val)
	{
		cont.erase(std::remove(std::begin(cont), std::end(cont), val));
	}

	template <typename ContT, typename PredT>
	void remove_pred(ContT& cont, PredT pred)
	{
		cont.erase(std::remove_if(std::begin(cont), std::end(cont), pred));
	}

} // namespace utils
} // namespace IRC

#endif
