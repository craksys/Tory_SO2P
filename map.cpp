#include "map.hpp"
#include <iostream>

void World_map::addLocation(int x, int y, const std::string name) {
    cities.push_back(City(x,y,name));
}

void World_map::matrixes(){
    int N = cities.size();
    std::vector<std::vector<std::vector<Track>>> matrix_routes(N, std::vector<std::vector<Track>>(N));
    std::vector<std::vector<bool>> matrix_connections(N, std::vector<bool>(N, false));


    areConnections = matrix_connections;
    routes = matrix_routes;
}

void World_map::revers(int cityA, int cityB){
    if(cities[cityB].x-cities[cityA].x < cities[cityA].y-cities[cityB].y){
        for(int i = 0; i < routes[cityA][cityB].size(); i++){
            routes[cityB][cityA].insert(routes[cityB][cityA].begin(),Track(routes[cityA][cityB][i].x-1,routes[cityA][cityB][i].y,routes[cityA][cityB][i].typeOfTrack));
        }
    }else{
        for(int i = 0; i < routes[cityA][cityB].size(); i++){
            routes[cityB][cityA].insert(
                routes[cityB][cityA].begin(),
                Track(routes[cityA][cityB][i].x,
                    routes[cityA][cityB][i].y-1,
                    routes[cityA][cityB][i].typeOfTrack));
        }
    }
    
}

void World_map::create_rails()
{
    for (int i = 0; i < cities.size(); i++) {
        for (int j = 0; j < cities.size(); j++) {
            for (auto &&point : routes[i][j])
            {
                if(map_rail.find(point.x*1000+point.y) == map_rail.end()){
                    map_rail[point.x*1000+point.y] = used_mutex;
                    used_mutex++;
                }
            }
        }
    }
}


void World_map::connect(int cityA, int cityB, int trackInLine){
    routes[cityA][cityB] = line(cityA, cityB, trackInLine);
    areConnections[cityA][cityB] = true;

    revers(cityA,cityB);
    areConnections[cityB][cityA] = true;
}

void World_map::connect_without_revers(int cityA, int cityB, int trackInLine){
    routes[cityA][cityB] = line(cityA, cityB, trackInLine);
}

void World_map::connect_reversed(int cityA, int cityB, int trackInLine){
    routes[cityA][cityB] = lineReverse(cityA, cityB, trackInLine);
}

std::vector<Track> World_map::line(int cityA, int cityB, int trackInLine){
    std::vector<Track> points;
    char diagonaChar;
    int slope;
    int step = 1;
    int x = cities[cityA].x+1;
    int y = cities[cityA].y+1;
    if(cities[cityB].x-cities[cityA].x < cities[cityA].y-cities[cityB].y){
        y--;
        int By = cities[cityB].y+2;
        for(int i=0;i<trackInLine;i++){{
            y -= step;
            points.push_back(Track(x,y,'|'));
            }
        }

        if(cities[cityA].x < cities[cityB].x){
            while (x < cities[cityB].x+1){
                y += -1;
                points.push_back(Track(x,y,'/'));

                x += 1;
            }
            y += -1;
            points.push_back(Track(x,y,'/'));
        }
        else{
            y += -1;
            points.push_back(Track(x,y,'\\'));
            while (x > cities[cityB].x+1){
                x += -1;
                y += -1;
                points.push_back(Track(x,y,'\\'));
            }
        }

        while(y > By){
        y -= step;
        points.push_back(Track(x,y,'|'));
        }
    }else{
        int By = cities[cityB].y+1;
        for(int i=0;i<trackInLine;i++){{
            x += step;
            points.push_back(Track(x,y,'_'));
            }
        }

        if(cities[cityA].y < By){
            while (y < By){
                x += 1;
                y += 1;
                points.push_back(Track(x,y,'\\'));
            }
        }
        else{
            while (y > By){
                x += 1;
                points.push_back(Track(x,y,'/'));
                y += -1;
            }
        }

        while(x<cities[cityB].x-1){
        x += step;
        points.push_back(Track(x,y,'_'));
        }
    }
    return points;
}
std::vector<Track> World_map::lineReverse(int cityA, int cityB, int trackInLine){
    std::vector<Track> points;
    char diagonaChar;
    int slope;
    int step = 1;
    int x = cities[cityA].x+1;
    int y = cities[cityA].y+1;
    if(cities[cityB].x-cities[cityA].x < cities[cityA].y-cities[cityB].y){
        y--;
        int By = cities[cityB].y+2;
        for(int i=0;i<trackInLine;i++){{
                y -= step;
                points.insert(points.begin(),Track(x,y,'|'));
                //points.push_back(Track(x,y,'|'));
            }
        }

        if(cities[cityA].x < cities[cityB].x){
            while (x < cities[cityB].x+1){
                y += -1;
                points.insert(points.begin(),Track(x,y,'/'));
                x += 1;
            }
            y += -1;
            points.insert(points.begin(),Track(x,y,'/'));
        }
        else{
            y += -1;
            points.insert(points.begin(),Track(x,y,'\\'));
            while (x > cities[cityB].x+1){
                x += -1;
                y += -1;
                points.insert(points.begin(),Track(x,y,'\\'));
            }
        }

        while(y > By){
            y -= step;
            points.insert(points.begin(),Track(x,y,'|'));
        }
    }else{
        int By = cities[cityB].y+1;
        for(int i=0;i<trackInLine;i++){{
                x += step;
                points.insert(points.begin(),Track(x,y,'_'));
            }
        }

        if(cities[cityA].y < By){
            while (y < By){
                x += 1;
                y += 1;
                points.insert(points.begin(),Track(x,y,'\\'));
            }
        }
        else{
            while (y > By){
                x += 1;
                points.insert(points.begin(),Track(x,y,'/'));
                y += -1;
            }
        }

        while(x<cities[cityB].x-1){
            x += step;
            points.insert(points.begin(),Track(x,y,'_'));
        }
    }
    return points;
}
void World_map::lock_mutex(int x, int y)
{
    mutex_rails[map_rail[x*1000+y]].lock();
    // std::cout<<x<<"  "<<y<<"L"<<std::endl;
}

void World_map::unlock_mutex(int x, int y)
{
    mutex_rails[map_rail[x*1000+y]].unlock();
    // std::cout<<x<<"  "<<y<<"U"<<std::endl;
}
