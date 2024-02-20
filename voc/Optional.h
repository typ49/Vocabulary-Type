#ifndef VOC_OPTIONAL_H
#define VOC_OPTIONAL_H

#include <typeinfo>
#include <memory>
#include <stdexcept>
#include <utility>

namespace voc
{

  struct InPlaceStruct
  {
  };

  inline constexpr InPlaceStruct InPlace = {};

  template <typename T>
  class Optional
  {
  public:
    Optional() : initialized(false) {}

    Optional(const T &value) : initialized(true)
    {
      new (&data) T(value);
    }

    Optional(T &&value) : initialized(true)
    {
      new (&data) T(std::move(value));
    }

    template <typename... Args>
    Optional(InPlaceStruct, Args &&...args) : initialized(true)
    {
      new (&data) T(std::forward<Args>(args)...);
    }

    Optional(const Optional &other) : initialized(other.initialized)
    {
      if (other.initialized)
      {
        new (&data) T(*other.ptr());
      }
    }

    Optional(Optional &&other) noexcept : initialized(other.initialized)
    {
      if (other.initialized)
      {
        new (&data) T(std::move(*other.ptr()));
        other.clear(); // Ensure the moved-from object is in a valid state
      }
    }

    ~Optional()
    {
      clear();
    }

    Optional &operator=(const Optional &other)
    {
      if (this != &other)
      {
        if (initialized)
          clear();
        if (other.initialized)
        {
          new (&data) T(*other.ptr());
          initialized = true;
        }
      }
      return *this;
    }

    Optional &operator=(Optional &&other) noexcept
    {
      if (this != &other)
      {
        if (initialized)
          clear();
        if (other.initialized)
        {
          new (&data) T(std::move(*other.ptr()));
          initialized = true;
          other.clear(); // Ensure the moved-from object is in a valid state
        }
      }
      return *this;
    }

    bool hasValue() const
    {
      return initialized;
    }

    explicit operator bool() const
    {
      return hasValue();
    }

    T &getValue()
    {
      if (!initialized)
        throw std::runtime_error("Optional has no value");
      return *ptr();
    }

    const T &getValue() const
    {
      if (!initialized)
        throw std::runtime_error("Optional has no value");
      return *ptr();
    }

    template <typename U>
    T getValueOr(U &&defaultValue) const
    {
      return initialized ? *ptr() : static_cast<T>(std::forward<U>(defaultValue));
    }

    void clear()
    {
      if (initialized)
      {
        ptr()->~T(); // Call the destructor explicitly
        initialized = false;
      }
    }

    T *operator->()
    {
      return ptr();
    }

    const T *operator->() const
    {
      return ptr();
    }

    T &operator*()
    {
      return *ptr();
    }

    const T &operator*() const
    {
      return *ptr();
    }

  private:
    alignas(T) mutable char data[sizeof(T)];
    bool initialized = false;

    T *ptr()
    {
      return reinterpret_cast<T *>(&data);
    }

    const T *ptr() const
    {
      return reinterpret_cast<const T *>(&data);
    }
  };

  template <typename T, typename... Args>
  Optional<T> makeOptional(Args &&...args)
  {
    return Optional<T>(InPlace, std::forward<Args>(args)...);
  }

} // namespace voc

#endif // VOC_OPTIONAL_H
