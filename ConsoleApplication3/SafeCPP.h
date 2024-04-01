#pragma once
#include <vector>
#include <array>

#define UNSAFE Unsafe call is not allowed

#define new                     UNSAFE
#define malloc(s)               UNSAFE
#define calloc(n, s)            UNSAFE
#define realloc(p, s)           UNSAFE 
#define aligned_alloc(p, s)     UNSAFE

#define memcpy(...)             UNSAFE
#define memset(...)             UNSAFE
#define memcmp(...)             UNSAFE
#define memchr(...)             UNSAFE

#define strcpy(...)             UNSAFE
#define strncpy(...)            UNSAFE
#define strcat(...)             UNSAFE
#define strncat(...)            UNSAFE
#define strcmp(...)             UNSAFE
#define strncmp(...)            UNSAFE
#define strchr(...)             UNSAFE
#define strlen(...)             UNSAFE

#define sprintf(...)            UNSAFE
#define snprintf(...)           UNSAFE
#define printf_s(...)           UNSAFE
#define fprintf_s(...)          UNSAFE
#define sprintf_s(...)          UNSAFE
#define snprintf_s(...)         UNSAFE
#define vsprintf(...)           UNSAFE
#define vsnprintf(...)          UNSAFE
#define vsprintf_s(...)         UNSAFE
#define vsnprintf_s(...)        UNSAFE
#define vswprintf(...)          UNSAFE
#define vswprintf(...)          UNSAFE
#define vswprintf_s(...)        UNSAFE
#define vsnwprintf_s(...)       UNSAFE
#define swprintf(...)           UNSAFE
#define swprintf_s(...)         UNSAFE
#define snwprintf_s(...)        UNSAFE
#define vsprintf(...)           UNSAFE
#define vsnprintf(...)          UNSAFE
#define vsprintf_s(...)         UNSAFE
#define vsnprintf_s(...)        UNSAFE
#define vswprintf(...)          UNSAFE
#define vswprintf_s(...)        UNSAFE
#define vsnwprintf_s(...)       UNSAFE

#define gets(...)               UNSAFE
#define gets_s(...)             UNSAFE
#define fgets(...)              UNSAFE
#define fgetws(...)             UNSAFE
#define read(...)               UNSAFE
#define readsome(...)           UNSAFE

#define scanf(...)              UNSAFE
#define fscanf(...)             UNSAFE
#define sscanf(...)             UNSAFE
#define scanf_s(...)            UNSAFE
#define fscanf_s(...)           UNSAFE
#define sscanf_s(...)           UNSAFE
#define vscanf(...)             UNSAFE
#define vfscanf(...)            UNSAFE
#define vsscanf(...)            UNSAFE
#define vscanf_s(...)           UNSAFE
#define vfscanf_s(...)          UNSAFE
#define vsscanf_s(...)          UNSAFE

namespace std {
    enum class direction {
        forward = 1,
        backward = -1
    };

    template<class C, class T>
    class safe_iterator {
    private:
        C& container_;
        int index_;
        direction direction_;

    public:
        safe_iterator(C& container, int ind, direction dir) : container_(container), index_(ind), direction_(dir) {}

        safe_iterator<C, T>& operator++() {
            auto index = index_ + static_cast<int>(direction_);
            if (index > static_cast<int>(container_.size()) || index < -1) {
                throw std::out_of_range("Iterator out of range.");
            }
            index_ = index;
            return *this;
        }

        safe_iterator<C, T> operator++(int) {
            auto old = *this;
            operator++(); 
            return old;   
        }

        safe_iterator<C, T>& operator--() {
            auto index = index_ - static_cast<int>(direction_);
            if (index > static_cast<int>(container_.size()) || index < -1) {
                throw std::out_of_range("Iterator out of range.");
            }
            index_ = index;
            return *this;
        }

        safe_iterator<C, T> operator+(const int offset) const {
            safe_iterator<C, T> result(container_);
            result.index_ = index_ + offset * static_cast<int>(direction_);
            if (result.index_ > static_cast<int>(container_.size()) || result.index_ < -1) {
                throw std::out_of_range("Iterator out of range.");
            }
            return result;
        }

