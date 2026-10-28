
#ifndef UTIL_MACROS_H
#define UTIL_MACROS_H

#define _GD_VARNAME_CONCAT_B_(m_ignore, m_name) m_name
#define _GD_VARNAME_CONCAT_A_(m_a, m_b, m_c)                                   \
  _GD_VARNAME_CONCAT_B_(hello there, m_a##m_b##m_c)
#define _GD_VARNAME_CONCAT_(m_a, m_b, m_c) _GD_VARNAME_CONCAT_A_(m_a, m_b, m_c)
#define GD_UNIQUE_NAME(m_name) _GD_VARNAME_CONCAT_(m_name, _, __COUNTER__)

// unreachable

#if defined(_MSC_VER)
#define GD_UNREACHABLE() __assume(0)
#elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 405
#define GD_UNREACHABLE() __builtin_unreachable()
#else
#define GD_UNREACHABLE()                                                       \
  CRASH_NOW();                                                                 \
  do {                                                                         \
  } while (true);
#endif

namespace gdmono {

template <typename F> struct ScopeExit {
  ScopeExit(F p_exit_func) : exit_func(p_exit_func) {}
  ~ScopeExit() { exit_func(); }
  F exit_func;
};

class ScopeExitAux {
public:
  template <typename F> ScopeExit<F> operator+(F p_exit_func) {
    return ScopeExit<F>(p_exit_func);
  }
};

} // namespace gdmono

#define SCOPE_EXIT                                                             \
  auto GD_UNIQUE_NAME(gd_scope_exit) = gdmono::ScopeExitAux() + [=]()

#endif // UTIL_MACROS_H
