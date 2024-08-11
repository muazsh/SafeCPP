#pragma once
#include <vector>
#include <array>
#include <mutex>
#include <memory>

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
        std::weak_ptr<C> container_;
        std::shared_ptr<std::mutex> mutex_;
        int index_;
        direction direction_;

    public:
        safe_iterator(std::weak_ptr<C> container, std::shared_ptr<std::mutex> mutex, int ind, direction dir) : 
            container_(std::move(container)),
            mutex_(std::move(mutex)), 
            index_(ind), 
            direction_(dir) {}
        int get_index() const noexcept {
            return index_;
        }
        void set_index(int index) noexcept {
            index_ = index;
        }
        safe_iterator<C, T>& operator++() noexcept {
            index_ += static_cast<int>(direction_);
            return *this;
        }

        safe_iterator<C, T> operator++(int) noexcept {
            auto prev = *this;
            operator++(); 
            return prev;   
        }

        safe_iterator<C, T>& operator--() noexcept {
            index_ -= static_cast<int>(direction_);
            return *this;
        }

        safe_iterator<C, T> operator--(int) noexcept {
            auto prev = *this;
            operator--();
            return prev;
        }

        safe_iterator<C, T> operator+(const int offset) const noexcept {
            safe_iterator<C, T> result(container_);
            result.index_ = index_ + offset * static_cast<int>(direction_);
            return result;
        }

        safe_iterator<C, T> operator-(const int offset) const noexcept {
            return *this + (-offset);
        }

        safe_iterator<C, T>& operator+=(const int offset) noexcept {
            index_ += offset * static_cast<int>(direction_);
            return *this;
        }

        safe_iterator<C, T>& operator-=(const int offset) noexcept {
            return *this += (-offset);
        }

        T& operator*() const {
            std::lock_guard<std::mutex> lock(*mutex_);
            if (container_.expired())
            {
                throw std::logic_error("Iterator invalid.");
            }
            if (index_ < 0) {
                throw std::out_of_range("Iterator out of range.");
            }
            return container_.lock()->at(index_);
        }

        bool operator==(const safe_iterator<C, T>& other) const noexcept {
            std::lock_guard<std::mutex> lock(*mutex_);
            if (!container_.expired()) {
                return container_.lock() == other.container_.lock() && index_ == other.index_;
            }
            return false;
        }

        bool operator<(const safe_iterator<C, T>& other) const noexcept {
            std::lock_guard<std::mutex> lock(*mutex_);
            if (!container_.expired()) {
                return container_.lock() == other.container_.lock() && index_ < other.index_;
            }
            return false;
        }

        bool operator<=(const safe_iterator<C, T>& other) const noexcept {
            std::lock_guard<std::mutex> lock(*mutex_);
            if (!container_.expired()) {
                return container_.lock() == other.container_.lock() && index_ <= other.index_;
            }
            return false;
        }

        bool operator!=(const safe_iterator<C, T>& other) const noexcept {
            return !(*this == other);
        }

        bool operator>(const safe_iterator<C, T>& other) const noexcept {
            return !(*this <= other) ;
        }

        bool operator>=(const safe_iterator<C, T>& other) const noexcept {
            return !(*this < other);
        }
    };

    template<typename T>
    struct type_false : std::false_type
    { };

    template<class T>
    class safe_vector {
        std::shared_ptr<std::vector<T>> vector_ = {};
        std::shared_ptr<std::mutex> mutex_ = {};
        std::vector<T>::const_iterator get_corresponding_internal_iterator(safe_iterator<safe_vector, T> const& itr) {
            return vector_->begin() + itr.get_index();
        }
    public:
        safe_vector() : vector_( std::make_shared<std::vector<T>>()), mutex_(make_shared<std::mutex>()) {}
        safe_vector(std::initializer_list<T> l) : vector_(std::make_shared<std::vector<T>>(l)), mutex_(make_shared<std::mutex>()) {}
        safe_iterator<std::vector<T>, T> begin() {
            return safe_iterator<std::vector<T>, T>(vector_, mutex_, 0, direction::forward);
        }
        safe_iterator<const std::vector<T>, const T> cbegin() {
            return safe_iterator<const std::vector<T>, const T>(vector_, mutex_, 0, direction::forward);
        }
        safe_iterator<std::vector<T>, T> rbegin() {
            return safe_iterator<std::vector<T>, T>(vector_, mutex_, vector_->size() == 0 ? 0 : vector_->size() - 1, direction::backward);
        }
        safe_iterator<const std::vector<T>, const T> crbegin() {
            return safe_iterator<const std::vector<T>, const T>(vector_, mutex_, vector_->size() == 0 ? 0 : vector_->size() - 1, direction::backward);
        }
        safe_iterator<std::vector<T>, T> end() {
            return safe_iterator<std::vector<T>, T>(vector_, mutex_, vector_->size(), direction::forward);
        }
        safe_iterator<const std::vector<T>, const T> cend() {
            return safe_iterator<const std::vector<T>, const T>(vector_, mutex_, vector_->size(), direction::forward);
        }
        safe_iterator<std::vector<T>, T> rend() {
            return safe_iterator<std::vector<T>, T>(vector_, mutex_, -1, direction::backward);
        }
        safe_iterator<const std::vector<T>, const T> crend() {
            return safe_iterator<const std::vector<T>, const T>(vector_, mutex_, -1, direction::backward);
        }
        T& operator[](const size_t index) {
            return vector_->at(index);
        }
        const T& operator[](const size_t index) const {
            return vector_->at(index);
        }

        T& at(const size_t index) {
            return vector_->at(index);
        }
        const T& at(const size_t index) const {
            return vector_->at(index);
        }

        T& front() {
            return vector_->at(0);
        }
        const T& front() const {
            return vector_->at(0);
        }

        T& back() {
            std::lock_guard<std::mutex> guard(mutex_);
            return vector_->at(vector_->size() - 1);
        }
        const T& back() const {
            std::lock_guard<std::mutex> guard(mutex_);
            return vector_->at(vector_->size() - 1);
        }

        bool empty() const noexcept {
            return vector_->empty();
        }

        size_t size() const noexcept {
            return vector_->size();
        }

        void clear() noexcept {
            vector_->clear();
        }

        safe_iterator<safe_vector, T> insert(const safe_iterator<safe_vector, T>& pos, const T& value) {
            std::lock_guard<std::mutex> guard(mutex_);
            auto result_itr = pos;
            auto itr = get_corresponding_internal_iterator(pos);
            auto insert_itr = vector_->insert(itr, value);
            result_itr.set_index(std::distance(vector_->begin(), insert_itr));
            return result_itr;
        }

        safe_iterator<safe_vector, T> insert(const safe_iterator<safe_vector, T>& pos, T&& value) {
            std::lock_guard<std::mutex> guard(mutex_);
            auto result_itr = pos;
            auto itr = get_corresponding_internal_iterator(pos);
            auto insert_itr = vector_->insert(itr, std::move(value));
            result_itr.set_index(std::distance(vector_->begin(), insert_itr));
            return result_itr;
        }

        safe_iterator<safe_vector, T> insert(const safe_iterator<safe_vector, T>& pos, std::size_t count, const T& value) {
            std::lock_guard<std::mutex> guard(mutex_);
            auto result_itr = pos;
            auto itr = get_corresponding_internal_iterator(pos);
            auto insert_itr = vector_->insert(itr, count, value);
            result_itr.set_index(std::distance(vector_->begin(), insert_itr));
            return result_itr;
        }

        template< class InputIt >
        safe_iterator<safe_vector, T> insert(const safe_iterator<safe_vector, T>& pos, InputIt first, InputIt last) {
            std::lock_guard<std::mutex> guard(mutex_);
            auto result_itr = pos;
            auto itr = get_corresponding_internal_iterator(pos);
            auto insert_itr = vector_->insert(itr, first, last);
            result_itr.set_index(std::distance(vector_->begin(), insert_itr));
            return result_itr;
        }

        safe_iterator<safe_vector, T> insert(const safe_iterator<safe_vector, T>& pos, std::initializer_list<T> ilist) {
            std::lock_guard<std::mutex> guard(mutex_);
            auto result_itr = pos;
            auto itr = get_corresponding_internal_iterator(pos);
            auto insert_itr = vector_->insert(itr, ilist);
            result_itr.set_index(std::distance(vector_->begin(), insert_itr));
            return result_itr;
        }

        template< class... Args >
        safe_iterator<safe_vector, T> emplace(const safe_iterator<safe_vector, T>& pos, Args&&... args) {
            std::lock_guard<std::mutex> guard(mutex_);
            auto result_itr = pos;
            auto itr = get_corresponding_internal_iterator(pos);
            auto insert_itr = vector_->emplace(itr, args);
            result_itr.set_index(std::distance(vector_->begin(), insert_itr));
            return result_itr;
        }

        void push_back(const T& value) {
            std::lock_guard<std::mutex> guard(mutex_);
            vector_->push_back(value);
        }
        void push_back(T&& value) {
            std::lock_guard<std::mutex> guard(mutex_);
            vector_->push_back(std::move(value));
        }

        template< class... Args >
        void emplace_back(Args&&... args) {
            std::lock_guard<std::mutex> guard(mutex_);
            vector_->emplace_back(args);
        }
       
        void pop_back() {
            std::lock_guard<std::mutex> guard(mutex_);
            vector_->pop_back();
        }
    };

#define vector safe_vector
}
