#pragma once

#include <iostream>
#include <string>

class bigint {
private:
    std::string digits;  // Most significant digit first

    void normalize();    // Remove leading zeros

public:
    // Constructors
    bigint();
    bigint(const bigint& other);
    bigint(unsigned long n);

    // Assignment
    bigint& operator=(const bigint& other);

    // Arithmetic
    bigint operator+(const bigint& other) const;
    bigint& operator+=(const bigint& other);

    // Increment
    bigint& operator++();    // prefix
    bigint operator++(int);  // postfix

    // Shifts (decimal shifts)
    bigint operator<<(unsigned long n) const;
    bigint& operator<<=(unsigned long n);
    bigint operator>>(unsigned long n) const;
    bigint& operator>>=(unsigned long n);

    bigint operator<<(const bigint& other) const;
    bigint& operator<<=(const bigint& other);
    bigint operator>>(const bigint& other) const;
    bigint& operator>>=(const bigint& other);

    // Comparisons
    bool operator==(const bigint& other) const;
    bool operator!=(const bigint& other) const;
    bool operator<(const bigint& other) const;
    bool operator<=(const bigint& other) const;
    bool operator>(const bigint& other) const;
    bool operator>=(const bigint& other) const;

    // I/O
    friend std::ostream& operator<<(std::ostream& os, const bigint& other);

    // Conversion to unsigned long
    friend unsigned long to_uint(const bigint& other);
};
