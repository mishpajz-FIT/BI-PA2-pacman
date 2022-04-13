/**
 * @file datove_typy.cpp
 * @author Michal Dobeš (dobesmic@fit.cvut.cz)
 * @date 2022-04-13
 *
 * @brief A set of classes that implement the description of some data types
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

/**
 * @brief Generic data type
 *
 */
class CDataType {
protected:
    /**
     * @brief Is this type equal to rhs
     *
     * @param rhs CDataType
     * @return true Types are equal
     * @return false Types are not equal
     */
    virtual bool isEqualTo(const CDataType & rhs) const = 0;

    /**
     * @brief Indent output stream by level
     *
     * @param stream Stream to indent
     * @param level Level of indentation
     */
    static void indentStream(ostream & stream, size_t level) {
        while (level--) {
            stream << "  ";
        }
    }

public:
    CDataType() { }

    virtual ~CDataType() { }

    /**
     * @brief Size of this type
     *
     * @return size_t size
     */
    virtual size_t getSize() const = 0;

    bool operator == (const CDataType & rhs) const {
        return isEqualTo(rhs);
    }

    bool operator != (const CDataType & rhs) const {
        return !(isEqualTo(rhs));
    }

    /**
     * @brief Allocate copy of this object
     *
     * @return CDataType* Pointer to copy
     */
    virtual CDataType * clone() const = 0;

    /**
     * @brief Print this type to stream
     *
     * @param stream Stream to print to
     * @param level Indentation level
     * @param suffix Suffix to add after printing (if type allows that)
     */
    virtual void print(ostream & stream, size_t level = 0, string suffix = "") const = 0;

    friend ostream & operator << (ostream & stream, const CDataType & rhs) {
        rhs.print(stream);
        return stream;
    }

    /**
     * @brief Element of this type
     *
     * @return const CDataType& Element
     */
    virtual const CDataType & element() const {
        stringstream ss;
        print(ss);
        throw std::invalid_argument("Cannot use element() for type: " + ss.str());
    }

    /**
     * @brief Field of this type
     *
     * @param name Name of field
     * @return const CDataType& Field
     */
    virtual const CDataType & field(const string & name) const {
        stringstream ss;
        print(ss);
        throw std::invalid_argument("Cannot use field() for type: " + ss.str());
    }
};

/**
 * @brief Int data type
 *
 */
class CDataTypeInt : public CDataType {
protected:
    bool isEqualTo(const CDataType & rhs) const override {
        return (dynamic_cast<const CDataTypeInt *>(&rhs) != nullptr);
    }

    void print(ostream & stream, size_t level = 0, string suffix = "") const override {
        CDataType::indentStream(stream, level);
        stream << "int";
        stream << suffix;
    }

    CDataTypeInt * clone() const override {
        return (new CDataTypeInt(*this));
    }

public:
    size_t getSize() const override {
        return 4;
    }
};

/**
 * @brief Double data type
 *
 */
class CDataTypeDouble : public CDataType {
protected:
    bool isEqualTo(const CDataType & rhs) const override {
        return (dynamic_cast<const CDataTypeDouble *>(&rhs) != nullptr);
    }

    void print(ostream & stream, size_t level = 0, string suffix = "") const override {
        CDataType::indentStream(stream, level);
        stream << "double";
        stream << suffix;
    }

    CDataTypeDouble * clone() const override {
        return (new CDataTypeDouble(*this));
    }

public:
    size_t getSize() const override {
        return 8;
    }
};

/**
 * @brief Enum data type
 *
 */
class CDataTypeEnum : public CDataType {
private:
    vector<string> values; //< Values of enum, sorted by insertion order

protected:
    bool isEqualTo(const CDataType & rhs) const override {
        const CDataTypeEnum * ptrRhs = dynamic_cast<const CDataTypeEnum *>(&rhs);

        // Check if values in the enum are same
        if ((ptrRhs == nullptr)
            || (values.size() != ptrRhs->values.size())) {
            return false;
        }

        for (size_t i = 0; i < values.size(); i++) {
            if (values[i] != ptrRhs->values[i]) {
                return false;
            }
        }

        return true;
    }

