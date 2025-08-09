#pragma once

#include <cstddef>  // size_t
#include <utility>  // enable_if_t, forward


typedef unsigned int    uint;
typedef unsigned char   uchar;

using SizeStr = uint;

struct Str 
{
    unsigned int size_alloc = 0;
    unsigned int size_str = 0;
    char* mem = nullptr;

    constexpr
    Str() = default;
    Str(const char *c_str);
    explicit
    Str(unsigned int size, char set_char);
    Str(const Str& other);
    Str(Str&& other);

    template <  typename... Args,
                typename = std::enable_if_t<(sizeof...(Args) > 1)>>
    Str(Args&&... args)
    {
        (*this += ... += Str(std::forward<Args>(args)));
    }

    ~Str();

    Str& operator=(const Str& other);
    Str& operator=(Str&& other);

    bool operator==(const Str& other) const;
    bool operator<(const Str& other) const;
    bool operator>=(const Str& other) const;

    Str& operator+=(Str&& rhs);
    Str& operator+=(const Str& rhs);
    // Str& operator+=(char ch);
    Str& operator+=(const char* c_str);

    Str operator+(const Str& rhs);
    Str operator+(const char* c_str);



    char& operator[](size_t index); // mutable char
    const char operator[](size_t index) const; // read only



    char* data() const;
    unsigned int size() const;
    unsigned int capacity() const;
    void pop_back();
    void reserve(unsigned int new_alloc_size);
    bool has_size() const;
    bool has_capacity() const;
    void fill_alloc(char fill_char);
    void clear();

    void allocate(unsigned int size_to_alloc);
    void reallocate(unsigned int new_size_alloc);
    void release_mem();
    // Ownership moved. Robbed of all assets. No mem release.
    void rob();


    /** Append chars to first string. realloc if necessary */
    void append(const Str& str_to_append);
    /** Shrink the string to specified starting index and size. Returns itself in cut state. */
    Str& cut_to_substr(unsigned int pos, unsigned int new_size);
    /** Returns new Str as specified substring. */
    Str substr(unsigned int pos, unsigned int new_size);

    Str indent_space(unsigned int indent_count);

    const char* to_c_str();

    /** Prints string and new line */
    void print() const;
    /** Prints quotes-enclosed string and new line */
    void print_in_quotes();

    Str& to_str()
    {
        return *this;
    }



    enum class FloatRep {
        Fixed, 
        Scientific,
    };

    /** ASCII char to Str */
    static Str CH(char ch);
    /** Signed Integer to Str */
    static Str SI(long long s_int);
    /** Unsigned Integer to Str */
    static Str UI(unsigned long long u_int);
    /** Float to Str */
    static Str FL(float fl, uchar decimals, Str::FloatRep float_representation);
    /** Double to Str */
    static Str DB(double db, int decimals);

    template <typename T>
    static Str Num(T num)
    {
        return Str::FL(num, (uchar)2, Str::FloatRep::Fixed);
    }
};
