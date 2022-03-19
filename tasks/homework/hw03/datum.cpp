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
ios_base & (*date_format(const char * fmt)) (ios_base & x) {
    return [ ](ios_base & ios) -> ios_base & { return ios; };
}

#define MAX_DAYS 11322
#define MAX_YEAR 30

class CDate {
private:
    unsigned int days; // Days sice lower bound (01/01/2000) to the date this instance is representing

    class DateConvertor {
    private:
        unsigned int _internalYear;
        unsigned int _internalMonth;
        unsigned int _internalDay;

    public:
        static unsigned int leapYearsUntil(unsigned int year) {
            return (year / 4) - (year / 100) + (year / 400) - (year / 4000) + (year > 0 ? 1 : 0);
        }

        static bool leapYear(unsigned int year) {
            return ((year == 0) || (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0 && year % 4000 != 0));
        }

        static unsigned int daysInMonth(unsigned int m, unsigned int y) {
            unsigned int inMonth[12] = { 31, leapYear(y) ? (unsigned)(29) : (unsigned)(28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
            return inMonth[m - 1];
        }

        void convertFromDays(unsigned int days) {
            _internalYear = days / 365;


            _internalDay = (days - (365 * _internalYear));
            if (_internalYear > 0) {
                if (days % 365 < leapYearsUntil(_internalYear - 1)) {
                    _internalYear--;
                    _internalDay += 365;
                }
                _internalDay -= leapYearsUntil(_internalYear - 1);
            }

            _internalMonth = 1;
            while (_internalDay >= daysInMonth(_internalMonth, _internalYear)) {
                _internalDay -= daysInMonth(_internalMonth, _internalYear);
                _internalMonth++;
            }
        }

        unsigned int year() const {
            return _internalYear + 2000;
        }

        unsigned int month() const {
            return _internalMonth;
        }

        unsigned int day() const {
            return _internalDay + 1;
        }
    };

public:
    CDate(unsigned int y, unsigned int m, unsigned int d) {
        y -= 2000;

        if (m > 12 || m == 0 ||
            d > DateConvertor::daysInMonth(m, y) || d == 0 ||
            y > MAX_YEAR) {
            throw InvalidDateException();
        }

        days = y * 365;
        if (y > 0) {
            days += DateConvertor::leapYearsUntil(y - 1);
        }
        for (unsigned int i = 1; i < m; i++) {
            days += DateConvertor::daysInMonth(i, y);
        }
        days += (d - 1);
    }

    CDate(unsigned int d) {
        if (d > MAX_DAYS) {
            throw InvalidDateException();
        }
        days = d;
    }

    CDate & operator += (unsigned int d) {
        if (days + d > MAX_DAYS) {
            throw InvalidDateException();
        }
        days += d;
        return *this;
    }

    friend CDate operator + (CDate lhs, unsigned int d) {
        lhs += d;
        return lhs;
    }

    CDate & operator -= (unsigned int d) {
        if (days - d > MAX_DAYS) {
            throw InvalidDateException();
        }
        days -= d;
        return *this;
    }

    friend CDate operator - (CDate lhs, unsigned int d) {
        lhs -= d;
        return lhs;
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
        DateConvertor convertor;
        convertor.convertFromDays(rhs.days);
        os << convertor.year() << "-" << setfill('0') << setw(2) << convertor.month() << "-" << setw(2) << convertor.day();
        return os;
    }

    friend istream & operator >> (istream & is, CDate & rhs) {
        char dummy1, dummy2;
        int y, m, d;

        is >> y >> dummy1 >> m >> dummy2 >> d;
        if (dummy1 != '-' || dummy2 != '-') {
            is.setstate(ios::failbit);
            return is;
        }

        try {
            CDate newDate(y, m, d);
            rhs = newDate;
        }
        catch (InvalidDateException & e) {
            is.setstate(ios::failbit);
        }
        return is;
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
    /*
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
    */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