    void print(ostream & stream, size_t level = 0, string suffix = "") const override {
        CDataType::indentStream(stream, level);
        stream << "enum\n";

        CDataType::indentStream(stream, level);
        stream << "{\n";

        // Print contents of enum
        for (size_t i = 0; i < values.size(); i++) {
            CDataType::indentStream(stream, level + 1);
            stream << values[i];
            if (i + 1 < values.size()) { //< Print separator between values
                stream << ",";
            }
            stream << "\n";
        }

        CDataType::indentStream(stream, level);
        stream << "}";
        stream << suffix;
    }

    CDataTypeEnum * clone() const override {
        return (new CDataTypeEnum(*this));
    }

public:
    size_t getSize() const override {
        return 4;
    }

    /**
     * @brief Add value to enum
     *
     * Throws invalid_argument if value is duplicate
     *
     * @param value Value to add
     * @return CDataTypeEnum& this
     */
    CDataTypeEnum & add(const string & value) {
        if (find(values.begin(), values.end(), value) != values.end()) { //< Try to find if value is already contained, throw if it is
            throw std::invalid_argument("Duplicate enum value: " + value);
        }

        values.emplace_back(value);
        return (*this);
    }
};

/**
 * @brief Struct data type
 *
 */
class CDataTypeStruct : public CDataType {
private:
    vector<pair<string, CDataType *>> values; //< Fields of struct, kept in order of insertion

    /**
     * @brief Deallocate fields
     *
     */
    void deallocValues() {
        for (auto & v : values) {
            delete v.second;
        }
        values.clear();
    }

    /**
     * @brief Find field
     *
     * @param name Name to find
     * @return vector<pair<string, CDataType *>>::const_iterator Iterator to field found
     */
    vector<pair<string, CDataType *>>::const_iterator findInValues(const string & name) const {
        auto iter = values.begin();
        for (; iter < values.end(); iter++) {
            if ((*iter).first == name) {
                break;
            }
        }

        return iter;
    }

protected:
    bool isEqualTo(const CDataType & rhs) const override {
        const CDataTypeStruct * ptrRhs = dynamic_cast<const CDataTypeStruct *>(&rhs);

        // Check if fields in enum are the same (compare only their values)
        if ((ptrRhs == nullptr)
            || (values.size() != ptrRhs->values.size())) {
            return false;
        }

        for (size_t i = 0; i < values.size(); i++) {
            if ((*values[i].second) != (*(ptrRhs->values[i].second))) {
                return false;
            }
        }

        return true;
    }

    void print(ostream & stream, size_t level = 0, string suffix = "") const override {
        CDataType::indentStream(stream, level);
        stream << "struct\n";

        CDataType::indentStream(stream, level);
        stream << "{\n";

        for (const auto & v : values) {
            string newSuffix = " ";
            newSuffix += v.first; //< Set field name to print as suffix after field value
            v.second->print(stream, level + 1, newSuffix); //< Print value of field
            stream << ";\n"; //< Print separator after field
        }

        CDataType::indentStream(stream, level);
        stream << "}";
        stream << suffix;

    }

    CDataTypeStruct * clone() const override {
        return (new CDataTypeStruct(*this));
    }

public:
    CDataTypeStruct() : CDataType() { }

    CDataTypeStruct(const CDataTypeStruct & toCopy) : CDataType() {
        for (auto & v : toCopy.values) { //< Copy field values
            values.push_back(make_pair(v.first, v.second->clone()));
        }
    }

    ~CDataTypeStruct() {
        deallocValues();
    }

    CDataTypeStruct & operator = (const CDataTypeStruct & toCopy) {
        if (this == &toCopy) {
            return *this;
        }

        // Deallocate current fields and copy field values
        deallocValues();

        for (auto & v : toCopy.values) {
            values.push_back(make_pair(v.first, v.second->clone()));
        }

        return (*this);
    }

    size_t getSize() const override {
        size_t sum = 0;
        for (auto & v : values) { // Get size of values of fields
            sum += v.second->getSize();
        }
        return sum;
    }

    /**
     * @brief Add field to struct
     *
     * If name is duplicate, throws invalid_argument
     *
     * @param name Name of field
     * @param value Value of field
     * @return CDataTypeStruct&
     */
    CDataTypeStruct & addField(const string & name, const CDataType & value) {
        auto iter = findInValues(name);

        if (iter != values.end()) { //< Check if name is unique
            throw std::invalid_argument("Duplicate field: " + name);
        }

        CDataType * valueCopy = value.clone(); //< Copy value and add
        values.push_back(make_pair(name, valueCopy));

        return (*this);
    }

