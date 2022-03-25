#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument {
public:
    InvalidDateException() : invalid_argument("invalid date or format") { }
};

// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
/*ios_base & (*date_format(const char * fmt)) (ios_base & x) {
    return [ ](ios_base & ios) -> ios_base & { return ios; };
}*/

const int date_index = ios::xalloc();

const string defaultFormat = "%Y-%m-%d";

struct date_format {
    string storedFormat;

    date_format(const char * s) : storedFormat(s) { }

    static void caughtEvent(ios::event event, ios_base & stream, int index) {
        if (stream.iword(index) == 1) {
            if (event == ios_base::erase_event) {
                delete (string *)(stream.pword(index));
                stream.pword(index) = nullptr;
                stream.iword(index) = 0;
            } else if (event == ios_base::copyfmt_event) {
                string * newString = new string(*(static_cast<string *>(stream.pword(index))));
                stream.pword(index) = newString;
            }
        }
    }

    static void addFormatToStream(ios_base & stream, const date_format & format) {
        if (stream.iword(date_index) == 1) {
            delete static_cast<string *>(stream.pword(date_index));
            stream.pword(date_index) = nullptr;
        }
        stream.iword(date_index) = 1;
        stream.pword(date_index) = static_cast<void *>(new string(format.storedFormat));
        stream.register_callback(caughtEvent, date_index);
    }

    friend ostream & operator << (ostream & os, const date_format & rhs) {
        addFormatToStream(os, rhs);
        return os;
    }

    friend istream & operator >> (istream & is, const date_format & rhs) {
        addFormatToStream(is, rhs);
        return is;
    }
};

class CDate {
private:
    unsigned int days; // Days sice lower bound (01/01/2000) to the date this instance is representing

    class DateConvertor {
    private:
        unsigned int year;
        unsigned int month;
        unsigned int day;

    public:
        static unsigned int leapYearsUntil(unsigned int y) {
            return (y / 4) - (y / 100) + (y / 400) - (y / 4000) + 1;
        }

        static bool leapYear(unsigned int y) {
            return ((y == 0) || (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0 && y % 4000 != 0));
        }

