#include <ui/elements/path_input.h>
#include <iostream>

PathInput::PathInput(Gtk::Window& root) :  root_(root){
    make_path_select();
    make_path_field();

    auto thing = path_field->get_text();

    append(*path_field);
    append(*path_select);
}

PathInput::~PathInput() {
    delete path_select;
    delete path_field;
}

void PathInput::make_path_select() {
    path_select = new Gtk::Button("Select path");
    path_select->set_margin(5);
    path_select->signal_clicked().connect(sigc::mem_fun(*this, &PathInput::on_path_select_click));
}

void PathInput::make_path_field() {
    path_field = new Gtk::Entry();
    path_field->set_margin(5);
    path_field->set_width_chars(50);
    path_field->set_placeholder_text("Enter path here");
}

void PathInput::on_path_select_click() {
    if(!dir_dialogue) {
        dir_dialogue = Gtk::FileChooserNative::create(
            "Select Directory", 
            root_,
            Gtk::FileChooser::Action::SELECT_FOLDER,
            "Select",
            "Cancel"
        );
        dir_dialogue->set_modal(true);
        dir_dialogue->signal_response().connect(sigc::mem_fun(*this, &PathInput::on_path_select_dialogue_end));
    }
    dir_dialogue->show();
}

void PathInput::on_path_select_dialogue_end(int response_id) {
    if (response_id == static_cast<int>(Gtk::ResponseType::ACCEPT)){
        auto dir = dir_dialogue->get_file();
        if (dir) {
            path_field->set_text(dir->get_path());
        }
    }
    dir_dialogue.reset();
}