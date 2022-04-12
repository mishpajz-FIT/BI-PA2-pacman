/**
 * @file datum.cpp
 * @author Michal Dobeš
 * @date 2022-03-26
 *
 * @brief Date representation, with stream modifier for formatted stream output/input
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstring>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

/**
 * @brief Dummy exception class
 *
 */
class InvalidDateException : public invalid_argument {
public:
    InvalidDateException() : invalid_argument("invalid date or format") { }
};

/**
 * @brief Handler of formatting
 *
 * Instance of this object is kept by stream and used for storing formatting string
 *
 */
struct DateFormatHandler {
    string formatString; //Formatting string for CDate
    void * currentStream; //Pointer to stream this object belongs to (only used to check for duplicate action on one stream)
};

/**
 * @brief Stream maniplator, used for formatting CDate
 *
 * Supports input and output stream.
 *
 * Serves as functor.
 * Utilized by passing this object to the stream which to modify.
 * The prameter when creating should be the format string.
 *
 * The format string can have special symbols: %Y - will be replaced by year, %m -  will be replaced by month, %d -  will be replaced by day, all other symbols will be kept
 *
 */
struct date_format {
private:
    /**
     * @brief std::event_callback method
     *
     * Reacts to callbacks, called when stream was registered by register_callback method
     *
     */
    static void caughtEvent(ios::event event, ios_base & stream, int index) {
        if (stream.iword(date_format::dateIndex) == 1) { //On stream destructon, deallocate DateFormatHandler stored in streams pword
            if (event == ios_base::erase_event) {
                delete static_cast<DateFormatHandler *>(stream.pword(date_format::dateIndex));
                stream.pword(date_format::dateIndex) = nullptr;
                stream.iword(date_format::dateIndex) = 0;
            } else if (event == ios_base::copyfmt_event) { // On stream copyfmt
                DateFormatHandler * formatHandler = static_cast<DateFormatHandler *>(stream.pword(date_format::dateIndex));
                if (formatHandler->currentStream != (&stream)) { //Check if this (possibly unknown, previously without format) stream has been processed (by checking stream in DateFormatHandler)
                    DateFormatHandler * newHandler = new DateFormatHandler();  //If stream has not been processed, create deep copy of DateFormatHandler and assign it to this streams pword
                    newHandler->formatString = string(formatHandler->formatString);
                    newHandler->currentStream = &stream;
                    stream.pword(date_format::dateIndex) = static_cast<void *>(newHandler);
                }
            }
        }
    }

    /**
     * @brief Store format string in stream
     *
     * @param stream Stream to modify
     * @param format Modifier
     */
    static void addFormatToStream(ios_base & stream, const date_format & format) {
        if (stream.iword(date_format::dateIndex) == 1) { //If stream already has modifier of this kind, dealloc it
            delete static_cast<DateFormatHandler *>(stream.pword(date_format::dateIndex));
            stream.pword(date_format::dateIndex) = nullptr;
        }
        stream.iword(date_format::dateIndex) = 1; //Raise flag in streams iword
        DateFormatHandler * newHandler = new DateFormatHandler(); //Allocate new DateFormatHandler, assign it format string and stream pointer
        newHandler->formatString = string(format.storedFormat);
        newHandler->currentStream = static_cast<void *>(&stream);
        stream.pword(date_format::dateIndex) = static_cast<void *>(newHandler); //Store DateFormatHandler in streams pword
        stream.register_callback(caughtEvent, date_format::dateIndex); //Register callback on stream (for deallocing DateFormatHandler and deep copying when std::copyfmt is called)
    }

public:
    const char * storedFormat; //Format string stored before being stored in stream

    date_format(const char * s) : storedFormat(s) { }

    //Global values for custom stream manipulator
    static const int dateIndex; //Unique id for modifier in iword and pword arrays of streams
    static const string defaultFormat; //Default formatting of CDate in stream

    friend ostream & operator << (ostream & os, const date_format & rhs) {
        addFormatToStream(os, rhs);
        return os;
    }

    friend istream & operator >> (istream & is, const date_format & rhs) {
        addFormatToStream(is, rhs);
        return is;
    }
};

