#pragma once

#include <iostream>
#include <vector>

template <typename It>
class IteratorRange
{
public:
    IteratorRange(It begin, It end)
        :beginIt_(begin), endIt_(end) {}

    It begin() const { return beginIt_; }
    It end() const { return endIt_; }
    auto size() const { return distance(beginIt_, endIt_); }

private:

    It beginIt_, endIt_;
};

template <typename It>
class Paginator
{
public:
    Paginator(It begin, It end, size_t pageSize)
    {
        auto it = begin;

        while (it < end)
        {
            if (it + pageSize < end)
            {
                iteratorRanges_.push_back(IteratorRange(it, it + pageSize));
                it += pageSize;
            }

            else
            {
                iteratorRanges_.push_back(IteratorRange(it, end));
                it = end;
            }
        }
    }

    auto begin() const { return iteratorRanges_.begin(); }
    auto end() const { return iteratorRanges_.end(); }
    auto size() const { return iteratorRanges_.size(); }

private:

    std::vector<IteratorRange<It>> iteratorRanges_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(c.begin(), c.end(), page_size);
}

template <typename It>
std::ostream& operator<<(std::ostream& out, const IteratorRange<It>& iteratorRange)
{
    for (auto it = iteratorRange.begin(); it < iteratorRange.end(); it++)
    {
        out << *it;
    }

    return out;
}
