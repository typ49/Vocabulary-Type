#ifndef VOC_ANY_H
#define VOC_ANY_H

#include <typeinfo>

namespace voc {

  template<typename T>
  struct InPlaceTypeStruct {
  };

  template<typename T>
  inline constexpr InPlaceTypeStruct<T> InPlaceType = { };

  class Any {
  public:
    /*
     * Create an empty object
     */
    Any();

    /*
     * Create an object thanks to a value
     */
    template<typename T>
    Any(T&& value) {
    }

    /*
     * Create an object in place with the arguments of a constructor of T
     */
    template<typename T, typename ... Args>
    Any(InPlaceTypeStruct<T> inPlace, Args&& ... args) {
    }

    /*
     * Affecte a directly value
     */
    template<typename U>
    Any& operator=(U&& value) {
      return *this;
    }

    /*
     * Tell if the object has a value or is empty
     */
    bool hasValue() const;

    /*
     * Tell if the object has a value or is empty
     */
    operator bool() const;

    /*
     * Clear the object. After a call to clear, the object is empty.
     */
    void clear();

    /*
     * Return the type_info of the type of the object,
     * or the type_info of void if the object is empty
     */
    const std::type_info& getType() const;

  private:
    // implementation
  };

  /*
   * Create an instance of Any with a value of type T
   */
  template<typename T, typename... Args>
  Any makeAny(Args&&... args) {
    return Any();
  }

  /*
   * Return a copy of the object, or std::bad_cast if the object is empty or has not the right type
   */
  template<typename T>
  T anyCast(const Any& any) {
    return T();
  }

  /*
   * Return a copy of the object, or std::bad_cast if the object is empty or has not the right type
   */
  template<typename T>
  T anyCast(Any& any) {
    return T();
  }

  /*
   * Return a copy of the object, or std::bad_cast if the object is empty or has not the right type
   */
  template<typename T>
  T anyCast(Any&& any) {
    return T();
  }

  /*
   * Return a pointer to the object, or nullptr if the object is empty or has not the right type
   */
  template<typename T>
  T* anyCast(Any* any) {
    return nullptr;
  }

  /*
   * Return a pointer to the object, or nullptr if the object is empty or has not the right type
   */
  template<typename T>
  const T* anyCast(const Any* any) {
    return nullptr;
  }

}

#endif // VOC_ANY_H
