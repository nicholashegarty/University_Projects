#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <map>

class Location {
    private:
    int x_coord;
    int y_coord;

    public:
    Location(int x=0, int y=0){
        this->set_coords(x,y);
    }

    // modifying instances' print outputs
    friend std::ostream& operator<<(std::ostream& os, const Location& obj ) {
        os << "X: " << obj.x_coord << ", Y: " << obj.y_coord;
        return os;
    }

    void set_coords(int x, int y){
        this->x_coord = x;
        this->y_coord = y;
    }

    int get_x(){
        return this->x_coord;
    }
    int get_y(){
        return this->y_coord;   
    }

    int manhattan_dist(Location dest){ // MD = |x1 - x2| + |y1 - y2|
        return abs(this->get_x() - dest.get_x()) + abs(this->get_y() - dest.get_y());
    }
};

class Restaurant {
    private:
    std::string name;
    Location loc;
    int opening_time; //should be 0-24 inclusive
    int closing_time;

    public:
    Restaurant(std::string n="", Location l=Location(), int o=0, int c=0){
        this->name = n;
        this->loc = l;
        this->opening_time = o;
        this->closing_time = c;
    }

    // custom print output
    friend std::ostream& operator<<(std::ostream& os, const Restaurant& obj ) {
        os << "Restaurant: " << obj.name << ", in " << obj.loc << ", Operating hours: " << obj.opening_time << "-" << obj.closing_time << "\n";
        return os;
    }

    std::string get_name(){
        return this->name;
    }

    std::tuple<int, int> get_operating_hours(){
        return std::make_tuple(this->opening_time, this->closing_time);
    } 

    void change_operating_hours(int o,int c){
        this->opening_time = o;
        this->closing_time = c;
    }

    Location get_location(){
        return this->loc;
    }

    bool is_open(int t){
        if (t >= this->opening_time && t <= this->closing_time){
            return true;
        } else {
            return false;
        }
    }
};

class Where2Eat_tool{
    private:
    std::vector<Restaurant> restaurants;
    std::map<std::string, int> ratings; //uses rest obj's name as key
    Location user_loc;

    public:
    Where2Eat_tool(Location l= Location(0,0)){
        this->reload_location(l);
    }

    //sets user location
    void reload_location(Location l){
        this->user_loc = l;
    }

    //methods to store new data in tool's data structures
    void add_restaurant(Restaurant new_rest,int rating){
        this->ratings[new_rest.get_name()] = rating;
        this->restaurants.push_back(new_rest);
    }

    void add_many_restaurants(std::vector<Restaurant> rest_list, std::vector<int> rating_list){
        for (int i=0; i<rest_list.size(); i++){
            this->add_restaurant(rest_list[i], rating_list[i]); //re-using above method
        }
    }

    //prints info about all stored restaurants
    void info(){
        for (Restaurant x: this->restaurants){
            std::cout << x << " Rating: " << this->ratings[x.get_name()] << "/10\n";
        }
    }

    //generates a list of open restaurants, and returns the nearest, or in case of tie, the best rated
    std::tuple<Restaurant, int, int> where_to_eat(int curr_time){
      std::vector<Restaurant> open_rests;
      for (Restaurant x: this->restaurants){
        if (x.is_open(curr_time)){
          open_rests.push_back(x);
        }
      }
      if (open_rests.size() == 0){
        std::cout << "There is no restaurants open at this time...\n";
        return std::make_tuple(Restaurant(), 0,0); //returning a null result in effect
      } else if (open_rests.size() == 1) {
        Restaurant choice =open_rests[0];
        return std::make_tuple
         (choice, this->ratings[choice.get_name()], this->user_loc.manhattan_dist(choice.get_location()) );
      } else {

        // making a shortlist of nearest open
        std::vector<Restaurant> closest = {open_rests[0]};
        int min_dist= this->user_loc.manhattan_dist(closest[0].get_location());
        for (Restaurant x: open_rests){
          int curr_dist = this->user_loc.manhattan_dist(x.get_location()); 
          if (curr_dist < min_dist){
            min_dist = curr_dist;
            closest.clear();
            closest.push_back(x);
          } else if (curr_dist == min_dist){
            closest.push_back(x);
          } else {
            continue;
          }
        }

        //finding the highest rated out of the nearest open
        Restaurant max_rated = closest[0];
        for (Restaurant x: closest){
          if (this->ratings[x.get_name()] > this->ratings[max_rated.get_name()]){
            max_rated = x;
          }
        }
        return std::make_tuple(max_rated, this->ratings[max_rated.get_name()], min_dist);
      }
    }
};
int main(){

    // Testing Location class
    Location ref = Location(20, 30);
    std::vector<Location> locs;

    //generating 10 random locations
    for (int i=0; i< 10; i++){
        int currx = rand() % 201 -100;
        int curry = rand() % 201 -100;
        locs.push_back(Location(currx, curry));
    }

    // std::cout << ref << "\n";
    // for (Location x: locs){
    //     // std::cout << x;
    //     std::cout << "Distance of Ref from (" << x << ") is: " << ref.manhattan_dist(x) << "\n";
    // }

    //Testing Restaurant class
    Restaurant res = Restaurant("Ratatouille", ref, 9, 21);
    // std::cout << res << "\n";

    std::vector<Restaurant> res_list; 
    //generating 4 random restaurants
    for (int i=0; i<4; i++){
        Restaurant curr_res = Restaurant( ("Res" + std::to_string(i)), Location(i*2, i*3), i + 8, i + 20);
        res_list.push_back(curr_res);
    }

    // for (Restaurant x: res_list){
    //     if  (x.is_open(9)) {
    //         // std::cout << x;
    //     }
    // }

    //Testing Where2Eat Class
    Where2Eat_tool tool = Where2Eat_tool();
    std::vector<int> rate_list = {1,2,3,4};
    tool.add_many_restaurants(res_list, rate_list);
    // tool.add_restaurant(Restaurant("Best", Location(0,0), 1, 23), 10); //test case that should always win
    tool.info();

    std::vector<int> times = {8, 22, 10, 0}; //simulates different current times for user
    for (int x: times){
      std::cout << "\nCurr Time is " << std::to_string(x) << "\n";
      std::tuple<Restaurant, int, int> res = tool.where_to_eat(x);
      Restaurant choice = std::get<0>(res);
      int rating = std::get<1>(res);
      int dist = std::get<2>(res);
      std::cout << "Best option is\n" << choice << "Rating: " << rating << "\nDistance: " << dist << "\n";
    }
}
