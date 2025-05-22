#pragma once
#include <libds/adt/sorts.h>
#include <libds/amt/implicit_sequence.h>
#include <functional>

template<typename T>
class UniversalSorter
{
public:
    void sort(ds::amt::ImplicitSequence<T>& seq, std::function<bool(const T&, const T&)> compare)
    {
        ds::adt::QuickSort<T> sorter;
        sorter.sort(seq, compare);
    }
};