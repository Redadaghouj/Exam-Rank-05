#include "bigint.hpp"
#include <algorithm>
#include <cstdlib>

// ---------------------------
// Helpers
// ---------------------------
void bigint::normalize() {
    size_t pos = digits.find_first_not_of('0');
    if (pos == std::string::npos)
        digits = "0";
    else
        digits.erase(0, pos);
}

// ---------------------------
// Constructors / Assignment
// ---------------------------
bigint::bigint() : digits("0") {}

bigint::bigint(const bigint& other) : digits(other.digits) {}

bigint::bigint(unsigned long n) {
    if (n == 0) {
        digits = "0";
        return;
    }
    while (n > 0) {
        digits.push_back((n % 10) + '0');
        n /= 10;
    }
    std::reverse(digits.begin(), digits.end());
}

bigint& bigint::operator=(const bigint& other) {
    if (this != &other)
        digits = other.digits;
    return *this;
}

// ---------------------------
// Addition
// ---------------------------
bigint bigint::operator+(const bigint& other) const {
    int carry = 0;
    int i = digits.size() - 1;
    int j = other.digits.size() - 1;
    std::string res;

    while (i >= 0 || j >= 0 || carry) {
        int x = (i >= 0) ? digits[i] - '0' : 0;
        int y = (j >= 0) ? other.digits[j] - '0' : 0;
        int sum = x + y + carry;
        carry = sum / 10;
        res.push_back((sum % 10) + '0');
        --i;
        --j;
    }

    std::reverse(res.begin(), res.end());
    bigint result;
    result.digits = res;
    result.normalize();
    return result;
}

bigint& bigint::operator+=(const bigint& other) {
    *this = *this + other;
    return *this;
}

// ---------------------------
// Increment
// ---------------------------
bigint& bigint::operator++() {
    *this += bigint(1);
    return *this;
}

bigint bigint::operator++(int) {
    bigint tmp(*this);
    ++(*this);
    return tmp;
}

// ---------------------------
// Decimal Shifts
// ---------------------------
bigint bigint::operator<<(unsigned long n) const {
    if (digits == "0") return *this;
    bigint res(*this);
    res.digits.append(n, '0');
    return res;
}

bigint& bigint::operator<<=(unsigned long n) {
    if (digits != "0") digits.append(n, '0');
    return *this;
}

bigint bigint::operator>>(unsigned long n) const {
    bigint res(*this);
    return res >>= bigint(n);
}

bigint& bigint::operator>>=(unsigned long n) {
    return *this >>= bigint(n);
}

bigint bigint::operator<<(const bigint& other) const {
    unsigned long n = atol(other.digits.c_str());
    return *this << n;
}

bigint& bigint::operator<<=(const bigint& other) {
    unsigned long n = atol(other.digits.c_str());
    return *this <<= n;
}

bigint bigint::operator>>(const bigint& other) const {
    bigint res(*this);
    res >>= other;
    return res;
}

bigint& bigint::operator>>=(const bigint& other) {
    unsigned long n = atol(other.digits.c_str());
    if (n >= digits.size())
        digits = "0";
    else
        digits.erase(digits.size() - n);
    return *this;
}

// ---------------------------
// Comparisons
// ---------------------------
bool bigint::operator==(const bigint& other) const { return digits == other.digits; }
bool bigint::operator!=(const bigint& other) const { return digits != other.digits; }
bool bigint::operator<(const bigint& other) const {
    if (digits.size() != other.digits.size())
        return digits.size() < other.digits.size();
    return digits < other.digits;
}
bool bigint::operator<=(const bigint& other) const { return !(*this > other); }
bool bigint::operator>(const bigint& other) const { return other < *this; }
bool bigint::operator>=(const bigint& other) const { return !(*this < other); }

// ---------------------------
// Output
// ---------------------------
std::ostream& operator<<(std::ostream& os, const bigint& n) {
    os << n.digits;
    return os;
}

// ---------------------------
// Conversion
// ---------------------------
unsigned long to_uint(const bigint& other) {
    return atol(other.digits.c_str());
}
