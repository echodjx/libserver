//
// Created by echo-djx on 2021/8/23.
//
# pragma once

#include <string.h>
#include <iosfwd>    // for ostream forward-declaration

class StringPiece {
private:
    const char *ptr_;
    int length_;

public:
    // We provide non-explicit singleton constructors so users can pass
    // in a "const char*" or a "string" wherever a "StringPiece" is
    // expected.
    StringPiece()
            : ptr_(NULL), length_(0) {}

    StringPiece(const char *str)
            : ptr_(str), length_(static_cast<int>(strlen(ptr_))) {}

    StringPiece(const unsigned char *str)
            : ptr_(reinterpret_cast<const char *>(str)),
              length_(static_cast<int>(strlen(ptr_))) {}

    StringPiece(const std::string &str)
            : ptr_(str.data()), length_(static_cast<int>(str.size())) {}

    StringPiece(const char *offset, int len)
            : ptr_(offset), length_(len) {}

    // data() may return a pointer to a buffer with embedded NULs, and the
    // returned buffer may or may not be null terminated.  Therefore it is
    // typically a mistake to pass data() to a routine that expects a NUL
    // terminated string.  Use "as_string().c_str()" if you really need to do
    // this.  Or better yet, change your routine so it does not rely on NUL
    // termination.
    const char *data() const { return ptr_; }

    int size() const { return length_; }

    bool empty() const { return length_ == 0; }

    const char *begin() const { return ptr_; }

    const char *end() const { return ptr_ + length_; }

    void clear() {
        ptr_ = NULL;
        length_ = 0;
    }

    void set(const char *buffer, int len) {
        ptr_ = buffer;
        length_ = len;
    }

    void set(const char *str) {
        ptr_ = str;
        length_ = static_cast<int>(strlen(str));
    }

    void set(const void *buffer, int len) {
        ptr_ = reinterpret_cast<const char *>(buffer);
        length_ = len;
    }

    char operator[](int i) const { return ptr_[i]; }

    void remove_prefix(int n) {
        ptr_ += n;
        length_ -= n;
    }

    void remove_suffix(int n) {
        length_ -= n;
    }

    bool operator==(const StringPiece &x) const {
        return ((length_ == x.length_) &&
                (memcmp(ptr_, x.ptr_, length_) == 0));
    }

    bool operator!=(const StringPiece &x) const {
        return !(*this == x);
    }

#define STRINGPIECE_BINARY_PREDICATE(cmp, auxcmp)                             \
bool operator cmp (const StringPiece& x) const {                           \
int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_); \
return ((r auxcmp 0) || ((r == 0) && (length_ cmp x.length_)));          \
}

    STRINGPIECE_BINARY_PREDICATE(<, <);

    STRINGPIECE_BINARY_PREDICATE(<=, <);

    STRINGPIECE_BINARY_PREDICATE(>=, >);

    STRINGPIECE_BINARY_PREDICATE(>, >);
#undef STRINGPIECE_BINARY_PREDICATE

    int compare(const StringPiece &x) const {
        int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_);
        if (r == 0) {
            if (length_ < x.length_) r = -1;
            else if (length_ > x.length_) r = +1;
        }
        return r;
    }

    std::string as_string() const {
        return std::string(data(), size());
    }

    void CopyToString(std::string *target) const {
        target->assign(ptr_, length_);
    }

    // Does "this" start with "x"
    bool starts_with(const StringPiece &x) const {
        return ((length_ >= x.length_) && (memcmp(ptr_, x.ptr_, x.length_) == 0));
    }
};

// allow StringPiece to be logged
std::ostream& operator<<(std::ostream& o, StringPiece& piece);