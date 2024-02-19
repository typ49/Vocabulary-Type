#ifndef VOC_OPTIONAL_H
#define VOC_OPTIONAL_H

namespace voc {

  struct InPlaceStruct {
  };

  inline constexpr InPlaceStruct InPlace = { };

  template<typename T>
  class Optional {
  public:
    /*
     * Create an empty object
     */
    Optional()
    {
    }

    /*
     * Create an object thanks to a value
     */
    Optional(/* implementation defined */ value)
    {
    }

    /*
     * Create an object in place with the arguments of a constructor of T
     */
    template<typename ... Args>
    Optional(InPlaceStruct inPlace, Args&& ... args)
    {
    }

    /*
     * Affectation from an value
     */
    template<typename U>
    Optional& operator=(U&& value) {
      return *this;
    }

    /*
     * Affectation from another compatible Optional
     */
    template<typename U>
    Optional& operator=(const Optional<U>& other) {
      return *this;
    }

    /*
     * Affectation from another compatible Optional
     */
    template<typename U>
    Optional& operator=(Optional<U>&& other) {
      return *this;
    }

    /*
     * Tell if the object has a value, or is empty
     */
    bool hasValue() const {
      return false;
    }

    /*
     * Tell if the object has a value, or is empty
     */
    explicit operator bool() const {
      return false;
    }

    /*
     * Return a reference to the object, or std::runtime_error if the object is empty
     */
    T& getValue() {
    }

    /*
     * Return a const reference to the object, or std::runtime_error if the object is empty
     */
    const T& getValue() const {
    }

    /*
     * Return a pointer to the stored data. If the value was initialized, the behaviour is undefined.
     */
    T* operator->() {
      return nullptr;
    }

    /*
     * Return a const pointer to the stored data. If the value was initialized, the behaviour is undefined.
     */
    const T* operator->() const {
      return nullptr;
    }

    /*
     * Return a reference to the stored data. If the value was initialized, the behaviour is undefined.
     */
    T& operator*() {
      return T();
    }

    /*
     * Return a reference to the stored data. If the value was initialized, the behaviour is undefined.
     */
    const T& operator*() const {
      return T();
    }

    /*
     * Return the stored object T or forward the default value
     */
    template<typename U>
    T getValueOr(U&& defaultValue) const {
    }

    /*
     * Clear the stored value
     */
    void clear() {
    }

  private:
    // implementation
  };

  template<typename T, typename... Args>
  Optional<T> makeOptional(Args&&... args) {
    return Optional<T>();
  }

  /*
   * The comparaison operator could take any instance of Optional with compatible type and
   * must works with non initalized Optional.
   * Furthermore we can compare an Option directly with a value.
   *
   * See https://en.cppreference.com/w/cpp/utility/optional/operator_cmp for more details.
   */
  bool operator==(/* implementation defined */ lhs, /* implementation defined */ rhs) {
    return false;
  }

  bool operator!=(/* implementation defined */ lhs, /* implementation defined */ rhs) {
    return false;
  }

  bool operator< (/* implementation defined */ lhs, /* implementation defined */ rhs) {
    return false;
  }

  bool operator<=(/* implementation defined */ lhs, /* implementation defined */ rhs) {
    return false;
  }

  bool operator> (/* implementation defined */ lhs, /* implementation defined */ rhs) {
    return false;
  }

  bool operator>=(/* implementation defined */ lhs, /* implementation defined */ rhs) {
    return false;
  }

}

#endif // VOC_OPTIONAL_H
