/*
  Filename   : Sorts.cc
  Author     : Joe Malone, Jerek Martinez-Andujar
  Course     : CSCI 362-01
  Assignment : Program 8
  Description: This program records the number of key comparisons and the run
			   time of mergeSort, quickSort, and shellSort. 


N:	 20,000,000		    40,000,000			80,000,000
=========================================================================
Merge	 1,806.70 (460,190,113)	    3,699.26 (960,377,833)	7,555.75 (2,000,768,116)
Quick	 1,054.88 (353,443,351)	    2,140.11 (685,074,636)	4,299.57 (1,425,273,732)
Shell	 2,638.87 (1,584,496,854)   5,473.93 (3,348,578,741)    12,151.23 (8,140,974,894)
std	 965.09	                    1,956.67		        3,935.26

The complexity (worst case) of Merge sort is O(n*lg(n)). The complexity (worst case) of quick sort is 
O(n^2). In most cases quicksort is faster than merge sort. However, it is possible to get burned (data 
already sorted / reverse sorted). According to our data, quicksort is the fastest and most efficient search.
The worst search is Shell Sort, clocking in at complexity (worst case) O(N^3/2). Our data clearly shows
that this is the case.
*/

// ======================================================================
// System includes
#include<iostream>
#include<iomanip>
#include<vector>
#include<cmath>
#include<iostream>
#include<random>
#include<algorithm>

// ======================================================================
// Local includes
#include "Timer.hpp"


// using
using std::vector;
using std::endl;
using std::cout;
using std::cin;
using std::mt19937;
using std::random_device;
using std::swap;
using std::sort;
using std::fixed;
using std::setprecision;
using std::boolalpha;
using std::move;


// ======================================================================
// prototypes

// Additional functions---------------------------------
void
fill (vector<int>& v);

size_t
quickSort (vector<int>& v, size_t first, size_t last);

int
median3 (vector<int>& v, size_t left, size_t right);

size_t
mergeSort (vector<int>& v, vector<int>& b, size_t left, size_t right);

size_t
merge (vector<int>& v, vector<int>& b, size_t leftPos, size_t rightPos, size_t rightEnd);

// Required functions---------------------------------
size_t
mergeSort (vector<int>& v);

size_t
quickSort (vector<int>& v);

size_t
insertionSort (vector<int>& v, size_t begin, size_t end);

size_t
shellSort (vector<int>& v);

// ======================================================================
int
main (int argc, char* argv[])
{
   cout << "N ==> ";
   size_t N;
   cin >> N;
   cout << endl;

   // declare the initial vector
   vector<int> mergeVec(N);

   // fill the initial vector
   fill(mergeVec);

   // create the other vectors using
   // the populated one.
   vector<int> quickVec(mergeVec);
   vector<int> shellVec(mergeVec);
   vector<int> stdVec(mergeVec);

   // STD SORT---------------------------------
   Timer<> t;
   sort(stdVec.begin(), stdVec.end());
   t.stop();

   double stdTime = t.getElapsedMs();
	
   // MERGE SORT-------------------------------
   t.start();
   size_t mergeCompares = mergeSort(mergeVec);
   t.stop();

   double mergeTime = t.getElapsedMs();

   // QUICKSORT--------------------------------
   t.start();
   size_t quickCompares = quickSort(quickVec);
   t.stop();

   double quickTime = t.getElapsedMs();

   // SHELLSORT--------------------------------
   t.start();
   size_t shellCompares = shellSort(shellVec);
   t.stop();
	
   double shellTime = t.getElapsedMs();

   // output the times and compares
   cout << fixed << setprecision(2);

   cout << "Merge time: " << mergeTime << " ms" << endl;
   cout << "Merge compares: " << mergeCompares << endl;

   cout << endl;

   cout << "Quick time: " << quickTime << " ms" << endl;
   cout << "Quick compares: " << quickCompares << endl;

   cout << endl;

   cout << "Shell time: " << shellTime << " ms" << endl;
   cout << "Shell compares: " << shellCompares << endl;

   cout << endl;

   cout << "\nStd time: " << stdTime << endl;

   // output the sort checks
   cout << endl;
	
   cout << "Merge ok? " << boolalpha << (mergeVec == stdVec) << endl;
   cout << "Quick ok? "  << (quickVec == stdVec) << endl;
   cout << "Shell ok? " << (shellVec == stdVec) << endl;

   return 0;
}

