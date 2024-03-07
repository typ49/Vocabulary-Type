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

    class AnyBase
    {
    public:
      virtual ~AnyBase() = default;
      virtual std::unique_ptr<AnyBase> clone() const = 0;
      virtual const std::type_info &type() const = 0;
    };

    template <typename T>
    class AnyConcrete : public AnyBase
    {
    public:
      template <typename U>
      AnyConcrete(U &&value) : value(std::forward<U>(value)) {}

      std::unique_ptr<AnyBase> clone() const override
      {
        return std::make_unique<AnyConcrete<T>>(*this);
      }

      const T &getValue() const
      {
        return value;
      }

      const std::type_info &type() const override
      {
        return typeid(T);
      }

    private:
      T value;
    };
  }

  template <typename T>
  struct InPlaceTypeStruct
  {
  };

  template <typename T>
  inline constexpr InPlaceTypeStruct<T> InPlaceType = {};

  class Any
  {
  private:
    std::unique_ptr<details::AnyBase> content;

  public:
    /// @brief Default constructor
    Any();

    /// @brief constructor with a value
    /// @tparam T the type of the value to be stored
    /// @param value the value to be stored
    template <typename T, typename std::enable_if<!std::is_same<Any, std::decay_t<T>>::value>::type* = nullptr>
    Any(T &&value) : content(new details::AnyConcrete<std::decay_t<T>>(std::forward<T>(value))) {}

    
    /// @brief constructor with a value and a type struct
    /// @tparam T the type of the value to be stored
    /// @tparam ...Args the type of the arguments to be passed to the constructor of T
    /// @param type the type struct
    /// @param ...args the arguments to be passed to the constructor of T
    template <typename T, typename... Args>
    Any(InPlaceTypeStruct<T>, Args &&...args) : content(new details::AnyConcrete<T>(T(std::forward<Args>(args)...))) {}

    /// @brief copy constructor
    /// @param other the other Any object to be copied
    Any(const Any& other) : content(other.content ? other.content->clone() : nullptr) {}

    /// @brief move constructor
    /// @param other the other Any object to be moved
    Any(Any &&other) noexcept : content(std::move(other.content)) {}

    Any &operator=(const Any &other)
    {
      if (this != &other)
      {
        content = other.content ? other.content->clone() : nullptr;
      }
      return *this;
    }

    Any &operator=(Any &&other) noexcept
    {
      content = std::move(other.content);
      return *this;
    }

    bool hasValue() const;

    operator bool() const;

    void clear();

    const std::type_info &getType() const;

    template <typename T>
    friend T anyCast(const Any &any);

    template <typename T>
    friend T *anyCast(Any *any);

    template <typename T>
    friend const T *anyCast(const Any *any);

    details::AnyBase *contentPtr() const { return content.get(); };
  };

  template <typename T, typename... Args>
  Any makeAny(Args &&...args)
  {
    return Any(InPlaceType<T>, std::forward<Args>(args)...);
  }

  /// @brief cast an any object to a T object
  /// @tparam T the type of the value to be casted
  /// @param any the Any object to be casted
  /// @return an object of type T
  // anyCast for any type
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

  // anyCast for any pointer type
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

  // anyCast for any const type
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