        static unsigned int daysInMonth(unsigned int m, unsigned int y) {
            unsigned int inMonth[12] = { 31, leapYear(y) ? (unsigned)(29) : (unsigned)(28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
            return inMonth[m - 1];
        }

        void convertFromDays(unsigned int days) {

            year = days / 365;

            day = (days - (365 * year));
            if (year > 0) {
                if (days % 365 < leapYearsUntil(year - 1)) {
                    year--;
                    day += 365;
                }
                if (year > 0) {
                    day -= leapYearsUntil(year - 1);
                }
            }

            month = 1;
            while (day >= daysInMonth(month, year)) {
                day -= daysInMonth(month, year);
                month++;
            }
        }

        unsigned int getYear() const {
            return year + 2000;
        }

        unsigned int getMonth() const {
            return month;
        }

        unsigned int getDay() const {
            return day + 1;
        }
    };

    static ostream & formatOutput(ostream & stream, const string & format, const CDate & date) {
        DateConvertor convertor;
        convertor.convertFromDays(date.days);

        bool modifierOnInput = false;;
        for (auto iter = format.begin(); iter != format.end(); iter++) {
            char c = *iter;
            if (c == '%' && !modifierOnInput) {
                modifierOnInput = true;
                continue;
            } else {
                if (modifierOnInput) {
                    modifierOnInput = false;
                    if (c == 'd') {
                        stream << setw(2) << setfill('0') << convertor.getDay();
                        continue;
                    } else if (c == 'm') {
                        stream << setw(2) << setfill('0') << convertor.getMonth();
                        continue;
                    } else if (c == 'Y') {
                        stream << convertor.getYear();
                        continue;
                    }
                }
                stream << c;
            }
        }
        return stream;
    }

    static bool processCharsToInt(int & to, char * chars, int size) {
        to = 0;
        int multiplier = 1;
        for (int j = 1; j < size; j++) {
            multiplier *= 10;
        }
        for (int i = 0; i < size; i++) {
            if (chars[i] < '0' || chars[i] > '9') {
                return false;
            }

            to += multiplier * (chars[i] - '0');
            multiplier /= 10;
        }
        return true;
    }

    static bool extractCharsFromStream(istream & stream, char * to, int count) {
        for (int j = 0; j < count; j++) {
            to[j] = stream.get();
            if (stream.fail()) {
                stream.setstate(ios::failbit);
                return false;
            }
        }
        return true;
    }

    static istream & formatInput(istream & stream, const string & format, CDate & date) {
        int d, m, y;
        bool dIn = false, mIn = false, yIn = false;
        bool modifierOnInput = false;

        for (auto iter = format.begin(); iter != format.end(); iter++) {
            char forChar = *iter;
            if (forChar == '%' && !modifierOnInput) {
                modifierOnInput = true;
                continue;
            } else {
                if (modifierOnInput) {
                    modifierOnInput = false;
                    if (forChar == 'd') {
                        char day[2];
                        if (dIn || !extractCharsFromStream(stream, day, 2) || !processCharsToInt(d, day, 2)) {
                            stream.setstate(ios::failbit);
                            return stream;
                        }
                        dIn = true;
                        continue;
                    } else if (forChar == 'm') {
                        char month[2];
                        if (mIn || !extractCharsFromStream(stream, month, 2) || !processCharsToInt(m, month, 2)) {
                            stream.setstate(ios::failbit);
                            return stream;
                        }
                        mIn = true;
                        continue;
                    } else if (forChar == 'Y') {
                        char year[4];
                        if (yIn || !extractCharsFromStream(stream, year, 4) || !processCharsToInt(y, year, 4)) {
                            stream.setstate(ios::failbit);
                            return stream;
                        }
                        yIn = true;
                        continue;
                    }
                }

                char inChar = stream.get();
                if (inChar != forChar || stream.fail()) {
                    stream.setstate(ios::failbit);
                    return stream;
                }
            }
        }

        if (!dIn || !mIn || !yIn) {
            stream.setstate(ios::failbit);
            return stream;
        }

        try {
            CDate newDate(y, m, d);
            date = newDate;
        }
        catch (InvalidDateException & e) {
            stream.setstate(ios::failbit);
        }
        return stream;
    }

public:
    CDate(unsigned int y, unsigned int m, unsigned int d) {
        if (y < 2000 ||
            m > 12 || m == 0 ||
            d > DateConvertor::daysInMonth(m, y) || d == 0) {
            throw InvalidDateException();
        }

        y -= 2000;
        days = y * 365;
        if (y > 0) {
            days += DateConvertor::leapYearsUntil(y - 1);
        }
        for (unsigned int i = 1; i < m; i++) {
            days += DateConvertor::daysInMonth(i, y);
        }
        days += (d - 1);
    }

    CDate(unsigned int d) : days(d) { }

    CDate & operator += (unsigned int d) {
        days += d;
        return *this;
    }

    friend CDate operator + (CDate lhs, unsigned int d) {
        lhs += d;
        return lhs;
    }

    friend CDate operator + (unsigned int d, CDate rhs) {
        rhs += d;
        return rhs;
    }

    CDate & operator -= (unsigned int d) {
        days -= d;
        return *this;
    }

    friend CDate operator - (CDate lhs, unsigned int d) {
        lhs -= d;
        return lhs;
    }

    friend CDate operator - (unsigned int d, CDate rhs) {
        rhs -= d;
        return rhs;
    }

    friend int operator - (const CDate & lhs, const CDate & rhs) {
        return (int)(lhs.days) - (int)(rhs.days);
    }

    CDate & operator ++ () {
        *this += 1;
        return *this;
    }

    CDate operator ++ (int) {
        CDate old = *this;
        ++(*this);
        return old;
    }

    CDate & operator -- () {
        *this -= 1;
        return *this;
    }

    CDate operator -- (int) {
        CDate old = *this;
        --(*this);
        return old;
    }

    friend bool operator < (const CDate & lhs, const CDate & rhs) {
        return lhs.days < rhs.days;
    }

    friend bool operator > (const CDate & lhs, const CDate & rhs) {
        return rhs < lhs;
    }

    friend bool operator <= (const CDate & lhs, const CDate & rhs) {
        return !(lhs > rhs);
    }

    friend bool operator >= (const CDate & lhs, const CDate & rhs) {
        return !(lhs < rhs);
    }

    friend bool operator == (const CDate & lhs, const CDate & rhs) {
        return lhs.days == rhs.days;
    }

    friend bool operator != (const CDate & lhs, const CDate & rhs) {
        return !(lhs == rhs);
    }

    friend ostream & operator << (ostream & os, const CDate & rhs) {
        if (os.iword(date_index) == 1) {
            string formatString = *(static_cast<string *>(os.pword(date_index)));
            return formatOutput(os, formatString, rhs);
        }
        return formatOutput(os, defaultFormat, rhs);
    }

    friend istream & operator >> (istream & is, CDate & rhs) {
        if (is.iword(date_index) == 1) {
            string formatString = *(static_cast<string *>(is.pword(date_index)));
            return formatInput(is, formatString, rhs);
        }
        return formatInput(is, defaultFormat, rhs);
    }

};

#ifndef __PROGTEST__
int main(void) {

    ostringstream oss;
    istringstream iss;
    CDate a(2000, 1, 2);
    CDate b(2010, 2, 3);
    CDate c(2004, 2, 10);
    oss.str("");
    oss << a;
    assert(oss.str() == "2000-01-02");
    oss.str("");
    oss << b;
    assert(oss.str() == "2010-02-03");
    oss.str("");
    oss << c;
    assert(oss.str() == "2004-02-10");
    a = a + 1500;
    oss.str("");
    oss << a;
    assert(oss.str() == "2004-02-10");
    b = b - 2000;
    oss.str("");
    oss << b;
    assert(oss.str() == "2004-08-13");
    assert(b - a == 185);
    assert((b == a) == false);
    assert((b != a) == true);
    assert((b <= a) == false);
    assert((b < a) == false);
    assert((b >= a) == true);
    assert((b > a) == true);
    assert((c == a) == true);
    assert((c != a) == false);
    assert((c <= a) == true);
    assert((c < a) == false);
    assert((c >= a) == true);
    assert((c > a) == false);
    a = ++c;
    oss.str("");
    oss << a << " " << c;
    assert(oss.str() == "2004-02-11 2004-02-11");
    a = --c;
    oss.str("");
    oss << a << " " << c;
    assert(oss.str() == "2004-02-10 2004-02-10");
    a = c++;
    oss.str("");
    oss << a << " " << c;
    assert(oss.str() == "2004-02-10 2004-02-11");
    a = c--;
    oss.str("");
    oss << a << " " << c;
    assert(oss.str() == "2004-02-11 2004-02-10");
    iss.clear();
    iss.str("2015-09-03");
    assert((iss >> a));
    oss.str("");
    oss << a;
    assert(oss.str() == "2015-09-03");
    a = a + 70;
    oss.str("");
    oss << a;
    assert(oss.str() == "2015-11-12");

    CDate d(2000, 1, 1);
    try {
        CDate e(2000, 32, 1);
        assert("No exception thrown!" == nullptr);
    }
    catch (...) { }
    iss.clear();
    iss.str("2000-12-33");
    assert(!(iss >> d));
    oss.str("");
    oss << d;
    assert(oss.str() == "2000-01-01");
    iss.clear();
    iss.str("2000-11-31");
    assert(!(iss >> d));
    oss.str("");
    oss << d;
    assert(oss.str() == "2000-01-01");
    iss.clear();
    iss.str("2000-02-29");
    assert((iss >> d));
    oss.str("");
    oss << d;
    assert(oss.str() == "2000-02-29");
    iss.clear();
    iss.str("2001-02-29");
    assert(!(iss >> d));
    oss.str("");
    oss << d;
    assert(oss.str() == "2000-02-29");

    //-----------------------------------------------------------------------------
    // bonus test examples
    //-----------------------------------------------------------------------------

    CDate f(2000, 5, 12);
    oss.str("");
    oss << f;
    assert(oss.str() == "2000-05-12");
    oss.str("");
    oss << date_format("%Y/%m/%d") << f;
    assert(oss.str() == "2000/05/12");
    oss.str("");
    oss << date_format("%d.%m.%Y") << f;
    assert(oss.str() == "12.05.2000");
    oss.str("");
    oss << date_format("%m/%d/%Y") << f;
    assert(oss.str() == "05/12/2000");
    oss.str("");
    oss << date_format("%Y%m%d") << f;
    assert(oss.str() == "20000512");
    oss.str("");
    oss << date_format("hello kitty") << f;
    assert(oss.str() == "hello kitty");
    oss.str("");
    oss << date_format("%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%") << f;
    assert(oss.str() == "121212121212050505200020002000%%%%%");
    oss.str("");
    oss << date_format("%Y-%m-%d") << f;
    assert(oss.str() == "2000-05-12");
    iss.clear();

    iss.str("2001-01-1");
    assert(!(iss >> f));
    oss.str("");
    oss << f;
    assert(oss.str() == "2000-05-12");
    iss.clear();
    iss.str("2001-1-01");
    assert(!(iss >> f));
    oss.str("");
    oss << f;
    assert(oss.str() == "2000-05-12");
    iss.clear();
    iss.str("2001-001-01");
    assert(!(iss >> f));
    oss.str("");
    oss << f;
    assert(oss.str() == "2000-05-12");
    iss.clear();
    iss.str("2001-01-02");
    assert((iss >> date_format("%Y-%m-%d") >> f));
    oss.str("");
    oss << f;
    assert(oss.str() == "2001-01-02");
    iss.clear();
    iss.str("05.06.2003");
    assert((iss >> date_format("%d.%m.%Y") >> f));
    oss.str("");
    oss << f;
    assert(oss.str() == "2003-06-05");
    iss.clear();
    iss.str("07/08/2004");
    assert((iss >> date_format("%m/%d/%Y") >> f));
    oss.str("");
    oss << f;
    assert(oss.str() == "2004-07-08");
    iss.clear();
    iss.str("2002*03*04");
    assert((iss >> date_format("%Y*%m*%d") >> f));
    oss.str("");
    oss << f;
    assert(oss.str() == "2002-03-04");
    iss.clear();
    iss.str("C++09format10PA22006rulez");
    assert((iss >> date_format("C++%mformat%dPA2%Yrulez") >> f));
    oss.str("");
    oss << f;
    assert(oss.str() == "2006-09-10");
    iss.clear();
    iss.str("%12%13%2010%");
    assert((iss >> date_format("%%%m%%%d%%%Y%%") >> f));
    oss.str("");
    oss << f;
    assert(oss.str() == "2010-12-13");

    CDate g(2000, 6, 8);
    iss.clear();
    iss.str("2001-11-33");
    assert(!(iss >> date_format("%Y-%m-%d") >> g));
    oss.str("");
    oss << g;
    assert(oss.str() == "2000-06-08");
    iss.clear();
    iss.str("29.02.2003");
    assert(!(iss >> date_format("%d.%m.%Y") >> g));
    oss.str("");
    oss << g;
    assert(oss.str() == "2000-06-08");
    iss.clear();
    iss.str("14/02/2004");
    assert(!(iss >> date_format("%m/%d/%Y") >> g));
    oss.str("");
    oss << g;
    assert(oss.str() == "2000-06-08");
    iss.clear();
    iss.str("2002-03");
    assert(!(iss >> date_format("%Y-%m") >> g));
    oss.str("");
    oss << g;
    assert(oss.str() == "2000-06-08");
    iss.clear();
    iss.str("hello kitty");
    assert(!(iss >> date_format("hello kitty") >> g));
    oss.str("");
    oss << g;
    assert(oss.str() == "2000-06-08");
    iss.clear();
    iss.str("2005-07-12-07");
    assert(!(iss >> date_format("%Y-%m-%d-%m") >> g));
    oss.str("");
    oss << g;
    assert(oss.str() == "2000-06-08");
    iss.clear();
    iss.str("20000101");
    assert((iss >> date_format("%Y%m%d") >> g));
    oss.str("");
    oss << g;
    assert(oss.str() == "2000-01-01");

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
