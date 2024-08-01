#pragma once
#include <iostream>
#include <time.h>
using namespace std;

class BigInteger
{
private:
public:
    // Constructor initializes a BigInteger with the default value of "0".
    // This is used when a BigInteger object is created without specifying an initial value.
    string value;
    BigInteger() : value("0") {}

    // Constructor initializes a BigInteger with a given string value.
    // This is used when a BigInteger object is created with a specific initial value.
    BigInteger(const string& str) : value(str) {}

    // Overloaded addition operator (+) for BigInteger objects.
    // Uses the helper function 'add' to perform addition on the underlying string representations.
    BigInteger operator+(const BigInteger& other) const
    {
        return BigInteger(add(value, other.value));
    }

    // Overloaded subtraction operator (-) for BigInteger objects.
    // Uses the helper function 'subtract' to perform subtraction on the underlying string representations.
    BigInteger operator-(const BigInteger& other) const
    {
        return BigInteger(subtract(value, other.value));
    }

    // Overloaded multiplication operator (*) for BigInteger objects.
    // Uses the helper function 'multiply' to perform multiplication on the underlying string representations.
    BigInteger operator*(const BigInteger& other) const
    {
        return BigInteger(multiply(value, other.value));
    }

    // Computes the power of the BigInteger with a given exponent.
    // Utilizes fast exponentiation using the binary exponentiation algorithm.
    BigInteger power(int exponent) const
    {
        // If the exponent is 0, the result is 1.
        if (exponent == 0)
        {
            return BigInteger("1");
        }

        // Initialize result to 1 and base to the current value of the BigInteger.
        BigInteger result("1");
        BigInteger base(value);

        // Binary exponentiation algorithm
        while (exponent > 0)
        {
            // If the current bit of the exponent is 1, multiply result by base.
            if (exponent % 2 == 1)
            {
                result = result * base;
            }

            // Square the base for the next iteration.
            base = base * base;
            // Move to the next bit of the exponent.
            exponent /= 2;
        }

        return result;
    }

    // Helper function to add two BigIntegers represented as strings.
    // Performs addition digit by digit, handling carry as necessary.
    string add(const string& a, const string& b) const
    {
        string result;
        int carry = 0;

        // Iterate from the least significant digit to the most significant digit.
        for (int i = a.length() - 1, j = b.length() - 1; i >= 0 || j >= 0 || carry; i--, j--)
        {
            // Sum the current digits and the carry.
            int sum = carry;
            if (i >= 0)
                sum += a[i] - '0';
            if (j >= 0)
                sum += b[j] - '0';

            // Calculate the new carry and the current digit.
            carry = sum / 10;
            sum %= 10;

            // Append the digit to the result.
            result.push_back(sum + '0');
        }

        // Reverse the result string to obtain the correct order.
        reverse(result);

        return result;
    }

    // Helper function to subtract two BigIntegers represented as strings.
    // Performs subtraction digit by digit, handling borrow as necessary.
    string subtract(const string& a, const string& b) const
    {
        string result;
        int borrow = 0;

        // Iterate from the least significant digit to the most significant digit.
        for (int i = a.length() - 1, j = b.length() - 1; i >= 0 || j >= 0; i--, j--)
        {
            // Difference of the current digits and the borrow.
            int diff = borrow;
            if (i >= 0)
                diff += a[i] - '0';
            if (j >= 0)
                diff -= b[j] - '0';

            // Adjust the borrow for the next iteration.
            if (diff < 0)
            {
                diff += 10;
                borrow = -1;
            }
            else
            {
                borrow = 0;
            }

            // Append the digit to the result.
            result.push_back(diff + '0');
        }

        // Remove leading zeros from the result.
        while (result.length() > 1 && result.back() == '0')
        {
            result.pop_back();
        }

        // Reverse the result string to obtain the correct order.
        reverse(result);

        return result;
    }

