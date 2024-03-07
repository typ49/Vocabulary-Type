#ifndef VOC_OPTIONAL_H
#define VOC_OPTIONAL_H

#include <typeinfo>
#include <memory>
#include <stdexcept>
#include <utility>

namespace voc
{

  /// @brief Struct for in-place construction of Optional
  struct InPlaceStruct
  {
  };

  /// @brief Constant for in-place construction of Optional
  inline constexpr InPlaceStruct InPlace = {};

  /// @brief Class to store a value or no value
  template <typename T>
  class Optional
  {
  public:
    /// @brief Default constructor
    Optional() : initialized(false) {}

    /// @brief Constructor from a value
    /// @param value The value to be stored
    Optional(const T &value) : initialized(true)
    {
      new (&data) T(value);
    }

    /// @brief Constructor from a rvalue
    /// @param value The value to be stored
    Optional(T &&value) : initialized(true)
    {
      new (&data) T(std::move(value));
    }

    /// @brief Constructor for in-place construction
    /// @tparam Args The types of the arguments to be passed to the constructor of T
    /// @param ...args The arguments to be passed to the constructor of T
    template <typename... Args>
    Optional(InPlaceStruct, Args &&...args) : initialized(true)
    {
      new (&data) T(std::forward<Args>(args)...);
    }

    /// @brief Copy constructor
    /// @param other The other Optional object to be copied
    Optional(const Optional &other) : initialized(other.initialized)
    {
      if (other.initialized)
      {
        new (&data) T(*other.ptr());
      }
    }

    /// @brief Move constructor
    /// @param other The other Optional object to be moved
    Optional(Optional &&other) noexcept : initialized(other.initialized)
    {
      if (other.initialized)
      {
        new (&data) T(std::move(*other.ptr()));
        other.clear(); // Ensure the moved-from object is in a valid state
      }
    }

    /// @brief Destructor
    ~Optional()
    {
      clear();
    }

    /// @brief Copy assignment operator
    /// @param other The other Optional object to be copied
    /// @return A reference to the current object
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

    /// @brief Move assignment operator
    /// @param other The other Optional object to be moved
    /// @return A reference to the current object
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

    /// @brief Check if the Optional object has a value
    /// @return true if the Optional object has a value, false otherwise
    bool hasValue() const
    {
      return initialized;
    }

    /// @brief Conversion operator to bool
    /// @return true if the Optional object has a value, false otherwise
    explicit operator bool() const
    {
      return hasValue();
    }

    /// @brief Get the stored value
    /// @return The stored value
    T &getValue()
    {
      if (!initialized)
        throw std::runtime_error("Optional has no value");
      return *ptr();
    }

    /// @brief Get the stored value
    /// @return The stored value
    const T &getValue() const
    {
      if (!initialized)
        throw std::runtime_error("Optional has no value");
      return *ptr();
    }

    /// @brief Get the stored value or a default value
    /// @tparam U The type of the default value
    /// @param defaultValue The default value
    /// @return The stored value if it exists, otherwise the default value
    template <typename U>
    T getValueOr(U &&defaultValue) const
    {
      return initialized ? *ptr() : static_cast<T>(std::forward<U>(defaultValue));
    }

    /// @brief Clear the Optional object
    void clear()
    {
      if (initialized)
      {
        ptr()->~T(); // Call the destructor explicitly
        initialized = false;
      }
    }

    /// @brief Dereference operator
    /// @return A reference to the stored value
    T &operator*()
    {
      return *ptr();
    }

    /// @brief Dereference operator
    /// @return A const reference to the stored value
    const T &operator*() const
    {
      return *ptr();
    }

    /// @brief Arrow operator
    /// @return A pointer to the stored value
    T *operator->()
    {
      return ptr();
    }

    /// @brief Arrow operator
    /// @return A const pointer to the stored value
    const T *operator->() const
    {
      return ptr();
    }

  private:
    alignas(T) mutable char data[sizeof(T)]; ///< The stored value
    bool initialized = false; ///< Whether the Optional object has a value

    /// @brief Get a pointer to the stored value
    /// @return A pointer to the stored value
    T *ptr()
    {
      return reinterpret_cast<T *>(&data);
    }

    /// @brief Get a const pointer to the stored value
    /// @return A const pointer to the stored value
    const T *ptr() const
    {
      return reinterpret_cast<const T *>(&data);
    }
  };

  /// @brief Create an Optional object from a value
  /// @tparam T The type of the value to be stored
  /// @tparam ...Args The type of the arguments to be passed to the constructor of T
  /// @param ...args The arguments to be passed to the constructor of T
  /// @return An Optional object storing the value
  template <typename T, typename... Args>
  Optional<T> makeOptional(Args &&...args)
  {
    return Optional<T>(InPlace, std::forward<Args>(args)...);
  }

} // namespace voc

#endif // VOC_OPTIONAL_H