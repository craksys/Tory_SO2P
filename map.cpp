#include "map.hpp"
#include <iostream>

void World_map::start_train_on_map(int train_id, int cityA, int cityB){
trains_on_map.insert(std::make_pair(train_id, occupied_track(cityA,cityB,rails[cityA][cityB])));
trains_on_map.at(train_id).rail->start_travel();
}

void World_map::move_train_forvard(int train_id){
trains_on_map.at(train_id).rail->go_forward();
}

void World_map::addLocation(int x, int y, const std::string name) {
    cities.push_back(City(x,y,name));
}

void World_map::matrixes(){
    int N = cities.size();
    std::vector<std::vector<std::vector<Track>>> matrix_routes(N, std::vector<std::vector<Track>>(N));
    std::vector<std::vector<Rail*>> matrix_rails(N, std::vector<Rail*>(N));
    std::vector<std::vector<Rail*>> matrix_crossovers(N, std::vector<Rail*>(N));
    routes = matrix_routes;
    rails = matrix_rails;
    crossovers = matrix_crossovers;
}

void World_map::revers(int cityA, int cityB){
    if(cities[cityB].x-cities[cityA].x < cities[cityA].y-cities[cityB].y){
        for(int i = 0; i < routes[cityA][cityB].size(); i++){
            routes[cityB][cityA].insert(routes[cityB][cityA].begin(),Track(routes[cityA][cityB][i].x-1,routes[cityA][cityB][i].y,routes[cityA][cityB][i].typeOfTrack));
        }
    }else{
        for(int i = 0; i < routes[cityA][cityB].size(); i++){
            routes[cityB][cityA].insert(routes[cityB][cityA].begin(),Track(routes[cityA][cityB][i].x,routes[cityA][cityB][i].y-1,routes[cityA][cityB][i].typeOfTrack));
        }
    }
    
}

void World_map::create_rails()
{
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < cities.size(); j++) {
            Rail* rail_array[4];
            std::cout<<i<<" "<<j<<":  ";
            if (routes[i][j].size() > 1){
                std::cout<<abs(cities[i].x-routes[i][j][0].x + cities[i].y-routes[i][j][0].y);
                rails[i][j] = rail_array[abs(cities[i].x-routes[i][j][0].x + cities[i].y-routes[i][j][0].y)];
            }
            std::cout<<std::endl;
        }
        //TODO wychodzą z miast
    }
}

void World_map::connect(int cityA, int cityB, int trackInLine){
    routes[cityA][cityB] = line(cityA, cityB, trackInLine);
    revers(cityA,cityB);
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