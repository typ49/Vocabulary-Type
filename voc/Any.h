#ifndef VOC_ANY_H
#define VOC_ANY_H

#include <typeinfo>
#include <memory>
#include <stdexcept>

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
  public:
    Any();

    template <typename T>
    Any(T &&value) : content(new details::AnyConcrete<std::decay_t<T>>(std::forward<T>(value))) {}

    template <typename T, typename... Args>
    Any(InPlaceTypeStruct<T>, Args &&...args) : content(new details::AnyConcrete<T>(T(std::forward<Args>(args)...))) {}

    Any(const Any &other) : content(other.content ? other.content->clone() : nullptr) {}

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

  private:
    std::unique_ptr<details::AnyBase> content;
  };

  template <typename T, typename... Args>
  Any makeAny(Args &&...args)
  {
    return Any(InPlaceType<T>, std::forward<Args>(args)...);
  }

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

  template <typename T>
  T *anyCast(Any *any)
  {
    if (any && any->hasValue() && any->getType() == typeid(T))
    {
      auto concrete = dynamic_cast<details::AnyConcrete<T> *>(any->content.get());
      if (concrete)
      {
        return &concrete->getValue();
      }
    }
    return nullptr;
  }

  template <typename T>
  const T *anyCast(const Any *any)
  {
    return anyCast<T>(const_cast<Any *>(any));
  }

} // namespace voc

#endif // VOC_ANY_H
