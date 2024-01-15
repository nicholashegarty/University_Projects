import random

class Location:
    __x_coord = 0
    __y_coord = 0
    
    def __init__(self, x=0, y=0):
        self.set_coords(x,y)

    # modifying instances' print outputs
    def __str__(self):
        return f'X: {self.__x_coord}, Y: {self.__y_coord}'

    def set_coords(self, x, y):
        self.__x_coord = x
        self.__y_coord = y

    def get_x(self):
        return self.__x_coord
    
    def get_y(self):
        return self.__y_coord;   

    def manhattan_dist(self, dest): # MD = |x1 - x2| + |y1 - y2|
        return abs(self.get_x() - dest.get_x()) + abs(self.get_y() - dest.get_y())

class Restaurant:
    __name = ""
    __loc = Location()
    __opening_time = 0 #should be 0-24 inclusive
    __closing_time = 0
   
    def __init__(self, n="", l=Location(), o=0, c=0):
        self.name = n
        self.loc = l
        self.opening_time = o
        self.closing_time = c

    # custom print output
    def __str__(self):
        return f'Restaurant: {self.get_name()}, in {self.get_location()}, Operating hours: {self.opening_time}-{self.closing_time}\n'

    def get_name(self):
        return self.name

    def get_operating_hours(self):
        return (self.opening_time, self.closing_time)

    def change_operating_hours(self, o, c):
        self.opening_time = o
        self.closing_time = c

    def get_location(self):
        return self.loc

    def is_open(self, t):
        if t >= self.opening_time and t <= self.closing_time:
            return True
        else: 
            return False

class Where2Eat_tool:
    __restaurants = []
    __ratings = {} #uses rest obj's name as key
    __user_loc = Location()
    
    def __init__(self, l= Location(0,0)):
        self.reload_location(l)

    #sets user location
    def reload_location(self, l):
        self.__user_loc = l

    #methods to store new data in tool's data structures
    def add_restaurant(self, new_rest, rating):
        self.__ratings[new_rest.get_name()] = rating
        self.__restaurants.append(new_rest)

    def add_many_restaurants(self, rest_list, rating_list):
        for i in range(0, len(rest_list)):
            self.add_restaurant(rest_list[i], rating_list[i]) #re-using above method

    #prints info about all stored restaurants
    def info(self):
        for  x in self.__restaurants:
            print(f'{x} Rating: {self.__ratings[x.get_name()]}/10\n')


    #generates a list of open restaurants, and returns the nearest, or in case of tie, the best rated
    def where_to_eat(self, curr_time):
      open_rests = []
      for  x in self.__restaurants:
        if x.is_open(curr_time):
          open_rests.append(x)
        
      if len(open_rests) == 0:
        print("There is no restaurants open at this time...\n")
        return (Restaurant(), 0, 0) #returning a null result in effect
      elif len(open_rests) == 1:
        choice =open_rests[0]
        return (choice, self.__ratings[choice.get_name()], self.__user_loc.manhattan_dist(choice.get_location()))
      else:
        # making a shortlist of nearest open
        closest = [open_rests[0]]
        min_dist = self.__user_loc.manhattan_dist(closest[0].get_location())
        for x in open_rests:
          curr_dist = self.__user_loc.manhattan_dist(x.get_location())
          if curr_dist < min_dist:
            min_dist = curr_dist
            closest.clear()
            closest.append(x)
          elif curr_dist == min_dist:
            closest.append(x)
          else:
            continue

        #finding the highest rated out of the nearest open
        max_rated = closest[0]
        for x in closest:
          if self.__ratings[x.get_name()] > self.__ratings[max_rated.get_name()]:
            max_rated = x
        return (max_rated, self.__ratings[max_rated.get_name()], min_dist)
      
    

# Testing Location class
ref = Location(20, 30)
locs = []

#generating 10 random locations
for i in range(0,10):
    currx = random.randint(-100, 100)
    curry = random.randint(-100, 100)
    locs.append(Location(currx, curry))


print(f'{ref}\n')
for x in locs:
    print(x)
    print(f'Distance of Ref from {x} is: {ref.manhattan_dist(x)}\n')


#Testing Restaurant class
res = Restaurant("Ratatouille", ref, 9, 21)
# std::cout << res << "\n";

res_list = []
#generating 4 random restaurants
for i in range (0,4):
    curr_res = Restaurant( ("Res" + str(i)), Location(i*2, i*3), i + 8, i + 20)
    res_list.append(curr_res)


for x in res_list:
    if  (x.is_open(9)):
        print(x)

#Testing Where2Eat Class
tool = Where2Eat_tool()
rate_list = [1,2,3,4]
tool.add_many_restaurants(res_list, rate_list)
#tool.add_restaurant(Restaurant("Best", Location(0,0), 1, 23), 10) //test case that should always win
tool.info()

times = [8, 22, 10, 0] #simulates different current times for user
for x in times:
    print(f'Curr Time is {x}\n')
    res = tool.where_to_eat(x)
    choice = res[0]
    rating = res[1]
    dist = res[2]
    print(f'Best option is:\n{choice}Rating: {rating}\nDistance: {dist}\n')

