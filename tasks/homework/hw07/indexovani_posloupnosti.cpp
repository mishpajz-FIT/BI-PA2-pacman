/**
 * @file indexovani_posloupnosti.cpp
 * @author Michal Dobeš
 * @date 2022-04-18
 *
 * @brief Index implementation for sequence containers, which allows for search of subsequences
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
#include <numeric>
#include <optional>
#include <variant>
#include <any>
using namespace std;
#endif /* __PROGTEST__ */

/**
 * @brief Implement Knuth-Morris-Pratt algorithm for searching all occurences of sequence inside TContainer
 *
 * @tparam TContainer Type of container, required to have random access using squared bracket notation
 * @tparam std::less<typename TContainer::value_type> Comparison function
 */
template <typename TContainer, typename TComparator = std::less<typename TContainer::value_type>>
class KnuthMorrisPratt {

    TComparator comparator;

    /**
     * @brief Equal based on supplied comparison function
     *
     * @param lhs TContainer::value_type value
     * @param rhs TContainer::value_type value
     * @return true Are equal
     * @return false Are not equal
     */
    bool equal(const typename TContainer::value_type & lhs, const typename TContainer::value_type & rhs) const {
        return (!comparator(lhs, rhs) && !comparator(rhs, lhs));
    }

    /**
     * @brief Create partial match table
     *
     * Implementation based on source: https://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm
     *
     * For each char i in pattern calculates longest prefix (prefix without the substring itself) in pattern[0...i] which is also a suffix in this pattern
     *
     * @param pattern Pattern to create table for
     */
    vector<int> table(const TContainer & pattern) const {
        vector<int> patternTable;
        patternTable.resize(pattern.size() + 2);

        patternTable[0] = -1; //< -1 Signalizes first character of prefix

        int i = 0;
        int j = 1;
        while (j < (long)(pattern.size())) {
            if (equal(pattern[i], pattern[j])) {
                patternTable[j] = patternTable[i]; //< If prefix and suffix are matching, use already calcualted values;
            } else {
                patternTable[j] = i; //< i is the char until where prefix was same as suffix (threfore size of match)
                while (i >= 0 && !equal(pattern[i], pattern[j])) { //< Until not found prefix that matches, return back
                    i = patternTable[i];
                }
            }
            j++;
            i++;
        }
        patternTable[j] = i;

        return patternTable;
    }

public:

    KnuthMorrisPratt() { }

    KnuthMorrisPratt(TComparator c) : comparator(c) { }

    /**
     * @brief Search for indexes in source which contain pattern
     *
     * Implementation based on source: https://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm
     *
     * @param source Indexable container in which to search
     * @param pattern Pattern to search for
     * @return set<size_t> Indexes containing pattern
     */
    set<size_t> calculate(const TContainer & source, const TContainer & pattern) const {

        vector<int> patternTable = table(pattern); //< Fail function, create partial match table
        // Because of pattern table, on mismatch knows to "move" the pattern more on next matching trial

        set<size_t> results;

        int i = 0;
        int j = 0;
        while (i < (long)(source.size())) {
            if (equal(pattern[j], source[i])) { //< Compare characters and continue if they match
                i++;
                j++;
                if (j == (long)(pattern.size())) { //< Pattern was found
                    results.emplace(i - j); //< Insert index
                    j = patternTable[pattern.size()];
                }
            } else { //< On mismatch, continue matching based on pattern table (eg. if last 3 characters matched and they are same as first 3 characters of pattern, continue matching from fourth character of pattern, not from the beggining, this is the information stored in patternTable)
                j = patternTable[j];
                if (j < 0) { //< If on first character of pattern, move to the next one (first one certainly won't match, because it will be the same as char that was checked just now)
                    i++;
                    j++;
                }
            }
        }
        return results;
    }
};

/**
 * @brief Index for sequence containers, which allows for search of subsequences
 *
 * @tparam TSequence Sequence container containing elements, needs to have iterator
 * @tparam std::less<typename TSequence::value_type> Comparator for elements of sequence
 */
template <typename TSequence, typename TComparator = std::less<typename TSequence::value_type>>
class CIndex {
private:
    vector<typename TSequence::value_type> indexes; //< Sequence elements copied into vector, which is indexable
    TComparator comparator;

public:
    CIndex(const TSequence & s) {
        for (const auto & c : s) { //< Copy elements into vector
            indexes.emplace_back(c);
        }
    }

    CIndex(const TSequence & s, const TComparator & c) : comparator(c) {
        for (const auto & c : s) { //< Copy elements into vector
            indexes.emplace_back(c);
        }
    }

    set<size_t> search(const TSequence & searchFor) {
        if (searchFor.size() == 0) { //< If size of searchFor subsequence is 0, return all indexes
            set<size_t> resultSet;
            for (size_t i = 0; i < indexes.size(); i++) {
                resultSet.emplace(i);
            }
            return resultSet;
        }

        vector<typename TSequence::value_type> pattern; //< Search for subsequence converted into vector

        for (const auto & c : searchFor) { //< Copy searchFor elements into vector
            pattern.emplace_back(c);
        }

        KnuthMorrisPratt<vector<typename TSequence::value_type>, TComparator> kmp(comparator); //< Find subsequences using Knuth-Morris-Pratt algorithm
        return kmp.calculate(indexes, pattern);
    }
};

