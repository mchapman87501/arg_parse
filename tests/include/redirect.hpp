#pragma once

#include <iostream>

// See https://gist.github.com/AndreLouisCaron/1841061
namespace Tests {
class Redirect {
  std::ostream &m_stream;
  std::streambuf *const m_buff;

public:
  Redirect(std::ostream &lhs, std::ostream &rhs = std::cout)
      : m_stream(rhs), m_buff(m_stream.rdbuf()) {
    m_stream.rdbuf(lhs.rdbuf());
  }

  ~Redirect() { m_stream.rdbuf(m_buff); }
};
} // namespace Tests
