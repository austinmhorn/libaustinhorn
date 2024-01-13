//
//  file.hpp
//
//  Created by Austin Horn on 1/8/23.
//

#ifndef file_attrib_hpp
#define file_attrib_hpp

#define ARR_BOUND 256

#include <iostream>
#include <fstream>

#include <austinhorn/io.hpp>

struct ascii
{ 
    public:

        char code; 
        int  count;
        ascii* arr;

        ascii()
        {
            create();
        }

        constexpr bool operator==(const ascii& rhs) noexcept { return rhs.count==this->count; }
        constexpr bool operator!=(const ascii& rhs) noexcept { return rhs.count!=this->count; }
        constexpr bool operator< (const ascii& rhs) noexcept { return rhs.count< this->count; }
        constexpr bool operator> (const ascii& rhs) noexcept { return rhs.count> this->count; }
        constexpr bool operator<=(const ascii& rhs) noexcept { return rhs.count<=this->count; }
        constexpr bool operator>=(const ascii& rhs) noexcept { return rhs.count>=this->count; }

        void create()
        {

        }

        void inject(const std::string& filename) 
        {
            std::fstream inFS{filename, std::ios::binary};
            assert(!inFS.fail());
            for (int i=0; i<ARR_BOUND; i++)
                {arr[i].code=(char)i; arr[i].count=0;}
            while (!inFS.eof())
                {char ch=inFS.get(); arr[(int)ch].count++;}
            inFS.close();
            inFS.clear(); // resets the IO flags
        }

        void print() 
        {
            unsigned chCnt = 0;    
            for (int i=0; i<ARR_BOUND; i++) {
                if (arr[i].count) {
                    std::cout << "| " << i << "\t  ";
                    (i<32) ? std::cout << " ": std::cout << arr[i].code;
                    std::cout << "\t\t" << arr[i].count << std::endl;
                    chCnt+=arr[i].count;
                }
            }
            std::cout<<'*'<<repeat('*', 32)<<'*'<<std::endl;
            std::cout<<"| "<<chCnt<<" characters processed"<< std::endl;
            std::cout<<'*'<<repeat('*', 32)<<'*'<<std::endl;
        }

        void selection_sort() {
            for (int i = 0; i < ARR_BOUND-1; i++)
                for (int j = i+1; j < ARR_BOUND; j++)
                    if (arr[j] < arr[i])
                        std::swap(arr[i],arr[j]);
        }

        void bubble_sort() {
            for (int i = 0; i < ARR_BOUND-1; i++)
                for (int j = 0; j < ARR_BOUND-i-1; j++)
                    if (arr[j] > arr[j+1])
                        std::swap(arr[j], arr[j+1]);
        }

        void insertion_sort() {
            for (int i = 1; i < ARR_BOUND; i++) {
                ascii temp = arr[i];
                int j = i;
                while (j > 0 && arr[j-1] > temp) {
                    arr[j] = arr[j-1];
                    j--;
                }
                arr[j] = temp;
            }
        }

        void heap_sort(int n=ARR_BOUND) {
            // Build heap (rearrange array)
            for (int i = n / 2 - 1; i >= 0; i--)
                __heapify(arr, n, i);
            
            // One by one extract an element from heap
            for (int i = n - 1; i > 0; i--) {
                // Move current root to end
                std::swap(arr[0].code, arr[i].code);
                std::swap(arr[0].count, arr[i].count);

                // Call max heapify on the reduced heap
                __heapify(arr, i, 0);
            }
        }

    private:

        // Heapify a subtree rooted with node i which is an index in arr
        void __heapify(ascii* arr, int n, int i) {
            int largest = i;   // Initialize largest as root
            int l = 2 * i + 1; // left = 2 * i + 1
            int r = 2 * i + 2; // right = 2 * i + 2

            if (l < n && arr[l] > arr[largest]) ///< If left child is larger than root
                largest = l;
            if (r < n && arr[r] > arr[largest]) ///< If right child is larger than largest so far
                largest = r;
            if (largest != i) {                 ///< If largest is not root
                std::swap(arr[i].code, arr[largest].code);
                std::swap(arr[i].count, arr[largest].count);
                __heapify(arr, n, largest);     ///< Recursively heapify the affected sub-tree
            }
        }
};




/*

template <class iter>
void reverse_sort(iter first, iter last) {
    typename std::iterator_traits<iter>::difference_type n = std::distance(first, last);
    for (--n; n > 0; n -= 2) {
        typename std::iterator_traits<iter>::value_type tmp = *first;
        *first++ = *--last;
        *last = tmp;
    }
}

*/






#endif /* file_attrib_hpp */