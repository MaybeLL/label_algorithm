#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// void maoPaoSort(int array[]，int len)
// {
//     for (int i = 0; i < len-1; i++)
//     {
// 	for (int j = 0; j < len-1-i; j++)
//         {
// 	    if (array[j] > array[j+1])
//             {
// 		int temp = array[j+1];
// 		array[j+1] = array[j];
// 		array[j] = temp;
// 	    }
// 	}
//     }
// }
// template <typename T>
// vector<size_t> sort_indexes_e(vector<T> &v)
// {
// 	vector<size_t> idx(v.size());
// 	iota(idx.begin(), idx.end(), 0);
// 	sort(idx.begin(), idx.end(),
// 		 [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });
// 	return idx;
// }

