//
//  CompoUtil.h
//  CompoView
//
//  Created by Didiet Noor on 20/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_UTIL_H
#define COMPO_UTIL_H
#include "CompoTypes.h"

const std::string getFullPathFromResource(const std::string &relativePath);

bool isFileExists( const std::string &relativePath );

#ifdef COMPO_TARGET_IOS
#include <UIKit/UIKit.h>
const std::string getFullPathFromBundle( NSBundle *bundle, const std::string &relativePath );

#endif
#endif
