#ifndef COMPO_SLOGGER_H
#define COMPO_SLOGGER_H

#include <slog2.h>
#include <cstdint>
using std::uint8_t;

class CompoSlogger
{
public:
  static inline const CompoSlogger &instance();
  void log( uint8_t severity, const char *format, ...) const;
private:
  CompoSlogger();
  CompoSlogger( const CompoSlogger &);
  CompoSlogger& operator=( const CompoSlogger &);

private:
  slog2_buffer_t mSlogBuffer;
};

inline const CompoSlogger &CompoSlogger::instance()
{
  static CompoSlogger inst;
  return inst;
}

#endif //COMPO_SLOGGER_H
