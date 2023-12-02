#pragma once

#include <stdexcept>

class ConnectionClosedByClient : public std::exception
{};
