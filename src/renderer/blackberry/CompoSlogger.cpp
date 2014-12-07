#include "CompoSlogger.h"
#include <cstdio>
#include <cstdarg>

using std::fprintf;

CompoSlogger::CompoSlogger()
{
  slog2_buffer_set_config_t buffer_config;
  buffer_config.buffer_set_name = "COMPOVIEW";

  buffer_config.num_buffers = 1;

  buffer_config.verbosity_level = SLOG2_INFO;

  buffer_config.buffer_config[0].buffer_name = "hirate";
  buffer_config.buffer_config[0].num_pages = 7;

  if ( -1 == slog2_register( &buffer_config, &mSlogBuffer, 0)) {
    fprintf( stderr, "Error registering slogger2 buffer\n");
    return;
  }
}

void CompoSlogger::log( uint8_t severity, const char *fmt, ... ) const
{
  va_list ap;
  va_start(ap, fmt);
  vslog2f( mSlogBuffer, 0xD00D, severity, fmt, ap );
  va_end(ap);
}

