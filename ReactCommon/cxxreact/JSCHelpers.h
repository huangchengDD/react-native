// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include "Executor.h"
#include "Value.h"

#include <JavaScriptCore/JSContextRef.h>
#include <JavaScriptCore/JSObjectRef.h>
#include <JavaScriptCore/JSValueRef.h>

#include <stdexcept>
#include <algorithm>

#include "Value.h"

namespace facebook {
namespace react {

class JSException : public std::runtime_error {
public:
  explicit JSException(const char* msg)
    : std::runtime_error(msg)
    , stack_("") {}

  JSException(const char* msg, const char* stack)
    : std::runtime_error(msg)
    , stack_(stack) {}

  const std::string& getStack() const {
    return stack_;
  }

private:
  std::string stack_;
};

inline void throwJSExecutionException(const char* msg) {
  throw JSException(msg);
}

template <typename... Args>
inline void throwJSExecutionException(const char* fmt, Args... args) {
  int msgSize = snprintf(nullptr, 0, fmt, args...);
  msgSize = std::min(512, msgSize + 1);
  char *msg = (char*) alloca(msgSize);
  snprintf(msg, msgSize, fmt, args...);
  throw JSException(msg);
}

template <typename... Args>
inline void throwJSExecutionExceptionWithStack(const char* msg, const char* stack) {
  throw JSException(msg, stack);
}

void installGlobalFunction(
    JSGlobalContextRef ctx,
    const char* name,
    JSObjectCallAsFunctionCallback callback);

JSValueRef makeJSCException(
    JSContextRef ctx,
    const char* exception_text);

String jsStringFromBigString(const JSBigString& bigstr);

JSValueRef evaluateScript(
    JSContextRef ctx,
    JSStringRef script,
    JSStringRef sourceURL);

JSValueRef makeJSError(JSContextRef ctx, const char *error);

JSValueRef translatePendingCppExceptionToJSError(JSContextRef ctx, const char *exceptionLocation);

} }