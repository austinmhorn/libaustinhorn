#include <austinhorn/stream.hpp>
#include <austinhorn/io.hpp>
#include <vector>

#define HEAVY_HORIZONTAL "\u2501"
#define HEAVY_VERTICAL "\u2503"

#define HEAVY_DOWN_AND_HORIZONTAL "\u2533"
#define HEAVY_DOWN_AND_LEFT "\u2513"
#define HEAVY_DOWN_AND_RIGHT "\u250F"

#define HEAVY_VERTICAL_AND_LEFT "\u252B"
#define HEAVY_VERTICAL_AND_RIGHT "\u2523"
#define HEAVY_VERTICAL_AND_HORIZONTAL "\u254B"

#define HEAVY_UP_AND_LEFT "\u251B"
#define HEAVY_UP_AND_RIGHT "\u2517"
#define HEAVY_UP_AND_HORIZONTAL "\u253B"

struct date
{
    int month, day, year;
    date() = default;
    date(int m, int d, int y) : month(m), day(d), year(y) {}
    date(const date& other)
    {
        month = other.month;
        day = other.day;
        year = other.year;
    }
};

struct pos
{ 
    int x, y;
    pos(int _x, int _y) : x(_x), y(_y) {}
};

class Employee
{
    private:

        std::string m_firstName;
        std::string m_lastName;
        int m_id;
        date m_birthdate;
        std::string m_title;
        std::vector<std::string> m_busy;

    public:

        Employee(std::string first, std::string last, int id, const date& dob, std::string title, std::vector<std::string> busy)
        {
            m_firstName = first;
            m_lastName = last;
            m_id = id;
            m_birthdate = dob;
            m_title = title;
            m_busy = busy;
        }

        void setFirstName(const std::string& name)
        {
            m_firstName = name;
        }
        std::string getFirstName() const
        {
            return m_firstName;
        }
        void setLastName(const std::string& name)
        {
            m_lastName = name;
        }
        std::string getLastName() const
        {
            return m_lastName;
        }
        void setID(int id)
        {
            m_id = id;
        }
        int getID() const
        {
            return m_id;
        }
        void setBirthdate(const date& birthdate)
        {
            m_birthdate = birthdate;
        }
        date getBirthdate() const
        {
            return m_birthdate;
        }
        void setTitle(const std::string& title)
        {
            m_title = title;
        }
        std::string getTitle() const
        {
            return m_title;
        }
        void setBusyDays(const std::vector<std::string>& busy)
        {
            m_busy = busy;
        }
        std::vector<std::string> getBusyDays() const
        {
            return m_busy;
        }
};

void run_json_example()
{
    nlohmann::json data = get_parsed_json("examples/data/employees.json");
    auto employees = data["employees"];

    std::vector<Employee> vect;

    for (auto& emp : employees)
    {
        std::string first_name = emp["first"].get<std::string>(); // Retrieve data as a string instead of json obj
        std::string last_name = emp["last"].get<std::string>(); 
        int id = emp["id"].get<int>();
        
        auto dob_info = emp["dob"];
        date dob = {dob_info["month"].get<int>(), dob_info["day"].get<int>(), dob_info["year"].get<int>()};

        std::string title = emp["title"].get<std::string>(); 

        std::vector<std::string> busy;
        auto busy_info = emp["busy"];
        const int SIZE = busy_info.size();

        for (int i = 0; i < SIZE; i++)
            busy.push_back( busy_info.at(i) );
        
        Employee e = Employee(first_name, last_name, id, dob, title, busy);
        vect.push_back(e);
    }

    for (auto& emp : vect)
    {
        std::cout << "********************************************" << std::endl;
        std::cout << "First:      " << emp.getFirstName() << std::endl;
        std::cout << "Last:       " << emp.getLastName() << std::endl;
        std::cout << "ID:         " << emp.getID() << std::endl;
        std::cout << "Birthdate:  " << emp.getBirthdate().month << '/' << emp.getBirthdate().day << '/' << emp.getBirthdate().year << std::endl;
        std::cout << "Title:      " << emp.getTitle() << std::endl;
        std::cout << "Busy:       ";
        for (auto& day : emp.getBusyDays())
        {
            std::cout << day;
            (day != emp.getBusyDays().at(emp.getBusyDays().size()-1))
                ? std::cout << ", " : std::cout << std::endl;
        }

        std::cout.flush();
    }
    std::cout << "********************************************" << std::endl;
}



void run_csv_example(std::string filename)
{
    std::fstream inFS;

    try {
        std::cout << "Initiating parse of .csv file " << '\'' << filename << '\'' << std::endl;
        inFS.open( filename, std::ios::in );

        if ( inFS.bad() )
            throw std::runtime_error("Bad file stream");
        else if ( !inFS.is_open() )
            throw std::runtime_error("File not found");
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return;
    }

    int rows = 0;
    int cols = 0;
    char arr[100][100];

    for (auto& row : CSVRange(inFS))
    {
        if ( !rows )
        {
            cols = row.line().size();
            std::cout << HEAVY_DOWN_AND_RIGHT << repeat(HEAVY_HORIZONTAL, cols) << HEAVY_DOWN_AND_LEFT << std::endl;
        }

        int i = rows;
        int j = 0;

        for (auto& ch : row.line())
        {
            if ( !j )
                std::cout << HEAVY_VERTICAL;
            std::cout << ch;
            arr[i][j] = ch;
            j++;
        }
        
        std::cout << HEAVY_VERTICAL << std::endl;
        rows++;
    }

    std::cout << HEAVY_UP_AND_RIGHT << repeat(HEAVY_HORIZONTAL, cols) << HEAVY_UP_AND_LEFT << std::endl;

    inFS.close();
}



