//
//  stream.hpp
//
//  Created by Austin Horn on 1/8/23.
//
#ifndef stream_hpp
#define stream_hpp

#include <nlohmann/json.hpp>
using namespace nlohmann;

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <iterator>
#include <vector>
#include <string>

nlohmann::json get_parsed_json(const std::string& filename)
{
    std::fstream inFS;
    auto data = nlohmann::json{};

    try {
        inFS.open( filename, std::ios::in );

        if ( inFS.bad() )
            throw std::runtime_error("Bad file stream");
        else if ( !inFS.is_open() )
            throw std::runtime_error("File not found");

        inFS >> data;
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }
    
    return data;
}

class CSVRow 
{
    public:
        std::string_view operator[](std::size_t index) const
            { return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1)); }
        std::size_t size() const
            { return m_data.size() - 1; }
        std::string line() const
            { return m_line; }
    
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

class CSVIterator 
{
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


class CSVRange 
{
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

#endif /* stream_hpp */