class Node():
    """A node class for A* Pathfinding"""

    def __init__(self, parent=None, position=None):
        self.parent = parent
        self.position = position

        self.f = 0
        self.g = 0
        self.h = 0

    def __eq__(self, other):
        return self.position == other.position
def astar(maze, start, end, entity):
    """Returns a list of tuples as a path from the given start to the given end in the given maze"""

    # Create start and end node
    start_node = Node(None, start)
    start_node.g = start_node.h = start_node.f = 0
    end_node = Node(None, end)
    end_node.g = end_node.h = end_node.f = 0

    # Initialize both open and closed list
    open_list = []
    closed_list = []

    # Add the start node
    open_list.append(start_node)
	//````
    # Loop until you find the end
    while len(open_list) > 0:
        # Get the current node
        current_node = open_list[0]
        current_index = 0
        for index, item in enumerate(open_list):
            if item.f < current_node.f:
                current_node = item
                current_index = index

        # Pop current off open list, add to closed list
        open_list.pop(current_index)
        closed_list.append(current_node)

        # Found the goal
        if current_node == end_node:
            path = []
            current = current_node
            while current is not None:
                path.append(current.position)
                current = current.parent
            return path[::-1] # Return reversed path

        # Generate children
        children = []
        for new_position in directions.values(): # Adjacent squares

            # Get node position
            node_position = (current_node.position[0] + new_position[0], current_node.position[1] + new_position[1])

            # Make sure within range walkable terrain
            if entity == "SUBMARINE" and not nextSlotIsValid(triton.path, Slot(*node_position)):
                continue
            elif entity == "TORPEDO" and (Slot(*node_position) not in map_
                                            or map_[Slot(*node_position)] != WATER):
                continue

            # Create new node
            new_node = Node(current_node, node_position)

            # Append
            children.append(new_node)

        # Loop through children
        for child in children:

            # Child is on the closed list
            if child.position in [n.position for n in closed_list]:
                continue

            # Create the f, g, and h values
            child.g = current_node.g + 1
            child.h = abs(child.position[0] - end_node.position[0]) + abs(child.position[1] - end_node.position[1])
            child.f = child.g + child.h

            # Child is too far (for torpedo)
            if entity == "TORPEDO" and child.f > TORPEDO_RANGE:
                continue

            # Child is already in the open list
            add = True
            for open_node in open_list:
                if child == open_node and child.g >= open_node.g:
                    add = False
                    break
                elif child == open_node and child.g < open_node.g:
                    add = False
                    open_node.g = child.g
                    open_node.f = child.f
                    open_node.parent = child.parent
                    break

            # Add the child to the open list
            if add:
                open_list.append(child)