template <typename _Tp>
class Grid
{
    public:
    
        Grid(int rows, int cols)
        {
            __rows_ = rows;
            __cols_ = cols;
            __longest_ = 0;

            __vect_.clear();
            __vect_.resize(__rows_);
            
            for (int row = 0; row < __rows_; row++)
                __vect_[row].resize(__cols_);

            create();
        }

        ~Grid()
        {
            __vect_.clear();
        }

        _Tp& operator[](const pos& position)
        {
            return __vect_[position.x][position.y];
        }
        
        template <typename type_t = _Tp,
            typename std::enable_if<std::is_same<type_t, int>::value, int>::type = 0> /// _Tp = int
        void create()
        { 
            for (int row = 0; row < __rows_; row++)
                for (int col = 0; col < __cols_; col++)
                    __vect_[row][col] = 0;
        }
        template <typename type_t = _Tp,
            typename std::enable_if<std::is_same<type_t, float>::value, int>::type = 0> /// _Tp = float
        void create()
        {
            for (int row = 0; row < __rows_; row++)
                for (int col = 0; col < __cols_; col++)
                    __vect_[row][col] = 0;
        }
        template <typename type_t = _Tp,
            typename std::enable_if<std::is_same<type_t, double>::value, int>::type = 0> /// _Tp = double
        void create()
        {
            for (int row = 0; row < __rows_; row++)
                for (int col = 0; col < __cols_; col++)
                    __vect_[row][col] = 0;
        }
        template <typename type_t = _Tp,
            typename std::enable_if<std::is_same<type_t, char>::value, int>::type = 0> /// _Tp = char
        void create()
        {
            for (int row = 0; row < __rows_; row++)
                for (int col = 0; col < __cols_; col++)
                    __vect_[row][col] = ' ';
        }
        template <typename type_t = _Tp,
            typename std::enable_if<std::is_same<type_t, bool>::value, int>::type = 0> /// _Tp = bool
        void create()
        {
            for (int row = 0; row < __rows_; row++)
                for (int col = 0; col < __cols_; col++)
                    __vect_[row][col] = false;
        }

        void print()
        {
            for (int i = 0; i < __rows_; i++)
            {
                if ( !i ) // Print top border
                {
                    std::cout << HEAVY_DOWN_AND_RIGHT;
                    for (int col = 0; col < __cols_-1; col++)
                        std::cout << HEAVY_HORIZONTAL << HEAVY_DOWN_AND_HORIZONTAL;
                    std::cout << HEAVY_HORIZONTAL << HEAVY_DOWN_AND_LEFT << std::endl;
                }
                
                for (int j = 0; j < __cols_; j++) // Print array value between vertical bars
                    std::cout << HEAVY_VERTICAL << __vect_[i][j];
                std::cout << HEAVY_VERTICAL << std::endl;
                
                if ( i+1 != __rows_ ) // Executes when current row is NOT the final row
                {
                    std::cout << HEAVY_VERTICAL_AND_RIGHT;
                    for (int col = 0; col < __cols_-1; col++)
                        std::cout << HEAVY_HORIZONTAL << HEAVY_VERTICAL_AND_HORIZONTAL;
                    std::cout << HEAVY_HORIZONTAL << HEAVY_VERTICAL_AND_LEFT << std::endl;
                }
                else // This is the final row - print bottom border
                {
                    std::cout << HEAVY_UP_AND_RIGHT;
                    for (int col = 0; col < __cols_-1; col++)
                        std::cout << HEAVY_HORIZONTAL << HEAVY_UP_AND_HORIZONTAL;
                    std::cout << HEAVY_HORIZONTAL << HEAVY_UP_AND_LEFT << std::endl;
                }
            }
        }

    private:

        int  __rows_;
        int  __cols_;
        std::vector<std::vector<_Tp>> __vect_;
        int __longest_;
};

void run_grid_example()
{
    Grid<char> grid = Grid<char>(4, 4);

    grid.print();

    std::cout << "Injecting \'A\' at (0,1)" << std::endl;
    grid[pos(0,1)] = 'A';
    std::cout << "Injecting \'B\' at (1,3)" << std::endl;
    grid[pos(1,3)] = 'B';
    std::cout << "Injecting \'C\' at (3,0)" << std::endl;
    grid[pos(3,0)] = 'C';
    std::cout << "Injecting \'D\' at (2,2)" << std::endl;
    grid[pos(2,2)] = 'D';

    grid.print();
}

int main (int argc, const char **argv)
{
    run_json_example();

    run_csv_example("examples/data/zone1.csv");
    run_csv_example("examples/data/zone2.csv");
    
    run_grid_example();

    return EXIT_SUCCESS;
}
