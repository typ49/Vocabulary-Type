#ifndef VOC_ANY_H
#define VOC_ANY_H

#include <typeinfo>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace voc
{
  namespace details
  {
    /// @brief Base class for AnyConcrete
    class AnyBase
    {
    public:
      /// @brief Default destructor
      virtual ~AnyBase() = default;

      /// @brief Clone the current object
      /// @return A unique_ptr to the cloned object
      virtual std::unique_ptr<AnyBase> clone() const = 0;

      /// @brief Get the type of the stored value
      /// @return The type_info of the stored value
      virtual const std::type_info &type() const = 0;
    };

    /// @brief Concrete class to store any type of value
    template <typename T>
    class AnyConcrete : public AnyBase
    {
    public:
      /// @brief Constructor from a value
      /// @param value The value to be stored
      template <typename U>
      AnyConcrete(U &&value) : value(std::forward<U>(value)) {}

      /// @brief Clone the current object
      /// @return A unique_ptr to the cloned object
      std::unique_ptr<AnyBase> clone() const override
      {
        return std::make_unique<AnyConcrete<T>>(*this);
      }

      /// @brief Get the stored value
      /// @return The stored value
      const T &getValue() const
      {
        return value;
      }

      /// @brief Get the type of the stored value
      /// @return The type_info of the stored value
      const std::type_info &type() const override
      {
        return typeid(T);
      }

    private:
      T value; ///< The stored value
    };
  }

  template <typename T>
  struct InPlaceTypeStruct
  {
  };

  template <typename T>
  inline constexpr InPlaceTypeStruct<T> InPlaceType = {};

  /// @brief Class to store any type of value
  class Any
  {
  private:
    std::unique_ptr<details::AnyBase> content; ///< The stored value

  public:
    /// @brief Default constructor
    Any();

    /// @brief Constructor from a value
    /// @tparam T The type of the value to be stored
    /// @param value The value to be stored
    template <typename T, typename std::enable_if<!std::is_same<Any, std::decay_t<T>>::value>::type * = nullptr>
    Any(T &&value) : content(new details::AnyConcrete<std::decay_t<T>>(std::forward<T>(value))) {}

    /// @brief Constructor from a value and a type struct
    /// @tparam T The type of the value to be stored
    /// @tparam ...Args The type of the arguments to be passed to the constructor of T
    /// @param type The type struct
    /// @param ...args The arguments to be passed to the constructor of T
    template <typename T, typename... Args>
    Any(InPlaceTypeStruct<T>, Args &&...args) : content(new details::AnyConcrete<T>(T(std::forward<Args>(args)...))) {}

    /// @brief Copy constructor
    /// @param other The other Any object to be copied
    Any(const Any &other) : content(other.content ? other.content->clone() : nullptr) {}

    /// @brief Move constructor
    /// @param other The other Any object to be moved
    Any(Any &&other) noexcept : content(std::move(other.content)) {}

    /// @brief Copy assignment operator
    /// @param other The other Any object to be copied
    /// @return A reference to the current object
    Any &operator=(const Any &other)
    {
      if (this != &other)
      {
        content = other.content ? other.content->clone() : nullptr;
      }
      return *this;
    }

    /// @brief Move assignment operator
    /// @param other The other Any object to be moved
    /// @return A reference to the current object
    Any &operator=(Any &&other) noexcept
    {
      content = std::move(other.content);
      return *this;
    }

    /// @brief Check if the Any object has a value
    /// @return true if the Any object has a value, false otherwise
    bool hasValue() const;

    /// @brief Conversion operator to bool
    /// @return true if the Any object has a value, false otherwise
    operator bool() const;

    /// @brief Clear the Any object
    void clear();

    /// @brief Get the type of the stored value
    /// @return The type_info of the stored value
    const std::type_info &getType() const;

    template <typename T>
    friend T anyCast(const Any &any);

    template <typename T>
    friend T *anyCast(Any *any);

    template <typename T>
    friend const T *anyCast(const Any *any);

    /// @brief Get a pointer to the stored value
    /// @return A pointer to the stored value
    details::AnyBase *contentPtr() const { return content.get(); };
  };

  /// @brief Create an Any object from a value
  /// @tparam T The type of the value to be stored
  /// @tparam ...Args The type of the arguments to be passed to the constructor of T
  /// @param ...args The arguments to be passed to the constructor of T
  /// @return An Any object storing the value
  template <typename T, typename... Args>
  Any makeAny(Args &&...args)
  {
    return Any(InPlaceType<T>, std::forward<Args>(args)...);
  }

  /// @brief Cast an Any object to a T object
  /// @tparam T The type of the value to be casted
  /// @param any The Any object to be casted
  /// @return An object of type T
  template <typename T>
  T anyCast(const Any &any)
  {
    if (!any.hasValue() || any.getType() != typeid(T))
    {
      throw std::bad_cast();
    }

    auto concrete = dynamic_cast<details::AnyConcrete<std::decay_t<T>> *>(any.content.get());
    if (!concrete)
    {
      throw std::bad_cast();
    }
    return concrete->getValue();
  }

  /// @brief Cast an Any object to a T pointer
  /// @tparam T The type of the value to be casted
  /// @param any The Any object to be casted
  /// @return A pointer to an object of type T, or nullptr if the cast fails
  template <typename T>
  T anyCast(Any *any)
  {
    if (any && any->hasValue() && any->getType() == typeid(T))
    {
      auto concrete = dynamic_cast<details::AnyConcrete<T> *>(any->contentPtr());
      if (concrete)
      {
        return concrete->getValue();
      }
    }
    return nullptr;
  }

  /// @brief Cast an Any object to a const T pointer
  /// @tparam T The type of the value to be casted
  /// @param any The Any object to be casted
  /// @return A const pointer to an object of type T, or nullptr if the cast fails
  template <typename T>
  const T *anyCast(const Any *any)
  {
    auto concrete = dynamic_cast<details::AnyConcrete<T> *>(any->content.get());
    if (concrete)
    {
      return &concrete->getValue();
    }
    return nullptr;
  }

} // namespace voc

#endif // VOC_ANY_H