const int date_format::dateIndex = ios::xalloc(); //Retrieve unique id in iword and pword arrays of streams
const string date_format::defaultFormat = "%Y-%m-%d"; //Default formatting of CDate in stream

/**
 * @brief Date representation
 *
 * Internally represented as days since bound date (01/01/2000)
 * Can represent only dates since 01/01/2000 and gurantees valid representation only until 31/12/2030
 *
 */
class CDate {
private:
    unsigned int days;//Days sice lower bound (01/01/2000) to the date this instance is representing

    /**
     * @brief Convertor used for caluclating other values (year, month and day) from internal representation (only days since date)
     *
     */
    class DateConvertor {
    private:
        unsigned int year;
        unsigned int month;
        unsigned int day;

    public:
        /**
         * @brief Returns leap years until year
         *
         * Works only for years over 2000, that have 2000 substracted from them (adds 1 for leap year 2000, which would be represented as 0).
         *
         * @param y year up to (including) calculate - 2000;
         * @return unsigned int Number of leap years
         */
        static unsigned int leapYearsUntil(unsigned int y) {
            return (y / 4) - (y / 100) + (y / 400) - (y / 4000) + 1;
        }

        /**
         * @brief Test if year is leap year
         *
         * Works for years over 2000, that have 2000 substracted from them (year 2000 is leap, which would be represented as 0).
         *
         * @param y year which should be checked - 2000
         * @return true Year is leap
         * @return false Year is not leap
         */
        static bool leapYear(unsigned int y) {
            return ((y == 0) || (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0 && y % 4000 != 0));
        }

