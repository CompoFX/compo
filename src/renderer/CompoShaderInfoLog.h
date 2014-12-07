//
//  CompoShaderInfoLog.h
//  CompoView
//
//  Created by Didiet Noor on 19/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_SHADER_INFOLOG
#define COMPO_SHADER_INFOLOG

#include "CompoTypes.h"

class CompoShaderInfoLog
{
  friend std::ostream &operator << (std::ostream &out, const CompoShaderInfoLog &);
private:
  size_t mCurrentLogSize;
  char *mLogBuffer;
  static CompoShaderInfoLog instance;
public:
  virtual ~CompoShaderInfoLog();
  static const CompoShaderInfoLog &fromShader(const CompoShader&);
  static const CompoShaderInfoLog &fromProgram(const CompoShaderProgram&);
  
  inline const std::string getLogString() const {
    return mLogBuffer;
  }
private:
  CompoShaderInfoLog();
  CompoShaderInfoLog( const CompoShaderInfoLog &);
  void getFromShader( const CompoShader&);
  void getFromProgram( const CompoShaderProgram&);
  void reallocTo( const size_t targetSize);
};

inline std::ostream &operator << (std::ostream &out, const CompoShaderInfoLog &log) {
  out << "[GL Infolog] : " << log.mLogBuffer;
  return out;
}

#endif /* defined(__CompoView__CompoShaderInfoLog__) */
