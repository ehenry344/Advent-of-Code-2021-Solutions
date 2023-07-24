# Evan Henry 
# Advent of Code 2021 Day 9 

def get_adjacents(r, c, map_len, line_len): 
    adjacents = [] 

    if r > 0: 
        adjacents.append((r - 1, c))
    if r < map_len - 1:
        adjacents.append((r + 1, c))
    if c > 0: 
        adjacents.append((r, c - 1))
    if c < line_len - 1: 
        adjacents.append((r, c + 1)) 

    return adjacents

def count_flood_fill(height_map, low_pt): 
    h_len = len(height_map)
    l_len = len(height_map[0]) 

    unvisited = [] 
    visited = [] 

    unvisited.append(low_pt) 

    while (len(unvisited)): 
        current_node = unvisited.pop() 

        if (int(height_map[current_node[0]][current_node[1]]) != 9): 
            if current_node not in visited: 
                visited.append(current_node) 
                adjacents = get_adjacents(current_node[0], current_node[1], h_len, l_len) 

                for adj in adjacents: 
                    unvisited.append(adj)
                    
    return len(visited) 


def get_low_points(height_map): 
    low_pts = [] 

    for i, line in enumerate(height_map): 
        for j, num in enumerate(line):
            if int(num) == 9: #never ends up being a low point 
                continue 

            adjacents = get_adjacents(i, j, len(height_map), len(line))
            is_low_point = True 

            for pos in adjacents: 
                if int(height_map[pos[0]][pos[1]]) <= int(num): 
                    is_low_point = False 
                
            if (is_low_point): 
                low_pts.append((i, j)) 

    return low_pts

def get_risk_level(file_name):     
    height_map = open(file_name).read().splitlines() 
    low_points = get_low_points(height_map) 

    risk_level = 0 

    for x, y in low_points: 
        risk_level += (int(height_map[x][y]) + 1) 
    
    return risk_level 

def get_basin_product(file_name, num): 
    height_map = open(file_name).read().splitlines() 

    basin_sizes = [] 

    for position in get_low_points(height_map): 
        basin_sizes.append(count_flood_fill(height_map, position)) 

    basin_sizes = sorted(basin_sizes, reverse = True)[:num] 

    basin_product = 1

    for basin in basin_sizes: 
        basin_product *= basin

    return basin_product
    
print(get_risk_level("Input/Day9.txt"))
print(get_basin_product("Input/Day9.txt", 3)) 