    const CDataType & field(const string & name) const override {
        auto iter = findInValues(name);

        if (iter == values.end()) {
            throw std::invalid_argument("Unknown field: " + name);
        }

        return (*((*iter).second));
    }
};

/**
 * @brief Array data type
 *
 */
class CDataTypeArray : public CDataType {
private:
    CDataType * type; //< Data type this array stores
    size_t count; //< Amount of size of array

protected:
    bool isEqualTo(const CDataType & rhs) const override {
        const CDataTypeArray * ptrRhs = dynamic_cast<const CDataTypeArray *>(&rhs);

        // Compare data type of array and count of elements
        if ((ptrRhs == nullptr)
            || (typeid(*(ptrRhs->type)) != typeid(*type))
            || ptrRhs->count != count) {
            return false;
        }

        return true;
    }

    void print(ostream & stream, size_t level = 0, string suffix = "") const override {
        suffix += "["; //< Add count as suffix
        suffix += to_string(count);
        suffix += "]";
        type->print(stream, level, suffix); //< Print array's type
    }

    CDataTypeArray * clone() const override {
        return (new CDataTypeArray(*this));
    }

public:
    CDataTypeArray(size_t s, const CDataType & t) : CDataType(), count(s) {
        type = t.clone(); //< Copy type
    }

    CDataTypeArray(const CDataTypeArray & toCopy) : CDataType(), count(toCopy.count) {
        CDataType * clone = toCopy.type->clone();
        type = clone;
    }

    ~CDataTypeArray() {
        delete type;
    }

    CDataTypeArray & operator = (const CDataTypeArray & toCopy) {
        if (this == &toCopy) {
            return *this;
        }

        // Copy type, then deallocate, then assign copied type
        CDataType * clone = toCopy.type->clone();

        delete type;
        type = clone;

        return (*this);
    }

    size_t getSize() const override {
        return (type->getSize() * count);
    }

    const CDataType & element() const override {
        return (*type);
    }
};

/**
 * @brief Pointer data type
 *
 */
class CDataTypePtr : public CDataType {
private:
    CDataType * type; //< Data type this points to

protected:
    bool isEqualTo(const CDataType & rhs) const override {
        const CDataTypePtr * ptrRhs = dynamic_cast<const CDataTypePtr *>(&rhs);

        // Copmare if type of pointers are same
        if ((ptrRhs == nullptr)
            || (typeid(*(ptrRhs->type)) != typeid(*type))) {
            return false;
        }

        return true;
    }

    void print(ostream & stream, size_t level = 0, string suffix = "") const override {
        bool pointingToArray = ((dynamic_cast<const CDataTypeArray *>(type) != nullptr) ? true : false); //< Check if pointer is pointing to array

        CDataType::indentStream(stream, level);

        string newSuffix = "";
        if (pointingToArray) { //< If pointing to array, add brackets to pointer symbol (before and after suffix)
            newSuffix = "(*";
            newSuffix += suffix;
            newSuffix += ")";
        } else { //< Add pointer symbol to before suffix
            newSuffix = "*";
            newSuffix += suffix;
        };

        type->print(stream, 0, newSuffix); //< Print type with suffix
    }

    CDataTypePtr * clone() const override {
        return (new CDataTypePtr(*this));
    }

public:
    CDataTypePtr(const CDataType & t) : CDataType() {
        type = t.clone(); //< Copy type
    }

    CDataTypePtr(const CDataTypePtr & toCopy) : CDataType() {
        CDataType * clone = toCopy.type->clone(); //< Copy type
        type = clone;
    }

    ~CDataTypePtr() {
        delete type;
    }

    CDataTypePtr & operator = (const CDataTypePtr & toCopy) {
        if (this == &toCopy) {
            return *this;
        }

        // Copy type, then deallocate, then assign copied type
        CDataType * clone = toCopy.type->clone();

        delete type;
        type = clone;

        return (*this);
    }

    size_t getSize() const override {
        return 8;
    }

    const CDataType & element() const override {
        return (*type);
    }
};



#ifndef __PROGTEST__
static bool whitespaceMatch(const string & a, const string & b) {
    string prepStringA = a;
    string prepStringB = b;

    size_t aIt = 0;
    size_t bIt = 0;

    while (true) {
        while (isspace(a[aIt])) {
            aIt++;
        }

        while (isspace(b[bIt])) {
            bIt++;
        }

        if (a[aIt] != b[bIt]) {
            return false;
        }

        if (aIt + 1 >= a.length() || bIt + 1 >= b.length()) {
            break;
        }

        aIt++;
        bIt++;
    }

    return true;
}

