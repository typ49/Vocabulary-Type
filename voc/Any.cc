#include "Any.h"

namespace voc {

  Any::Any() {
  }

  bool Any::hasValue() const {
    return false;
  }

  Any::operator bool() const {
    return false;
  }

  void Any::clear() {
  }

  const std::type_info& Any::getType() const {
    return typeid(void);
  }

}
