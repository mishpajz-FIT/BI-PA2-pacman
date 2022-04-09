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

class CDataType {
protected:
    virtual bool isEqualTo(const CDataType & rhs) const = 0;

    static void indentStream(ostream & stream, size_t level) {
        while (level--) {
            stream << "  ";
        }
    }

public:
    CDataType() { }

    virtual ~CDataType() { }

    virtual size_t getSize() const = 0;

    bool operator == (const CDataType & rhs) const {
        return isEqualTo(rhs);
    }

    bool operator != (const CDataType & rhs) const {
        return !(isEqualTo(rhs));
    }

    virtual CDataType * clone() const = 0;

    virtual void print(ostream & stream, size_t level = 0) const = 0;

    friend ostream & operator << (ostream & stream, const CDataType & rhs) {
        rhs.print(stream);
        return stream;
    }
};

class CDataTypeInt : public CDataType {
protected:
    bool isEqualTo(const CDataType & rhs) const override {
        return (dynamic_cast<const CDataTypeInt *>(&rhs) != nullptr);
    }

    void print(ostream & stream, size_t level = 0) const override {
        CDataType::indentStream(stream, level);
        stream << "int";
    }

    CDataTypeInt * clone() const override {
        return (new CDataTypeInt(*this));
    }

public:
    size_t getSize() const override {
        return 4;
    }
};

class CDataTypeDouble : public CDataType {
protected:
    bool isEqualTo(const CDataType & rhs) const override {
        return (dynamic_cast<const CDataTypeDouble *>(&rhs) != nullptr);
    }

    void print(ostream & stream, size_t level = 0) const override {
        CDataType::indentStream(stream, level);
        stream << "double";
    }

    CDataTypeDouble * clone() const override {
        return (new CDataTypeDouble(*this));
    }

public:
    size_t getSize() const override {
        return 8;
    }
};

class CDataTypeEnum : public CDataType {
private:
    vector<string> values;

protected:
    bool isEqualTo(const CDataType & rhs) const override {
        const CDataTypeEnum * ptrRhs = dynamic_cast<const CDataTypeEnum *>(&rhs);
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

    void print(ostream & stream, size_t level = 0) const override {
        CDataType::indentStream(stream, level);
        stream << "enum\n";

        CDataType::indentStream(stream, level);
        stream << "{\n";

        for (size_t i = 0; i < values.size(); i++) {
            CDataType::indentStream(stream, level + 1);
            stream << values[i];
            if (i + 1 < values.size()) {
                stream << ",";
            }
            stream << "\n";
        }

        CDataType::indentStream(stream, level);
        stream << "}";
    }

    CDataTypeEnum * clone() const override {
        return (new CDataTypeEnum(*this));
    }

public:
    size_t getSize() const override {
        return 4;
    }

    CDataTypeEnum & add(const string & value) {
        if (find(values.begin(), values.end(), value) != values.end()) {
            throw std::invalid_argument("Duplicate enum value: " + value);
        }

        values.emplace_back(value);
        return (*this);
    }
};

class CDataTypeStruct : public CDataType {
private:
    vector<pair<string, CDataType *>> values;

    void deallocValues() {
        for (auto & v : values) {
            delete v.second;
        }
        values.clear();
    }

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

    void print(ostream & stream, size_t level = 0) const override {
        CDataType::indentStream(stream, level);
        stream << "struct\n";

        CDataType::indentStream(stream, level);
        stream << "{\n";

        for (const auto & v : values) {
            v.second->print(stream, level + 1);
            stream << " " << v.first << ";\n";
        }

        CDataType::indentStream(stream, level);
        stream << "}";

    }

    CDataTypeStruct * clone() const override {
        return (new CDataTypeStruct(*this));
    }

public:
    CDataTypeStruct() : CDataType() { }

    CDataTypeStruct(const CDataTypeStruct & toCopy) : CDataType() {
        for (auto & v : toCopy.values) {
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

        deallocValues();

        for (auto & v : toCopy.values) {
            values.push_back(make_pair(v.first, v.second->clone()));
        }

        return (*this);
    }

    size_t getSize() const override {
        size_t sum = 0;
        for (auto & v : values) {
            sum += v.second->getSize();
        }
        return sum;
    }

    CDataTypeStruct & addField(const string & name, const CDataType & value) {
        auto iter = findInValues(name);

        if (iter != values.end()) {
            throw std::invalid_argument("Duplicate field: " + name);
        }

        CDataType * valueCopy = value.clone();
        values.push_back(make_pair(name, valueCopy));

        return (*this);
    }

    const CDataType & field(const string & name) const {
        auto iter = findInValues(name);

        if (iter == values.end()) {
            throw std::invalid_argument("Unknown field: " + name);
        }

        return (*((*iter).second));
    }
};

#ifndef __PROGTEST__
static bool whitespaceMatch(const string & a, const string & b) {
    string prepStringA = a;
    string prepStringB = b;

    prepStringA.erase(remove_if(prepStringA.begin(), prepStringA.end(), [ ](char & c) { return isspace(c); }));
    prepStringB.erase(remove_if(prepStringB.begin(), prepStringB.end(), [ ](char & c) { return isspace(c); }));

    if (prepStringA.length() != prepStringB.length()) {
        return false;
    }

    for (size_t i = 0; i < prepStringA.length(); i++) {
        if (prepStringA[i] != prepStringB[i]) {
            return false;
        }
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
    assert(a.field("m_Status") == CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("DEAD"));
    assert(a.field("m_Status") != CDataTypeEnum().add("NEW").add("BROKEN").add("FIXED").add("DEAD"));
    assert(a != CDataTypeInt());
    assert(whitespaceMatch(a.field("m_Status"), "enum\n"
        "{\n"
        "  NEW,\n"
        "  FIXED,\n"
        "  BROKEN,\n"
        "  DEAD\n"
        "}"));

    CDataTypeStruct aOld = a;
    b.addField("m_Other", CDataTypeDouble());

    a.addField("m_Sum", CDataTypeInt());

    assert(a != aOld);
    assert(a != c);
    assert(aOld == c);
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
        "  double m_Other;\n"
        "}"));

    c.addField("m_Another", a.field("m_Status"));

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
        "  enum\n"
        "  {\n"
        "    NEW,\n"
        "    FIXED,\n"
        "    BROKEN,\n"
        "    DEAD\n"
        "  } m_Another;\n"
        "}"));

    d.addField("m_Another", a.field("m_Ratio"));

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
        "  double m_Another;\n"
        "}"));

    assert(a.getSize() == 20);
    assert(b.getSize() == 24);
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
