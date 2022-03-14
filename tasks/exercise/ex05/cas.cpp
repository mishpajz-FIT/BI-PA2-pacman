#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

class CTime {
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

    void recalculate() {

        if (m_Second < 0) {
            m_Second += 60 * 60 * 24;
        }

        m_Minute += m_Second / 60;
        m_Hour += m_Minute / 60;

        m_Second %= 60;
        m_Minute %= 60;
        m_Hour %= 24;
    }

    int toSeconds() const {
        return ((m_Hour * 60) + m_Minute) * 60 + m_Second;
    }

    static int abs(int from) {
        if (from > 0) {
            return from;
        }
        return from * (-1);
    }

public:
    // constructor, destructor
    CTime() : m_Hour(0), m_Minute(0), m_Second(0) { }

    CTime(int hour, int minute, int second = 0) {
        if (hour >= 24 || hour < 0
            || minute >= 60 || minute < 0
            || second >= 60 || second < 0) {
            throw invalid_argument("Invalid values for time (hours must be 0-23, minutes must be 0-59, seconds must be 0-59)");
        }

        m_Hour = hour;
        m_Minute = minute;
        m_Second = second;
    }

    // arithmetic operators
    CTime & operator += (int seconds) {
        m_Second += seconds;
        recalculate();
        return *this;
    }

    CTime operator + (int seconds) {
        CTime newTime = *this;
        newTime += seconds;
        return newTime;
    }

    CTime & operator -= (int seconds) {
        m_Second -= seconds;
        recalculate();
        return *this;
    }

    CTime operator - (int seconds) {
        CTime newTime = *this;
        newTime -= seconds;
        return newTime;
    }

    friend int operator - (CTime & lhs, CTime & rhs) {
        return abs(lhs.toSeconds() - rhs.toSeconds());
    }

    CTime & operator ++ () {
        *this += 1;
        return *this;
    }

    CTime & operator -- () {
        *this -= 1;
        return *this;
    }

    // comparison operators
    bool operator == (CTime & rhs) const {
        return ((m_Hour == rhs.hours()) && (m_Minute == rhs.minutes()) && (m_Second == rhs.seconds()));
    }

    bool operator != (CTime & rhs) const {
        return !(*this == rhs);
    }

    bool operator > (CTime & rhs) const {
        if (m_Hour == rhs.hours()) {
            if (m_Minute == rhs.minutes()) {
                return m_Second > rhs.seconds();
            }
            return m_Minute > rhs.minutes();
        }
        return m_Hour > rhs.hours();
    }

    bool operator < (CTime & rhs) const {
        if (m_Hour == rhs.hours()) {
            if (m_Minute == rhs.minutes()) {
                return m_Second < rhs.seconds();
            }
            return m_Minute < rhs.minutes();
        }
        return m_Hour < rhs.hours();
    }

    bool operator <= (CTime & rhs) const {
        return (*this < rhs) || (*this == rhs);
    }

    bool operator >= (CTime & rhs) const {
        return (*this > rhs) || (*this == rhs);
    }

    // output operator
    friend ostream & operator << (ostream & out, CTime & rhs) {
        out << std::setfill(' ') << std::setw(2) << rhs.hours();
        out << ":";
        out << std::setfill('0') << std::setw(2) << rhs.minutes();
        out << ":";
        out << std::setfill('0') << std::setw(2) << rhs.seconds();
        return out;
    }

    // helper functions
    int hours() const {
        return m_Hour;
    }

    int minutes() const {
        return m_Minute;
    }

    int seconds() const {
        return m_Second;
    }

    friend class ::CTimeTester;
};

#ifndef __PROGTEST__
struct CTimeTester {
    static bool test(const CTime & time, int hour, int minute, int second) {
        return time.m_Hour == hour
            && time.m_Minute == minute
            && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main() {
    CTime a { 12, 30 };
    assert(CTimeTester::test(a, 12, 30, 0));

    CTime b { 13, 30 };
    assert(CTimeTester::test(b, 13, 30, 0));

    assert(b - a == 3600);

    assert(CTimeTester::test(a + 60, 12, 31, 0));
    assert(CTimeTester::test(a - 60, 12, 29, 0));

    assert(a < b);
    assert(a <= b);
    assert(a != b);
    assert(!(a > b));
    assert(!(a >= b));
    assert(!(a == b));

    while (++a != b);
    assert(a == b);

    std::ostringstream output;
    assert(static_cast<bool>(output << a));
    assert(output.str() == "13:30:00");

    assert(a++ == b++);
    assert(a == b);

    assert(--a == --b);
    assert(a == b);

    assert(a-- == b--);
    assert(a == b);

    return 0;
}
#endif /* __PROGTEST__ */
