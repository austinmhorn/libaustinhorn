//
//  file_attrib.hpp
//
//  Created by Austin Horn on 1/8/23.
//

#ifndef file_attrib_hpp
#define file_attrib_hpp

#define ARR_BOUND 256

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <iterator>
#include <vector>
#include <string>



struct ascii { char code; int count; };
struct ascii create_ascii(char code, int count=0) noexcept { ascii ch; ch.code=code; ch.count=count; return ch; }

constexpr bool operator==(const ascii& lhs, const ascii& rhs) noexcept { return rhs.count==lhs.count; }
constexpr bool operator!=(const ascii& lhs, const ascii& rhs) noexcept { return rhs.count!=lhs.count; }
constexpr bool operator< (const ascii& lhs, const ascii& rhs) noexcept { return rhs.count< lhs.count; }
constexpr bool operator> (const ascii& lhs, const ascii& rhs) noexcept { return rhs.count> lhs.count; }
constexpr bool operator<=(const ascii& lhs, const ascii& rhs) noexcept { return rhs.count<=lhs.count; }
constexpr bool operator>=(const ascii& lhs, const ascii& rhs) noexcept { return rhs.count>=lhs.count; }



void inject_file_data(ascii* arr, const char* filename) {
    std::ifstream fs (filename, std::ios::binary);
    assert(!fs.fail());
    for (int i=0; i<ARR_BOUND; i++)
        {arr[i].code=(char)i; arr[i].count=0;}
    while (!fs.eof())
        {char ch=fs.get(); arr[(int)ch].count++;}
    fs.close();
    fs.clear(); // resets the IO flags
}

void print_arr(ascii* arr) {
    unsigned chCnt = 0;    
    for (int i=0; i<ARR_BOUND; i++) {
        if (arr[i].count) {
            std::cout << "| " << i << "\t  ";
            (i<32) ? std::cout << " ": std::cout << arr[i].code;
            std::cout << "\t\t" << arr[i].count << std::endl;
            chCnt+=arr[i].count;
        }
    }
    std::cout<<'*'<<std::setw(32)<<std::setfill('*')<<'*'<<std::endl;
    std::cout<<"| "<<chCnt<<" characters processed"<<std::setfill(' ')<< std::endl;
    std::cout<<'*'<<std::setw(32)<<std::setfill('*')<<'*'<<std::endl;
}

void selection_sort(ascii* arr) {
    for (int i = 0; i < ARR_BOUND-1; i++)
        for (int j = i+1; j < ARR_BOUND; j++)
            if (arr[j] < arr[i])
                std::swap(arr[i],arr[j]);
}

void bubble_sort(ascii* arr) {
    for (int i = 0; i < ARR_BOUND-1; i++)
        for (int j = 0; j < ARR_BOUND-i-1; j++)
            if (arr[j] > arr[j+1])
                std::swap(arr[j], arr[j+1]);
}

void insertion_sort(ascii* arr) {
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

void heap_sort(ascii* arr, int n=ARR_BOUND) {
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

template <class iter>
void reverse_sort(iter first, iter last) {
    typename std::iterator_traits<iter>::difference_type n = std::distance(first, last);
    for (--n; n > 0; n -= 2) {
        typename std::iterator_traits<iter>::value_type tmp = *first;
        *first++ = *--last;
        *last = tmp;
    }
}





class CSVRow {
public:
        std::string_view operator[](std::size_t index) const
            { return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1)); }
        std::size_t size() const
            { return m_data.size() - 1; }
    
        void readNextRow(std::istream& str)
        {
            std::getline(str, m_line);

            m_data.clear();
            m_data.emplace_back(-1);
            std::string::size_type pos = 0;
            while((pos = m_line.find(',', pos)) != std::string::npos)
            {
                m_data.emplace_back(pos);
                ++pos;
            }
            // This checks for a trailing comma with no data after it.
            pos   = m_line.size();
            m_data.emplace_back(pos);
        }
    
private:
    
        std::string      m_line;
        std::vector<int> m_data;
};




std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}






class CSVIterator {
public:
    
    typedef std::input_iterator_tag iterator_category;
    typedef CSVRow                  value_type;
    typedef std::size_t             difference_type;
    typedef CSVRow*                 pointer;
    typedef CSVRow&                 reference;
    
    CSVIterator(std::istream& str)
        : m_str(str.good()?&str:nullptr) { ++(*this); }
    CSVIterator()
        : m_str(nullptr) {}

    CSVIterator& operator++()
        {if (m_str) { if (!((*m_str) >> m_row)){m_str = nullptr;}}return *this;}
    CSVIterator operator++(int)
        {CSVIterator    tmp(*this);++(*this);return tmp;}
    CSVRow const& operator*() const
        {return m_row;}
    CSVRow const* operator->() const
        {return &m_row;}

    bool operator==(CSVIterator const& rhs)
        {return ((this == &rhs) || ((this->m_str == nullptr) && (rhs.m_str == nullptr)));}
    bool operator!=(CSVIterator const& rhs)
        {return !((*this) == rhs);}

private:
    
    std::istream* m_str;
    CSVRow        m_row;
};







class CSVRange {
public:
    CSVRange(std::istream& str)
        : m_stream(str) {}
    CSVIterator begin() const
        {return CSVIterator{m_stream};}
    CSVIterator end()   const
        {return CSVIterator{};}
    
private:
    
    std::istream& m_stream;
};


#endif /* file_attrib_hpp */