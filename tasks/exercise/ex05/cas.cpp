/**
 * @file cas.cpp
 * @author Michal Dobeš
 * @date 2022-03-14
 *
 * @brief Implementation of class representing time
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

/**
 * @brief Representation of time with overloaded operators.
 *
 * Note: It was required to implement time as hours, minutes and seconds in proprietary variables.
 *
 */
class CTime {
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

    void recalculate() {

        /* Trick to substract seconds from time:
            When seconds go under zero after substracting, instead of calculating minutes and hours from this underflown variable
            add exactly one day to seconds, and then calculate minutes and days from seconds as if
            seconds were added (modulo cancels effects of the added day) */
        while (m_Second < 0) {
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

    static inline int min(int a, int b) {
        return a < b ? a : b;
    }

public:
    // Constructors
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

    // Arithmetic operators
    CTime & operator += (int seconds) {
        m_Second += seconds;
        recalculate();
        return *this;
    }

    friend CTime operator + (CTime lhs, int seconds) {
        lhs += seconds;
        return lhs;
    }

    friend CTime operator + (int seconds, const CTime & rhs) {
        return rhs + seconds;
    }

    CTime & operator -= (int seconds) {
        m_Second -= seconds;
        recalculate();
        return *this;
    }

    friend CTime operator - (CTime lhs, int seconds) {
        lhs -= seconds;
        return lhs;
    }

    friend CTime operator - (int seconds, const CTime & rhs) {
        return rhs - seconds;
    }

    friend int operator - (const CTime & lhs, const CTime & rhs) {
        return min((lhs - rhs.toSeconds()).toSeconds(), (rhs - lhs.toSeconds()).toSeconds());
    }

    CTime & operator ++ () {
        *this += 1;
        return *this;
    }

    CTime operator ++ (int) {
        CTime old = *this;
        ++(*this);
        return old;
    }

    CTime & operator -- () {
        *this -= 1;
        return *this;
    }

    CTime operator -- (int) {
        CTime old = *this;
        --(*this);
        return old;
    }

    // Comparison operators
    friend bool operator == (const CTime & lhs, const CTime & rhs) {
        return ((lhs.m_Hour == rhs.m_Hour) && (lhs.m_Minute == rhs.m_Minute) && (lhs.m_Second == rhs.m_Second));
    }

    friend bool operator != (const CTime & lhs, const CTime & rhs) {
        return !(lhs == rhs);
    }

    friend bool operator < (const CTime & lhs, const CTime & rhs) {
        if (lhs.m_Hour == rhs.m_Hour) {
            if (lhs.m_Minute == rhs.m_Minute) {
                return lhs.m_Second < rhs.m_Second;
            }
            return lhs.m_Minute < rhs.m_Minute;
        }
        return lhs.m_Hour < rhs.m_Hour;
    }

    friend bool operator > (const CTime & lhs, const CTime & rhs) {
        return rhs < lhs;
    }

    friend bool operator <= (const CTime & lhs, const CTime & rhs) {
        return !(lhs > rhs);
    }

    friend bool operator >= (const CTime & lhs, const CTime & rhs) {
        return !(lhs < rhs);
    }

    // Output operator
    friend ostream & operator << (ostream & out, const CTime & rhs) {
        out << std::setfill(' ') << std::setw(2) << rhs.m_Hour;
        out << ":";
        out << std::setfill('0') << std::setw(2) << rhs.m_Minute;
        out << ":";
        out << std::setfill('0') << std::setw(2) << rhs.m_Second;
        return out;
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

    CTime c(23, 59, 59);
    CTime d(0, 0, 1);

    assert((c - d) == 2);

    return 0;
}
#endif /* __PROGTEST__ */
