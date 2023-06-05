#include "train.hpp"
#include <iostream>

void Train::get_on_map(int x, int y)
{
    map.lock_mutex(x, y);
    draw_on_map = true;
    distance_map = 0;
}

void Train::ride_on_map(int x, int y)
{
    x = map.getRoutes()[from][to][distance_map+1].x;
    y = map.getRoutes()[from][to][distance_map+1].y;
    map.lock_mutex(x, y);
    distance_map++;

    x = map.getRoutes()[from][to][distance_map-1].x;
    y = map.getRoutes()[from][to][distance_map-1].y;
    map.unlock_mutex(x, y);
}

void Train::get_off_map( int x, int y)
{
    map.unlock_mutex(x, y);
    distance_map = -1;
    draw_on_map = false;
    is_on_map = false;
}

void Train::get_on_track(World_map& city_map, int x, int y)
{
    city_map.lock_mutex(x, y);
    draw_in_city = true;
    distance_tr = 0;
}

void Train::ride_on_track(World_map& city_map, int x, int y)
{
    x = city_map.getRoutes()[from_tr][to_tr][distance_tr+1].x;
    y = city_map.getRoutes()[from_tr][to_tr][distance_tr+1].y;
    city_map.lock_mutex(x, y);
    distance_tr++;

    x = city_map.getRoutes()[from_tr][to_tr][distance_tr-1].x;
    y = city_map.getRoutes()[from_tr][to_tr][distance_tr-1].y;
    city_map.unlock_mutex(x, y);
}


void Train::get_off_track(World_map& city_map, int x, int y)
{
    city_map.unlock_mutex(x, y);
    distance_tr = -1;
    draw_in_city = false;
}


void Train::set_is_on_map(bool is)
{
    is_on_map = is;
}


int Train::form_city()
{
    return from;
}

int Train::to_city()
{
    return to;
}

int Train::distance()
{
    return distance_map;
}

int Train::form_track()
{
    return from_tr;
}

int Train::to_track()
{
    return to_tr;
}

int Train::distance_track()
{
    return distance_tr;
}

void Train::run_in_city(World_map& city_map){
    int xt = 0;
    int yt = 0;
    from_tr = 0;
    to_tr = 1;
    while(true){ //cos innego
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(distance_tr==-1){
                xt = city_map.getRoutes()[from_tr][to_tr][0].x;
                yt = city_map.getRoutes()[from_tr][to_tr][0].y;
                get_on_track(city_map, xt, yt);
            }else if(city_map.getRoutes()[from_tr][to_tr].size()<distance_tr+2){
                 xt = city_map.getRoutes()[from_tr][to_tr][distance_tr].x;
                 yt = city_map.getRoutes()[from_tr][to_tr][distance_tr].y;
                //ride_on_track(city_map, xt, yt);
                from_tr = getFromOlesnicaIn(to_tr, city_map);
                to_tr = getToOlesnicaIn(to_tr,city_map);
                get_off_track(city_map, xt, yt);
            }else{
                xt = city_map.getRoutes()[from_tr][to_tr][distance_tr].x;
                yt = city_map.getRoutes()[from_tr][to_tr][distance_tr].y;
                ride_on_track(city_map, xt, yt);
            }
    }
}
int Train::getFromOlesnicaIn(int to, World_map& city_map){
    switch(to){
        case 1:
            if (city_map.counter[0]){
                return 4;
            }else{
                return 6;
            }
            break;
        case 5:
            return 26;
            break;
        case 27:
            if (city_map.counter[1]){
                return 32;
            }else{
                return 34;
            }
            break;
        case 7:
            if (city_map.counter[2]){
                return 30;
            }else{
                return 28;
            }
        default:
            return 0;
            break;
    }

}

int Train::getToOlesnicaIn(int to, World_map& city_map){
    switch(to){
        case 1:
            if (city_map.counter[0]){
                city_map.counter[0] = false;
                return 5;
            }else{
                city_map.counter[0] = true;
                return 7;
            }
            break;
        case 5:
            return 27;
            break;
        case 27:
            if (city_map.counter[1]){
                city_map.counter[1] = false;
                return 33;
            }else{
                city_map.counter[1] = true;
                return 35;
            }
            break;
        case 7:
            if (city_map.counter[2]){
                city_map.counter[2] = false;
                return 31;
            }else{
                city_map.counter[2] = true;
                return 29;
            }
        default:
            return 1;
            break;
    }


}

void Train::run(){
    int x = 0;
    int y = 0;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(speed*50));
        if(is_on_map){
            
            
            // std::cout<<distance_map<<std::endl;
            if(distance_map==-1){
                x = map.getRoutes()[from][to][0].x;
                y = map.getRoutes()[from][to][0].y;
                    get_on_map(x, y);
            }else if(map.getRoutes()[from][to].size()<distance_map+2){
                x = map.getRoutes()[from][to][distance_map].x;
                y = map.getRoutes()[from][to][distance_map].y;
                if(controler.enter_city(to)){
                    get_off_map(x, y);
                }
            }else{
                x = map.getRoutes()[from][to][distance_map].x;
                y = map.getRoutes()[from][to][distance_map].y;
                    ride_on_map(x, y);
            }

        }else{
            switch (to)
            {
            case 7:
                run_in_city(olesnica);
                break;
            
            case 8:
                /* code */
                break;
            
            default:
                std::this_thread::sleep_for(std::chrono::seconds(5));
                break;
            }
            
            // std::cout<<from<<"->"<<to<<std::endl;
            controler.take_passangers(train_cap, to);
            from = to;
            to = controler.where_to_go(to);
            // std::cout<<from<<"->"<<to<<std::endl;
            is_on_map = true;
            controler.exit_city(from);
        }
    }
}

void Train::start(int f)
{
    from = f;
    to = controler.start_destination(f);
    is_on_map = true;
}

void Train::set_up_route(int f, int t)
{
    to = t;
    from = f;
}