    // Helper function to multiply two BigIntegers represented as strings.
    // Uses the standard multiplication algorithm with carry handling.
    string multiply(const string& a, const string& b) const
    {
        // Initialize result with zeros, with a length equal to the sum of lengths of a and b.
        string result(a.length() + b.length(), '0');

        // Iterate through digits of a in reverse order.
        for (int i = a.length() - 1; i >= 0; i--)
        {
            int carry = 0;

            // Iterate through digits of b in reverse order.
            for (int j = b.length() - 1; j >= 0; j--)
            {
                // Calculate the product of the current digits and the carry.
                int product = (a[i] - '0') * (b[j] - '0') + (result[i + j + 1] - '0') + carry;

                // Update carry for the next iteration.
                carry = product / 10;

                // Update the current digit in the result.
                result[i + j + 1] = product % 10 + '0';
            }

            // Add the remaining carry to the most significant digit of the result.
            result[i] += carry;
        }

        // Remove leading zeros from the result.
        if (result.front() == '0')
        {
            result.erase(result.begin());
        }

        return result;
    }

    // Helper function to reverse a string.
    // Used to reverse the order of digits in the result.
    void reverse(string& str) const
    {
        int left = 0;
        int right = str.length() - 1;

        // Swap characters from the outer ends towards the center.
        while (left < right)
        {
            swap(str[left], str[right]);
            left++;
            right--;
        }
    }
    // Overloaded modulo operator (%) for BigInteger objects.
    // Computes the remainder of the division between the current BigInteger and another BigInteger.
    BigInteger operator%(const BigInteger& other) const
    {
        // Declare variables to store the quotient and remainder.
        BigInteger quotient, remainder;
        // Call the divide helper function to perform division and obtain the remainder.
        divide(*this, other, quotient, remainder);
        // Return the computed remainder.
        return remainder;
    }

    // Overloaded division operator (/) for BigInteger objects.
    // Computes the quotient of the division between the current BigInteger and another BigInteger.
    BigInteger operator/(const BigInteger& other) const
    {
        // Declare variables to store the quotient and remainder.
        BigInteger quotient, remainder;
        // Call the divide helper function to perform division and obtain the quotient.
        divide(*this, other, quotient, remainder);
        // Return the computed quotient.
        return quotient;
    }

    // Helper function for division operation.
    // Divides the numerator by the denominator and computes both the quotient and remainder.
    void divide(const BigInteger& numerator, const BigInteger& denominator, BigInteger& quotient, BigInteger& remainder) const
    {
        // Create a temporary copy of the numerator.
        BigInteger tempNumerator = numerator;
        // Initialize the quotient to "0".
        quotient = "0";

        // Perform long division until the numerator is less than the denominator.
        while (tempNumerator >= denominator)
        {
            // Subtract the denominator from the numerator.
            tempNumerator = tempNumerator - denominator;
            // Increment the quotient by 1.
            BigInteger val1("1");
            quotient = quotient + val1;
        }

        // After the loop, the remainder is stored in tempNumerator.
        remainder = tempNumerator;
    }

    // Overloaded less than operator (<) for BigInteger objects.
    // Compares the current BigInteger with another BigInteger to check if it is less than.
    bool operator<(const BigInteger& other) const
    {
        // Compare the lengths of the string representations.
        if (value.length() < other.value.length())
        {
            return true;
        }
        if (value.length() > other.value.length())
        {
            return false;
        }

        // If lengths are equal, compare digits from most significant to least significant.
        for (int i = 0; i < value.length(); i++)
        {
            if (value[i] > other.value[i])
            {
                return false;
            }
            else if (value[i] < other.value[i])
            {
                return true;
            }
        }

        // If all digits are equal, the numbers are equal.
        return false;
    }

    // Overloaded less than or equal to operator (<=) for BigInteger objects.
    // Compares the current BigInteger with another BigInteger to check if it is less than or equal to.
    bool operator<=(const BigInteger& other) const
    {
        // Compare the lengths of the string representations.
        if (value.length() < other.value.length())
        {
            return true;
        }
        if (value.length() > other.value.length())
        {
            return false;
        }

        // If lengths are equal, compare digits from most significant to least significant.
        for (int i = 0; i < value.length(); i++)
        {
            if (value[i] > other.value[i])
            {
                return false;
            }
            else if (value[i] < other.value[i])
            {
                return true;
            }
        }

        // If all digits are equal, the numbers are equal.
        return true;
    }