// ======================================================================
// Populate a vector with random numbers from 0 to 9999 inclusive
void
fill (vector<int>& v)
{
   random_device rand;
   mt19937 gen(rand());
	
   for (size_t x = 0; x < v.size(); ++x)
   {
      v[x] = gen() % 10'000;
   }
}

// Calculates the median of three for quickSort
int
median3 (vector<int>& v, size_t left, size_t right)
{
   int center = (left + right) / 2;

   if (v[center] < v[left])
   {
      swap(v[left], v[center]);
   }

   if (v[right] < v[left])
   {
      swap(v[left], v[right]);
   }

   if (v[right] < v[center])
   {
      swap(v[center], v[right]);
   }

   swap(v[center], v[right - 1]);

   return v[right - 1];
}

// Recursive quickSort
size_t
quickSort (vector<int>& v, size_t first, size_t last)
{
   size_t compares = 0;

   if (first + 20 <= last)
   {
      int pivot = median3(v, first, last);

      int i = first, j = last - 1;

      for ( ; ;)
      {
         while (v[++i] < pivot) { ++compares; };
    	 while (pivot < v[--j]) { ++compares; };

	 ++compares;
	 if (i < j)
	 {
       	    swap(v[i], v[j]);
	 }
	 else
	 {
	    break;
	 }
       }

       swap(v[i], v[last - 1]);

       compares += quickSort(v, first, i - 1);
       compares += quickSort(v, i + 1, last);

    }
    else
    {
       compares += insertionSort(v, first, last);
    }
   
   return compares;
}

// quickSort driver
size_t
quickSort (vector<int>& v)
{
   size_t compares = quickSort(v, 0, v.size() - 1);
   return compares; 
}

size_t
insertionSort (vector<int>& v, size_t begin, size_t end)
{
   size_t compares = 0;

   for (size_t i = begin; i <= end; ++i)
   {
      int  h = v[i];
      size_t  j = i;

      while (j >= 1 && h < v[j - 1])
      {
         ++compares;
	 v[j] = v[j - 1];
	 --j;
      }

      if (j != 0 && h >= v[j - 1])
      {
         ++compares;
      }

      v[j] = h;
   }
   
   return compares;
}

// mergeSort driver
size_t
mergeSort (vector<int>& v)
{
   vector<int> b(v.size());
   size_t compares = mergeSort(v, b, 0, v.size() - 1);
   return compares;
}

// recursive mergeSort
size_t
mergeSort (vector<int>& v, vector<int>& b, size_t left, size_t right)
{
   size_t compares = 0;

   if (left < right)
   {
      int center = (left + right) / 2;
      compares += mergeSort(v, b, left, center);
      compares += mergeSort(v, b, center + 1, right);
      compares += merge(v, b, left, center + 1, right);
   }

   return compares;
}

// required for the mergeSort function.
size_t
merge (vector<int>& v, vector<int>& b, size_t leftPos, size_t rightPos, size_t rightEnd)
{
   size_t compares = 0;

   size_t leftEnd = rightPos - 1;
   size_t tmpPos = leftPos;
   size_t numElements = rightEnd - leftPos + 1;

   while (leftPos <= leftEnd && rightPos <= rightEnd)
   {	
      ++compares;
      if (v[leftPos] <= v[rightPos])
      {
         b[tmpPos++] = move(v[leftPos++]);
      }
      else
      {
         b[tmpPos++] = move(v[rightPos++]);
      }
   }

   while (leftPos <= leftEnd)
   {
      b[tmpPos++] = move(v[leftPos++]);
   }

   while (rightPos <= rightEnd)
   {
      b[tmpPos++] = move(v[rightPos++]);
   }

   for (size_t i = 0; i < numElements; ++i, --rightEnd)
   {
      v[rightEnd] = move(b[rightEnd]);
   }

   return compares;
}

size_t
shellSort (vector<int>& v)
{
   size_t counter = 0;
   size_t h;

   for (h = 1; h <= v.size() / 6; h = 3 * h + 1);

   while (h > 0)
   {
      for (size_t i = h; i < v.size(); ++i)
      {
         int val = v[i];
	 size_t j = i;

	 while (j >= h && val < v[j - h])
	 {
	    ++counter;
	    v[j] = v[j - h];
	    j -= h;
	 }

	 v[j] = val;
      }

      h /= 3;
   }
   return counter;
}
