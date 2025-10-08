#ifndef MESSAGE_BOX_SINK_H
#define MESSAGE_BOX_SINK_H

#include <spdlog/sinks/base_sink.h>

#include <glibmm.h>
#include <functional>
#include <mutex>

class MessageSink : public spdlog::sinks::base_sink<std::mutex> {
public:
    explicit MessageSink(std::function<void(const std::string&)> cb) : callback_(std::move(cb)) {}

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override {
        spdlog::memory_buf_t formatted;
        formatter_->format(msg, formatted);
        std::string s = fmt::to_string(formatted);

        // schedule GUI update on main loop, safe from any thread
        Glib::signal_idle().connect_once([cb = callback_, s = std::move(s)](){
            cb(s);
        });
    }

    void flush_() override {}

private:
    std::function<void(const std::string&)> callback_;
};

#endif