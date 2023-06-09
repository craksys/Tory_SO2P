#ifndef GUI_HEADER
#define GUI_HEADER

#include "gui_map.hpp"
#include <ncurses.h>
#include <vector>
#include <string>

class GUI {
private:
    WINDOW *menu_win;
    WINDOW *info_win;
    GUImap gui_map;
    GUImap olesnica_map;
    GUImap zgorzelec_map;
    std::vector<std::string> optionsVector;
    int counter = 0;
    int option = 0;

public:
    void setUpGUI(const std::vector<City>& c, const std::vector<std::vector<std::vector<Track>>>& r);
    int getCounter() const;
    void printOptions() const;
    void show(std::vector<Train>& trains, Control_map& contr);
    void keyboard();

    void setUpGUIOlesnica(const std::vector<City> &c, 
                          const std::vector<std::vector<std::vector<Track>>> &r);
    void setUpGUIZgorzelec(const std::vector<City> &c, 
                           const std::vector<std::vector<std::vector<Track>>> &r);
};

#endif