        safe_iterator<C, T> operator-(const int offset) const {
            return *this + (-offset);
        }

        safe_iterator<C, T>& operator+=(const int offset) {
            auto index = index_ + offset * static_cast<int>(direction_);
            if (index > static_cast<int>(container_.size()) || index < -1) {
                throw std::out_of_range("Iterator out of range.");
            }
            index_ = index;
            return *this;
        }

        safe_iterator<C, T>& operator-=(const int offset) {
            return *this += (-offset);
        }

        T& operator*() const {
            return container_[index_];
        }

        bool operator!=(const safe_iterator<C, T>& other) const {
            return container_ != other.container_ || index_ != other.index_;
        }

        bool operator==(const safe_iterator<C, T>& other) const {
            return container_ == other.container_ && index_ == other.index_;
        }
    };

    template<class T>
    class safe_vector : public std::vector<T> {
    public:
        using std::vector<T>::vector;
        safe_iterator<safe_vector, T> begin() {
            return safe_iterator<safe_vector, T>(*this, 0, direction::forward);
        }
        safe_iterator<const safe_vector, const T> cbegin() {
            return safe_iterator<const safe_vector, const T>(*this, 0, direction::forward);
        }
        safe_iterator<safe_vector, T> rbegin() {
            return safe_iterator<safe_vector, T>(*this, this->size() == 0 ? 0 : this->size() - 1, direction::backward);
        }
        safe_iterator<const safe_vector, const T> crbegin() {
            return safe_iterator<const safe_vector, const T>(*this, this->size() == 0 ? 0 : this->size() - 1, direction::backward);
        }
        safe_iterator<safe_vector, T> end() {
            return safe_iterator<safe_vector, T>(*this, this->size(), direction::forward);
        }
        safe_iterator<const safe_vector, const T> cend() {
            return safe_iterator<const safe_vector, const T>(*this, this->size(), direction::forward);
        }
        safe_iterator<safe_vector, T> rend() {
            return safe_iterator<safe_vector, T>(*this, -1, direction::backward);
        }
        safe_iterator<const safe_vector, const T> crend() {
            return safe_iterator<const safe_vector, const T>(*this, -1, direction::backward);
        }
        T& operator[](const size_t index) {
            return this->at(index);
        }
        const T& operator[](const size_t index) const {
            return this->at(index);
        }
    };

#define vector safe_vector

    template<class T, size_t S>
    class safe_array : public std::array<T, S> {
    public:
        safe_iterator<safe_array, T> begin() {
            return safe_iterator<safe_array, T>(*this, 0, direction::forward);
        }
        safe_iterator<const safe_array, const T> cbegin() {
            return safe_iterator<const safe_array, const T>(*this, 0, direction::forward);
        }
        safe_iterator<safe_array, T> rbegin() {
            return safe_iterator<safe_array, T>(*this, this->size() == 0 ? 0 : this->size() - 1, direction::backward);
        }
        safe_iterator<const safe_array, const T> crbegin() {
            return safe_iterator<const safe_array, const T>(*this, this->size() == 0 ? 0 : this->size() - 1, direction::backward);
        }
        safe_iterator<safe_array, T> end() {
            return safe_iterator<safe_array, T>(*this, this->size(), direction::forward);
        }
        safe_iterator<const safe_array, const T> cend() {
            return safe_iterator<const safe_array, const T>(*this, this->size(), direction::forward);
        }
        safe_iterator<safe_array, T> rend() {
            return safe_iterator<safe_array, T>(*this, -1, direction::backward);
        }
        safe_iterator<const safe_array, const T> crend() {
            return safe_iterator<const safe_array, const T>(*this, -1, direction::backward);
        }
        T& operator[](const size_t index) {
            return this->at(index);
        }
        const T& operator[](const size_t index) const {
            return this->at(index);
        }
    };

#define array safe_array
}