template<>
class CIndex<string> {
    // Specialization of CIndex for string and implicit comparator, because string is already indexable so it doesnt need to be preprocessed
private:
    string sequence;

public:
    CIndex(const string & s) : sequence(s) { } //< Copy string

    set<size_t> search(const string & searchFor) {
        set<size_t> result;

        if (searchFor.size() == 0) { //< If substring to searchFor is empty, return all indexes
            set<size_t> resultSet;
            for (size_t i = 0; i < sequence.size(); i++) {
                resultSet.emplace(i);
            }
            return resultSet;
        }

        KnuthMorrisPratt<string> kmp;
        return kmp.calculate(sequence, searchFor); //< Find subsequences using Knuth-Morris-Pratt algorithm
    }
};

#ifndef __PROGTEST__  
class CStrComparator {
public:
    CStrComparator(bool caseSensitive) : m_CaseSensitive(caseSensitive) { }

    bool operator () (const string & a, const string & b) const {
        return m_CaseSensitive ? strcasecmp(a.c_str(), b.c_str()) < 0 : a < b;
    }

    bool m_CaseSensitive;
};

bool upperCaseCompare(const char & a, const char & b) {
    return toupper(a) < toupper(b);
}

int main(void) {
    CIndex <string> t0("abcabcabc");
    set<size_t> r0 = t0.search("a");
    assert(r0 == (set<size_t> { 0, 3, 6 }));
    set<size_t> r1 = t0.search("abc");
    assert(r1 == (set<size_t> { 0, 3, 6 }));
    set<size_t> r2 = t0.search("abcabc");
    assert(r2 == (set<size_t> { 0, 3 }));

    CIndex <string> t1("abcababc");
    set<size_t> r3 = t1.search("a");
    assert(r3 == (set<size_t> { 0, 3, 5 }));
    set<size_t> r4 = t1.search("abc");
    assert(r4 == (set<size_t> { 0, 5 }));
    set<size_t> r5 = t1.search("abcabc");
    assert(r5 == (set<size_t> {  }));

    CIndex <string> t2("kokokokoskokosokos");
    set<size_t> r6 = t2.search("kos");
    assert(r6 == (set<size_t> { 6, 11, 15 }));
    set<size_t> r7 = t2.search("kokos");
    assert(r7 == (set<size_t> { 4, 9 }));

    CIndex <string> t3("aaaaaaau aaauaaaau");
    set<size_t> r8 = t3.search("aa");
    assert(r8 == (set<size_t> { 0, 1, 2, 3, 4, 5, 9, 10, 13, 14, 15 }));
    set<size_t> r9 = t3.search("aaa");
    assert(r9 == (set<size_t> { 0, 1, 2, 3, 4, 9, 13, 14 }));
    set<size_t> r10 = t3.search("aaaa");
    assert(r10 == (set<size_t> { 0, 1, 2, 3, 13 }));

    CIndex <string> t4("automatIc authentication automotive auTOmation raut");
    set<size_t> r11 = t4.search("auto");
    assert(r11 == (set<size_t> { 0, 25 }));
    set<size_t> r12 = t4.search("aut");
    assert(r12 == (set<size_t> { 0, 10, 25, 48 }));
    set<size_t> r13 = t4.search("tic");
    assert(r13 == (set<size_t> { 16 }));
    set<size_t> r14 = t4.search("trunk");
    assert(r14 == (set<size_t> {  }));
    set<size_t> r15 = t4.search("a");
    assert(r15 == (set<size_t> { 0, 5, 10, 19, 25, 36, 41, 48 }));
    set<size_t> r16 = t4.search("");
    assert(r16 == (set<size_t> { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 }));

    CIndex <string, bool(*)(const char &, const char &)> t5("automatIc authentication automotive auTOmation raut", upperCaseCompare);
    set<size_t> r17 = t5.search("auto");
    assert(r17 == (set<size_t> { 0, 25, 36 }));
    set<size_t> r18 = t5.search("aut");
    assert(r18 == (set<size_t> { 0, 10, 25, 36, 48 }));
    set<size_t> r19 = t5.search("tic");
    assert(r19 == (set<size_t> { 6, 16 }));
    set<size_t> r20 = t5.search("trunk");
    assert(r20 == (set<size_t> {  }));
    set<size_t> r21 = t5.search("a");
    assert(r21 == (set<size_t> { 0, 5, 10, 19, 25, 36, 41, 48 }));
    set<size_t> r22 = t5.search("");
    assert(r22 == (set<size_t> { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 }));

    CIndex <list<string>, CStrComparator> t6(list<string>{ "Hello", "world", "test", "this", "foo", "TEsT", "this", "done" }, CStrComparator(false));
    set<size_t> r23 = t6.search(list<string>{"test", "this", "foo"});
    assert(r23 == (set<size_t> { 2 }));
    set<size_t> r24 = t6.search(list<string>{"test", "this"});
    assert(r24 == (set<size_t> { 2 }));

    CIndex <list<string>, CStrComparator> t7(list<string>{ "Hello", "world", "test", "this", "foo", "TEsT", "this", "done" }, CStrComparator(true));
    set<size_t> r25 = t7.search(list<string>{"test", "this", "foo"});
    assert(r25 == (set<size_t> { 2 }));
    set<size_t> r26 = t7.search(list<string>{"test", "this"});
    assert(r26 == (set<size_t> { 2, 5 }));
    return 0;
}
#endif /* __PROGTEST__ */
