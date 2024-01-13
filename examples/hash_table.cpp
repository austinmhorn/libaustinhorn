#include <austinhorn/hash_table.hpp>


struct node
{
    std::string identifier;

    node()
        : identifier("n/a")
    {
        //std::cout << "node constructor called" << std::endl;
    }
    node(std::string s)
    {
        //std::cout << "alternate node constructor called" << std::endl;
        this->identifier = s;
    }
    ~node()
    {
        //std::cout << "node destructor called" << std::endl;
    }

    node(const node &__n)
    {
        //std::cout << "node copy constructor called" << std::endl;
        this->identifier = __n.identifier;
    }

    node operator =(const node &__n)
    {
        //std::cout << "node copy assignment operator called" << std::endl;
        this->identifier = __n.identifier;
        return *this;
    }

    void operator =(std::string s)
    {
        //std::cout << "node operator =(string)" << std::endl;
        this->identifier = s;
    }

    bool operator==(const node &__n)
    {
        return this->identifier==__n.identifier;
    }
    bool operator!=(const node &__n)
    {
        return this->identifier!=__n.identifier;
    }
    bool operator< (const node &__n)
    {
        return this->identifier<__n.identifier;
    }
    bool operator> (const node &__n)
    {
        return this->identifier>__n.identifier;
    }
    bool operator<=(const node &__n)
    {
        return this->identifier<=__n.identifier;
    }
    bool operator>=(const node &__n)
    {
        return this->identifier>=__n.identifier;
    }

    friend std::ostream & operator << (std::ostream &out, const node &__n)
    {
        out.clear();
        out << __n.identifier;
        return out;
    }

    unsigned long getHash() const
    {
        std::stringstream ss;
        unsigned int n;

        std::string s = md5(identifier).substr(0,8);

        ss << std::hex << s;
        ss >> n;
        std::bitset<32> b(n);

        return b.to_ulong();
    }

};



int main (int argc, const char **argv)
{
    hash_table<node> hash;
    node n;

    for (int i = 100; i < 2000; i += 100)
    {
        n = std::to_string(i);
        hash.put(n);
    }

    hash.print();
    std::cout << "size: " << hash.size() << std::endl;
    std::cout << "bucket_count: " << hash.bucket_count() << std::endl;
    std::cout << "max_bucket_count: " << hash.max_bucket_count() << std::endl;
    hash.clear();

    return EXIT_SUCCESS;
}
