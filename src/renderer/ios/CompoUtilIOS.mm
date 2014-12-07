//
//  CompoUtilIOS.mm
//  CompoView
//
//  Created by Didiet Noor on 20/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#include "CompoUtil.h"

inline const std::string getBundleResourcePath( NSBundle *bundle )
{
  NSString *bundlePath = [bundle resourcePath];
  return std::string([bundlePath UTF8String]);
}

const std::string getFullPathFromBundle( NSBundle *bundle, const std::string &relativePath )
{
  return getBundleResourcePath(bundle) + "/" + relativePath;
}

const std::string getFullPathFromResource(const std::string &relativePath)
{
  NSBundle *mainBundle = [NSBundle mainBundle];
  NSString *mainBundlePath = [mainBundle resourcePath];
  NSString *compoBundlePath = [mainBundlePath stringByAppendingPathComponent:@"Compo.bundle"];
  NSBundle *compoBundle = [NSBundle bundleWithPath:compoBundlePath];
  
  const std::string mainBundlePathStr = getFullPathFromBundle(mainBundle, relativePath);
  
  if (isFileExists(mainBundlePathStr)) {
    return mainBundlePathStr;
  }
  else {
    const std::string compoBundlePathStr = getFullPathFromBundle(compoBundle, relativePath);
    if (isFileExists(compoBundlePathStr)) {
      return compoBundlePathStr;
    }
    else
    {
      return "";
    }
  }
  
}

bool isFileExists( const std::string &relativePath )
{
  NSString *strPath = [NSString stringWithUTF8String:relativePath.c_str()];
  BOOL exists = [[NSFileManager defaultManager] fileExistsAtPath:strPath];
  return (exists == YES);
}