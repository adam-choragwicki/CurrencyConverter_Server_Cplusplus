#pragma once

class Config
{
public:
    explicit Config(int port, bool debug);

    [[nodiscard]] int getPort() const
    { return port_; }

    [[nodiscard]] bool isDebug() const
    { return debug_; }

private:
    const int port_;
    const bool debug_;
};