        /**
         * @brief Days in month, including difference in leap years
         *
         * @param m Month
         * @param y Year - 2000
         * @return unsigned int Days in month
         */
        static unsigned int daysInMonth(unsigned int m, unsigned int y) {
            unsigned int inMonth[12] = { 31, leapYear(y) ? (unsigned)(29) : (unsigned)(28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
            return inMonth[m - 1];
        }

        /**
         * @brief Takes amount of days since baseline date and converts to date with day, month and year (and stores them as internal variables)
         *
         * @param days Days since 01/01/2000
         */
        void convertFromDays(unsigned int days) {

            year = days / 365; //Calculate max year
            day = (days - (365 * year)); //Substract years that were calculated from days

            if (year > 0) {
                if (days % 365 < leapYearsUntil(year - 1)) { //If counted one more year thanks to calculating with 365 days even for leap years, remove the year. 
                    year--;
                    day += 365;
                }
                if (year > 0) { //Remove 366th day for already calculated leap years
                    day -= leapYearsUntil(year - 1);
                }
            }

            month = 1;
            while (day >= daysInMonth(month, year)) { //Calculate day and month
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

    /**
     * @brief Output formatted CDate values into stream
     *
     * The format string can have special symbols: %Y - will be replaced by year, %m -  will be replaced by month, %d -  will be replaced by day, all other character will be kept
     *
     * @param stream Stream to output into
     * @param format Format string to format by
     * @param date Date to output
     * @return ostream& Stream where was CDate outputed
     */
    static ostream & formatOutput(ostream & stream, const string & format, const CDate & date) {
        DateConvertor convertor;
        convertor.convertFromDays(date.days); //Get date values from days

        bool modifierOnInput = false;;
        for (auto iter = format.begin(); iter != format.end(); iter++) { //Iterate through format string
            char c = *iter;
            if (c == '%' && !modifierOnInput) { //If encountered %, do not output anything but keep the information
                modifierOnInput = true;
                continue;
            } else {
                if (modifierOnInput) {
                    modifierOnInput = false;
                    if (c == 'd') { //If d, m, or Y follows % output specified value
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
                stream << c; //Output non-special character
            }
        }
        return stream;
    }

    /**
     * @brief From array of chars, (containing only characters between '0' - '9'), recreate numerical value
     *
     * @param to Where to store final value
     * @param chars Array of chars
     * @param size Length of array of chars
     * @return true Conversion was successful
     * @return false Conversion wasn't successful
     */
    static bool processCharsToInt(int & to, char * chars, int size) {
        to = 0;

        int multiplier = 1; //Multiplier specifing digit number
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

    /**
     * @brief Extract chars from input stream into array of chars
     *
     * @param stream Stream to extract from
     * @param to Array to store chars into
     * @param count Number of chars to extract
     * @return true Extraction was successful
     * @return false Extraction was not successful
     */
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

    /**
     * @brief Extracts numerical value from stream
     *
     * Limited to 4 digits.
     *
     * @param stream Stream to extract from
     * @param extracted Has this value been extracted
     * @param value Store extracted value into
     * @param digits Number of digits to extract (max 4)
     * @return true Extraction was successful
     * @return false Extraction was not successful
     */
    static bool getDateValueFromStream(istream & stream, bool & extracted, int & value, int digits) {
        char array[4];
        if (extracted || !extractCharsFromStream(stream, array, digits) || !processCharsToInt(value, array, digits)) {
            return false;
        }
        extracted = true;
        return true;
    }

    /**
     * @brief Get CDate from stream using specified format
     *
     * @param stream Stream to input from
     * @param format Format string to expect
     * @param date Date to output
     * @return istream& Stream where was CDate inputed
     */
    static istream & formatInput(istream & stream, const string & format, CDate & date) {
        int d = 0, m = 0, y = 0; //Recieved values
        bool dIn = false, mIn = false, yIn = false; //Variables specifing if values were recieved

        bool modifierOnInput = false;
        for (auto iter = format.begin(); iter != format.end(); iter++) { //Iterate through format string
            char forChar = *iter;
            if (forChar == '%' && !modifierOnInput) { //If encountered %, do not input anything but keep the information
                modifierOnInput = true;
                continue;
            } else {
                if (modifierOnInput) {
                    modifierOnInput = false;
                    if (forChar == 'd') { //If d, m, or Y follows % extract expected value
                        if (!getDateValueFromStream(stream, dIn, d, 2)) {
                            stream.setstate(ios::failbit);
                            return stream;
                        }
                        continue;
                    } else if (forChar == 'm') {
                        if (!getDateValueFromStream(stream, mIn, m, 2)) {
                            stream.setstate(ios::failbit);
                            return stream;
                        }
                        continue;
                    } else if (forChar == 'Y') {
                        if (!getDateValueFromStream(stream, yIn, y, 4)) {
                            stream.setstate(ios::failbit);
                            return stream;
                        }
                        continue;
                    }
                }

                char inChar = stream.get(); //Extract non-special character and check if it conforms expected format
                if (inChar != forChar || stream.fail()) {
                    stream.setstate(ios::failbit);
                    return stream;
                }
            }
        }

        if (!dIn || !mIn || !yIn) { //Check if all values have been inputted
            stream.setstate(ios::failbit);
            return stream;
        }

        try { //Try to create CDate from inputted variables
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
            d > DateConvertor::daysInMonth(m, y) || d == 0) { //Input sanitization
            throw InvalidDateException();
        }

        y -= 2000;

        days = y * 365; //Calculate total days from years
        if (y > 0) {
            days += DateConvertor::leapYearsUntil(y - 1); //Add 366th day thanks to leap years
        }

        for (unsigned int i = 1; i < m; i++) { //Calculate total days from months
            days += DateConvertor::daysInMonth(i, y);
        }

        days += (d - 1); //Calculate total days from days
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
        if (os.iword(date_format::dateIndex) == 1) { //If stream contains flag, specifying format modifier has been used, retrieve format string from DateFormatHandler and format output using this string, else use default format
            DateFormatHandler * formatHandler = static_cast<DateFormatHandler *>(os.pword(date_format::dateIndex));
            string formatString(formatHandler->formatString);
            return formatOutput(os, formatString, rhs);
        }
        return formatOutput(os, date_format::defaultFormat, rhs);
    }

    friend istream & operator >> (istream & is, CDate & rhs) {
        if (is.iword(date_format::dateIndex) == 1) { //If stream contains flag, specifying format modifier has been used, retrieve format string from DateFormatHandler and format input using this string, else use default format
            DateFormatHandler * formatHandler = static_cast<DateFormatHandler *>(is.pword(date_format::dateIndex));
            string formatString(formatHandler->formatString);
            return formatInput(is, formatString, rhs);
        }
        return formatInput(is, date_format::defaultFormat, rhs);
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
    // formattor test examples
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
    ostringstream ossss;
    ossss.copyfmt(oss);
    ossss << f;
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
