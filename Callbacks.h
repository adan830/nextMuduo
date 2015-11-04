#pragma once

#include "Declear.h"

#include <memory>
#include <functional>
using namespace std;

namespace nextMuduo {

typedef std::shared_ptr<ConnectionHandler> ConnectionHandlerPtr;
typedef std::function<void (const ConnectionHandlerPtr&)> ConnectionCallback;
typedef std::function<void (const ConnectionHandlerPtr&)> WriteCompleteCallback;
typedef std::function<void (const ConnectionHandlerPtr&, Buffer&)> MessageCallback;

}