    // Overloaded equal to operator (==) for BigInteger objects.
    // Compares the current BigInteger with another BigInteger to check if it is equal to.
    bool operator==(const BigInteger& other) const
    {
        // Compare the lengths of the string representations.
        if (value.length() < other.value.length())
        {
            return false;
        }
        if (value.length() > other.value.length())
        {
            return false;
        }

        // If lengths are equal, compare digits from most significant to least significant.
        for (int i = 0; i < value.length(); i++)
        {
            if (value[i] > other.value[i])
            {
                return false;
            }
            else if (value[i] < other.value[i])
            {
                return false;
            }
        }

        // If all digits are equal, the numbers are equal.
        return true;
    }

    // Overloaded not equal to operator (!=) for BigInteger objects.
    // Inverts the result of the equal to operator.
    bool operator!=(const BigInteger& other) const
    {
        // Invert the result of the equal to operator.
        return !(*this == other);
    }

    // Overloaded greater than or equal to operator (>=) for BigInteger objects.
    // Inverts the result of the less than operator.
    bool operator>=(const BigInteger& other) const
    {
        // Invert the result of the less than operator.
        if (value.length() < other.value.length())
        {
            return false;
        }
        if (value.length() > other.value.length())
        {
            return true;
        }

        // If lengths are equal, compare digits from most significant to least significant.
        for (int i = 0; i < value.length(); i++)
        {
            if (value[i] > other.value[i])
            {
                return true;
            }
            else if (value[i] < other.value[i])
            {
                return false;
            }
        }

        // If all digits are equal, the numbers are equal.
        return true;
    }

    // Overloaded greater than operator (>) for BigInteger objects.
    // Inverts the result of the less than or equal to operator.
    bool operator>(const BigInteger& other) const
    {
        // Invert the result of the less than or equal to operator.
        if (value.length() < other.value.length())
        {
            return false;
        }
        if (value.length() > other.value.length())
        {
            return true;
        }

        // If lengths are equal, compare digits from most significant to least significant.
        for (int i = 0; i < value.length(); i++)
        {
            if (value[i] > other.value[i])
            {
                return true;
            }
            else if (value[i] < other.value[i])
            {
                return false;
            }
        }

        // If all digits are equal, the numbers are equal.
        return false;
    }

    // Overloaded assignment operator (=) for BigInteger objects.
    // Assigns the value of another BigInteger to the current BigInteger.
    BigInteger& operator=(const BigInteger& other)
    {
        // Check for self-assignment to avoid unnecessary work.
        if (this != &other)
        {
            // Assign the value of the other BigInteger to the current BigInteger.
            value = other.value;
        }
        // Return the reference to the current object.
        return *this;
    }

    // Overloaded assignment operator (=) for assigning a string to a BigInteger.
    // Assigns the value of a string to the current BigInteger.
    BigInteger& operator=(const string& other)
    {
        // Assign the value of the string to the current BigInteger.
        value = other;
        // Return the reference to the current object.
        return *this;
    }

    // Converts the BigInteger to its string representation.
    // Returns the string representation of the BigInteger.
    std::string toString() const
    {
        return value;
    }
    static BigInteger generateRandomBigInteger(int numDigits)
    {
        BigInteger val("123");
        int a = rand() % numDigits + 1;

        for (int i = 0; i < a; i++)
        {
            int b = rand() % numDigits;
            string newval = std::to_string(b);
            BigInteger new_val(newval);
            val = val + new_val;
        }
        a = rand() % numDigits;

        for (int i = 0; i < a; i++)
        {
            int b = rand() % numDigits;
            string newval = std::to_string(b);
            BigInteger new_val(newval);
            val = val * new_val;
        }

        return val;
    }

    // Displays the value of the BigInteger.
    // Prints the string representation of the BigInteger to the console.
    void display() const
    {
        std::cout << value << endl;
    }
};