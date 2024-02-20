#include "Any.h"

namespace voc
{

  Any::Any() : content(nullptr)
  {
  }

  bool Any::hasValue() const
  {
    return content != nullptr;
  }

  Any::operator bool() const
  {
    return hasValue();
  }

  void Any::clear()
  {
    content.reset();
  }

  const std::type_info &Any::getType() const
  {
    return content ? content->type() : typeid(void);
  }

}