template <typename T_>
static bool whitespaceMatch(const T_ & x, const string & ref) {
    ostringstream oss;
    oss << x;
    return whitespaceMatch(oss.str(), ref);
}
int main(void) {
    CDataTypeStruct  a = CDataTypeStruct() .
        addField("m_Length", CDataTypeInt()) .
        addField("m_Status", CDataTypeEnum() .
            add("NEW") .
            add("FIXED") .
            add("BROKEN") .
            add("DEAD")).
        addField("m_Ratio", CDataTypeDouble());

    CDataTypeStruct b = CDataTypeStruct() .
        addField("m_Length", CDataTypeInt()) .
        addField("m_Status", CDataTypeEnum() .
            add("NEW") .
            add("FIXED") .
            add("BROKEN") .
            add("READY")).
        addField("m_Ratio", CDataTypeDouble());

    CDataTypeStruct c = CDataTypeStruct() .
        addField("m_First", CDataTypeInt()) .
        addField("m_Second", CDataTypeEnum() .
            add("NEW") .
            add("FIXED") .
            add("BROKEN") .
            add("DEAD")).
        addField("m_Third", CDataTypeDouble());

    CDataTypeStruct  d = CDataTypeStruct() .
        addField("m_Length", CDataTypeInt()) .
        addField("m_Status", CDataTypeEnum() .
            add("NEW") .
            add("FIXED") .
            add("BROKEN") .
            add("DEAD")).
        addField("m_Ratio", CDataTypeInt());
    assert(whitespaceMatch(a, "struct\n"
        "{\n"
        "  int m_Length;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    DEAD\n"
        "  } m_Status;\n"
        "  double m_Ratio;\n"
        "}"));

    assert(whitespaceMatch(b, "struct\n"
        "{\n"
        "  int m_Length;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    READY\n"
        "  } m_Status;\n"
        "  double m_Ratio;\n"
        "}"));

    assert(whitespaceMatch(c, "struct\n"
        "{\n"
        "  int m_First;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    DEAD\n"
        "  } m_Second;\n"
        "  double m_Third;\n"
        "}"));

    assert(whitespaceMatch(d, "struct\n"
        "{\n"
        "  int m_Length;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    DEAD\n"
        "  } m_Status;\n"
        "  int m_Ratio;\n"
        "}"));

    assert(a != b);
    assert(a == c);
    assert(a != d);
    assert(a != CDataTypeInt());
    assert(whitespaceMatch(a.field("m_Status"), "enum\n"
        "{\n"
        "  NEW,\n"
        "  FIXED,\n"
        "  BROKEN,\n"
        "  DEAD\n"
        "}"));

    b.addField("m_Other", a);

    a.addField("m_Sum", CDataTypeInt());

    assert(whitespaceMatch(a, "struct\n"
        "{\n"
        "  int m_Length;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    DEAD\n"
        "  } m_Status;\n"
        "  double m_Ratio;\n"
        "  int m_Sum;\n"
        "}"));

    assert(whitespaceMatch(b, "struct\n"
        "{\n"
        "  int m_Length;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    READY\n"
        "  } m_Status;\n"
        "  double m_Ratio;\n"
        "  struct\n"
        "  {\n"
        "    int m_Length;\n"
        "    enum\n"
        "    {\n"
        "      NEW,\n"
        "      FIXED,\n"
        "      BROKEN,\n"
        "      DEAD\n"
        "    } m_Status;\n"
        "    double m_Ratio;\n"
        "  } m_Other;\n"
        "}"));

    assert(whitespaceMatch(b.field("m_Other"), "struct\n"
        "{\n"
        "  int m_Length;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    DEAD\n"
        "  } m_Status;\n"
        "  double m_Ratio;\n"
        "}"));

    assert(whitespaceMatch(b.field("m_Other").field("m_Status"), "enum\n"
        "{\n"
        "  NEW,\n"
        "  FIXED,\n"
        "  BROKEN,\n"
        "  DEAD\n"
        "}"));

    assert(a.getSize() == 20);
    assert(b.getSize() == 32);
    b.addField("m_Other1", b);
    b.addField("m_Other2", b);
    b.addField("m_Other3", b);
    assert(b.field("m_Other3").field("m_Other2").field("m_Other1") == b.field("m_Other1"));

    assert(b.getSize() == 256);

    assert(whitespaceMatch(b, "struct\n"
        "{\n"
        "  int m_Length;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    READY\n"
        "  } m_Status;\n"
        "  double m_Ratio;\n"
        "  struct\n"
        "  {\n"
        "    int m_Length;\n"
        "    enum\n"
        "    {\n"
        "      NEW,\n"
        "      FIXED,\n"
        "      BROKEN,\n"
        "      DEAD\n"
        "    } m_Status;\n"
        "    double m_Ratio;\n"
        "  } m_Other;\n"
        "  struct\n"
        "  {\n"
        "    int m_Length;\n"
        "    enum\n"
        "    {\n"
        "      NEW,\n"
        "      FIXED,\n"
        "      BROKEN,\n"
        "      READY\n"
        "    } m_Status;\n"
        "    double m_Ratio;\n"
        "    struct\n"
        "    {\n"
        "      int m_Length;\n"
        "      enum\n"
        "      {\n"
        "        NEW,\n"
        "        FIXED,\n"
        "        BROKEN,\n"
        "        DEAD\n"
        "      } m_Status;\n"
        "      double m_Ratio;\n"
        "    } m_Other;\n"
        "  } m_Other1;\n"
        "  struct\n"
        "  {\n"
        "    int m_Length;\n"
        "    enum\n"
        "    {\n"
        "      NEW,\n"
        "      FIXED,\n"
        "      BROKEN,\n"
        "      READY\n"
        "    } m_Status;\n"
        "    double m_Ratio;\n"
        "    struct\n"
        "    {\n"
        "      int m_Length;\n"
        "      enum\n"
        "      {\n"
        "        NEW,\n"
        "        FIXED,\n"
        "        BROKEN,\n"
        "        DEAD\n"
        "      } m_Status;\n"
        "      double m_Ratio;\n"
        "    } m_Other;\n"
        "    struct\n"
        "    {\n"
        "      int m_Length;\n"
        "      enum\n"
        "      {\n"
        "        NEW,\n"
        "        FIXED,\n"
        "        BROKEN,\n"
        "        READY\n"
        "      } m_Status;\n"
        "      double m_Ratio;\n"
        "      struct\n"
        "      {\n"
        "        int m_Length;\n"
        "        enum\n"
        "        {\n"
        "          NEW,\n"
        "          FIXED,\n"
        "          BROKEN,\n"
        "          DEAD\n"
        "        } m_Status;\n"
        "        double m_Ratio;\n"
        "      } m_Other;\n"
        "    } m_Other1;\n"
        "  } m_Other2;\n"
        "  struct\n"
        "  {\n"
        "    int m_Length;\n"
        "    enum\n"
        "    {\n"
        "      NEW,\n"
        "      FIXED,\n"
        "      BROKEN,\n"
        "      READY\n"
        "    } m_Status;\n"
        "    double m_Ratio;\n"
        "    struct\n"
        "    {\n"
        "      int m_Length;\n"
        "      enum\n"
        "      {\n"
        "        NEW,\n"
        "        FIXED,\n"
        "        BROKEN,\n"
        "        DEAD\n"
        "      } m_Status;\n"
        "      double m_Ratio;\n"
        "    } m_Other;\n"
        "    struct\n"
        "    {\n"
        "      int m_Length;\n"
        "      enum\n"
        "      {\n"
        "        NEW,\n"
        "        FIXED,\n"
        "        BROKEN,\n"
        "        READY\n"
        "      } m_Status;\n"
        "      double m_Ratio;\n"
        "      struct\n"
        "      {\n"
        "        int m_Length;\n"
        "        enum\n"
        "        {\n"
        "          NEW,\n"
        "          FIXED,\n"
        "          BROKEN,\n"
        "          DEAD\n"
        "        } m_Status;\n"
        "        double m_Ratio;\n"
        "      } m_Other;\n"
        "    } m_Other1;\n"
        "    struct\n"
        "    {\n"
        "      int m_Length;\n"
        "      enum\n"
        "      {\n"
        "        NEW,\n"
        "        FIXED,\n"
        "        BROKEN,\n"
        "        READY\n"
        "      } m_Status;\n"
        "      double m_Ratio;\n"
        "      struct\n"
        "      {\n"
        "        int m_Length;\n"
        "        enum\n"
        "        {\n"
        "          NEW,\n"
        "          FIXED,\n"
        "          BROKEN,\n"
        "          DEAD\n"
        "        } m_Status;\n"
        "        double m_Ratio;\n"
        "      } m_Other;\n"
        "      struct\n"
        "      {\n"
        "        int m_Length;\n"
        "        enum\n"
        "        {\n"
        "          NEW,\n"
        "          FIXED,\n"
        "          BROKEN,\n"
        "          READY\n"
        "        } m_Status;\n"
        "        double m_Ratio;\n"
        "        struct\n"
        "        {\n"
        "          int m_Length;\n"
        "          enum\n"
        "          {\n"
        "            NEW,\n"
        "            FIXED,\n"
        "            BROKEN,\n"
        "            DEAD\n"
        "          } m_Status;\n"
        "          double m_Ratio;\n"
        "        } m_Other;\n"
        "      } m_Other1;\n"
        "    } m_Other2;\n"
        "  } m_Other3;\n"
        "}"));

    try {
        a.addField("m_Status", CDataTypeInt());
        assert("addField: missing exception!" == nullptr);
    }
    catch (const invalid_argument & e) {
        assert(e.what() == "Duplicate field: m_Status"sv);
    }

    try {
        cout << a.field("m_Fail") << endl;
        assert("field: missing exception!" == nullptr);
    }
    catch (const invalid_argument & e) {
        assert(e.what() == "Unknown field: m_Fail"sv);
    }

    try {
        cout << a.field("m_Length").field("m_Foo") << endl;
        assert("field: missing exception!" == nullptr);
    }
    catch (const invalid_argument & e) {
        assert(whitespaceMatch(e.what(), "Cannot use field() for type: int"));
    }

    try {
        cout << a.field("m_Status").field("m_Foo") << endl;
        assert("field: missing exception!" == nullptr);
    }
    catch (const invalid_argument & e) {
        assert(whitespaceMatch(e.what(), "Cannot use field() for type: enum\n"
            "{\n"
            "  NEW,\n"
            "  FIXED,\n"
            "  BROKEN,\n"
            "  DEAD\n"
            "}"));
    }

    try {
        CDataTypeEnum en;
        en.add("FIRST") .
            add("SECOND") .
            add("FIRST");
        assert("add: missing exception!" == nullptr);
    }
    catch (const invalid_argument & e) {
        assert(e.what() == "Duplicate enum value: FIRST"sv);
    }

    CDataTypeArray ar1(10, CDataTypeInt());
    assert(whitespaceMatch(ar1, "int[10]"));
    assert(whitespaceMatch(ar1.element(), "int"));
    CDataTypeArray ar2(11, ar1);
    assert(whitespaceMatch(ar2, "int[11][10]"));
    assert(whitespaceMatch(ar2.element(), "int[10]"));
    assert(whitespaceMatch(ar2.element().element(), "int"));
    CDataTypeArray ar3(10, CDataTypeArray(20, CDataTypePtr(CDataTypeInt())));
    assert(whitespaceMatch(ar3, "int*[10][20]"));
    assert(whitespaceMatch(ar3.element(), "int*[20]"));
    assert(whitespaceMatch(ar3.element().element(), "int*"));
    assert(whitespaceMatch(ar3.element().element().element(), "int"));
    CDataTypePtr ar4(ar1.element());
    assert(whitespaceMatch(ar4, "int*"));
    assert(whitespaceMatch(ar4.element(), "int"));
    CDataTypePtr ar5(b.field("m_Status"));
    assert(whitespaceMatch(ar5, "enum\n"
        "{\n"
        "  NEW,\n"
        "  FIXED,\n"
        "  BROKEN,\n"
        "  READY\n"
        "}*"));
    assert(whitespaceMatch(ar5.element(), "enum\n"
        "{\n"
        "  NEW,\n"
        "  FIXED,\n"
        "  BROKEN,\n"
        "  READY\n"
        "}"));
    CDataTypePtr ar6(ar3.element().element());
    assert(whitespaceMatch(ar6, "int**"));
    assert(whitespaceMatch(ar6.element(), "int*"));
    assert(whitespaceMatch(ar6.element().element(), "int"));
    CDataTypePtr ar7(CDataTypeArray(50, ar6));
    assert(whitespaceMatch(ar7, "int**(*)[50]"));
    assert(whitespaceMatch(ar7.element(), "int**[50]"));
    assert(whitespaceMatch(ar7.element().element(), "int**"));
    assert(whitespaceMatch(ar7.element().element().element(), "int*"));
    assert(whitespaceMatch(ar7.element().element().element().element(), "int"));
    CDataTypeArray ar8(25, ar7);
    assert(whitespaceMatch(ar8, "int**(*[25])[50]"));
    CDataTypePtr ar9(ar8);
    assert(whitespaceMatch(ar9, "int**(*(*)[25])[50]"));
    a.addField("m_Ar1", ar1) .
        addField("m_Ar2", ar2) .
        addField("m_Ar3", ar3) .
        addField("m_Ar4", ar4) .
        addField("m_Ar5", ar5) .
        addField("m_Ar6", ar6) .
        addField("m_Ar7", ar7) .
        addField("m_Ar8", ar8) .
        addField("m_Ar9", ar9);
    assert(whitespaceMatch(a, "struct\n"
        "{\n"
        "  int m_Length;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    DEAD\n"
        "  } m_Status;\n"
        "  double m_Ratio;\n"
        "  int m_Sum;\n"
        "  int m_Ar1[10];\n"
        "  int m_Ar2[11][10];\n"
        "  int* m_Ar3[10][20];\n"
        "  int* m_Ar4;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    READY\n"
        "  }* m_Ar5;\n"
        "  int** m_Ar6;\n"
        "  int**(* m_Ar7)[50];\n"
        "  int**(* m_Ar8[25])[50];\n"
        "  int**(*(* m_Ar9)[25])[50];\n"
        "}"));
    a.addField("m_Madness", CDataTypeArray(40, CDataTypePtr(a)));
    assert(whitespaceMatch(a, "struct\n"
        "{\n"
        "  int m_Length;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    DEAD\n"
        "  } m_Status;\n"
        "  double m_Ratio;\n"
        "  int m_Sum;\n"
        "  int m_Ar1[10];\n"
        "  int m_Ar2[11][10];\n"
        "  int* m_Ar3[10][20];\n"
        "  int* m_Ar4;\n"
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    READY\n"
        "  }* m_Ar5;\n"
        "  int** m_Ar6;\n"
        "  int**(* m_Ar7)[50];\n"
        "  int**(* m_Ar8[25])[50];\n"
        "  int**(*(* m_Ar9)[25])[50];\n"
        "  struct\n"
        "  {\n"
        "    int m_Length;\n"
        "    enum\n"
        "    {\n"
        "      NEW,\n"
        "      FIXED,\n"
        "      BROKEN,\n"
        "      DEAD\n"
        "    } m_Status;\n"
        "    double m_Ratio;\n"
        "    int m_Sum;\n"
        "    int m_Ar1[10];\n"
        "    int m_Ar2[11][10];\n"
        "    int* m_Ar3[10][20];\n"
        "    int* m_Ar4;\n"
        "    enum\n"
        "    {\n"
        "      NEW,\n"
        "      FIXED,\n"
        "      BROKEN,\n"
        "      READY\n"
        "    }* m_Ar5;\n"
        "    int** m_Ar6;\n"
        "    int**(* m_Ar7)[50];\n"
        "    int**(* m_Ar8[25])[50];\n"
        "    int**(*(* m_Ar9)[25])[50];\n"
        "  }* m_Madness[40];\n"
        "}"));
    assert(a.field("m_Madness").element().element().field("m_Ar9") == a.field("m_Ar9"));
    assert(a.field("m_Madness").element().element().field("m_Ar9") != a.field("m_Ar8"));
    try {
        cout << ar2.field("m_Foo") << endl;
        assert("field: missing exception!" == nullptr);
    }
    catch (const invalid_argument & e) {
        assert(whitespaceMatch(e.what(), "Cannot use field() for type: int[11][10]"));
    }

    try {
        cout << c.element() << endl;
        assert("element: missing exception!" == nullptr);
    }
    catch (const invalid_argument & e) {
        assert(whitespaceMatch(e.what(), "Cannot use element() for type: struct\n"
            "{\n"
            "  int m_First;\n"
            "  enum\n"
            "  {\n"
            "    NEW,\n"
            "    FIXED,\n"
            "    BROKEN,\n"
            "    DEAD\n"
            "  } m_Second;\n"
            "  double m_Third;\n"
            "}"));
    }

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
