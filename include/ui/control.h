#ifndef CONTROL_H
#define CONTROL_H

#include <ui/ui_main.h>

class Control {
public:
private:
    void run();
    
    void on_log_select(int signal_responce);
    void on_json_check(int signal_responce);
    void on_message_box_check(int signal_responce);
};

#endif