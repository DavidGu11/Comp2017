"""
Quokka Maze
===========

This file represents the quokka maze, a graph of locations where a quokka is
trying to find a new home.

Please help the quokkas find a path from their current home to their
destination such that they have sufficient food along the way!

*** Assignment Notes ***

This is the main file that will be interacted with during testing.
All functions to implement are marked with a `TODO` comment.

Please implement these methods to help the quokkas find their new home!
"""

from typing import List, Union

from vertex import Vertex


class QuokkaMaze:
    """
    Quokka Maze
    -----------

    This class is the undirected graph class that will contain all the
    information about the locations between the Quokka colony's current home
    and their final destination.

    We _will_ be performing some minor adversarial testing this time, so make
    sure you're performing checks and ensuring that the graph is a valid simple
    graph!

    ===== Functions =====

        * block_edge(u, v) - removes the edge between vertex `u` and vertex `v`
        * fix_edge(u, v) - fixes the edge between vertex `u` and `v`. or adds an
            edge if non-existent
        * find_path(s, t, k) - find a SIMPLE path from veretx `s` to vertex `t`
            such that from any location with food along this simple path we
            reach the next location with food in at most `k` steps
        * exists_path_with_extra_food(s, t, k, x) - returns whether it’s
            possible for the quokkas to make it from s to t along a simple path
            where from any location with food we reach the next location with
            food in at most k steps, by placing food at at most x new locations

    ===== Notes ======

    * We _will_ be adversarially testing, so make sure you check your params!
    * The ordering of vertices in the `vertex.edges` does not matter.
    * You MUST check that `k>=0` and `x>=0` for the respective functions
        * find_path (k must be greater than or equal to 0)
        * exists_path_with_extra_food (k and x must be greater than or equal to
            0)
    * This is an undirected graph, so you don't need to worry about the
        direction of traversing during your path finding.
    * This is a SIMPLE GRAPH, your functions should ensure that it stays that
        way.
    * All vertices in the graph SHOULD BE UNIQUE! IT SHOULD NOT BE POSSIBLE
        TO ADD DUPLICATE VERTICES! (i.e the same vertex instance)
    """

    def __init__(self) -> None:
        """
        Initialises an empty graph with a list of empty vertices.
        """
        self.V = 0
        self.vertices = []
        self.path = []

    def add_vertex(self, v: Vertex) -> bool:
        """
        Adds a vertex to the graph.
        Returns whether the operation was successful or not.

        :param v - The vertex to add to the graph.
        :return true if the vertex was correctly added, else false
        """
        if v == None:
            return False
        if v in self.vertices:
            return False
        self.vertices.append(v)
        for i in self.vertices:
            if i == None:
                self.vertices.remove(i)
        self.V += 1
        return True
        # TODO implement me, please?
        pass

    def fix_edge(self, u: Vertex, v: Vertex) -> bool:
        """
        Fixes the edge between two vertices, u and v.
        If an edge already exists, then this operation should return False.

        :param u - A vertex
        :param v - Another vertex
        :return true if the edge was successfully fixed, else false.
        """
        if u == None or v == None:
            return False
        if v in u.edges or (u in v.edges) or (v not in self.vertices) or (u not in self.vertices):
            return False
        u.add_edge(v)
        v.add_edge(u)
        return True
        # TODO implement me please.
        pass

    def block_edge(self, u: Vertex, v: Vertex) -> bool:
        """
        Blocks the edge between two vertices, u and v.
        Removes the edge if it exists.
        If not, it should be unsuccessful.

        :param u - A vertex
        :param v - Another vertex.
        :return true if the edge was successfully removed, else false.
        """
        if u == None or v == None:
            return False
        if v not in u.edges or (u not in v.edges) or (v not in self.vertices) or (u not in self.vertices):
            return False
        u.rm_edge(v)
        v.rm_edge(u)
        return True
        # TODO implement me, please!
        pass

    def find_path(
            self,
            s: Vertex,
            t: Vertex,
            k: int
    ) -> Union[List[Vertex], None]:
        """
        find_path returns a SIMPLE path between `s` and `t` such that from any
        location with food along this path we reach the next location with food
        in at most `k` steps

        :param s - The start vertex for the quokka colony
        :param t - The destination for the quokka colony
        :param k - The maximum number of hops between locations with food, so
        that the colony can survive!
        :returns
            * The list of vertices to form the simple path from `s` to `t`
            satisfying the conditions.
            OR
            * None if no simple path exists that can satisfy the conditions, or
            is invalid.

        Example:
        (* means the vertex has food)
                    *       *
            A---B---C---D---E

            1/ find_path(s=A, t=E, k=2) -> returns: [A, B, C, D, E]

            2/ find_path(s=A, t=E, k=1) -> returns: None
            (because there isn't enough food!)

            3/ find_path(s=A, t=C, k=4) -> returns: [A, B, C]

        """

        if k < 0  or self.vertices == None:
            return None
        if s == None or t == None:
            return None
        forest  = []
        visited = [s]
        nodes_list = [[s]]
        food = k
        the_t = 0
        # print(t)
        while True:
            if len(nodes_list) == 0:
                for pathss in forest: 
                    # print(pathss)

                    for node in pathss:
                        if(node == pathss[0]):
                            continue
                        
                        food -= 1
                        if node.has_food == True:
                            food = k
                        elif food == 0 and node.has_food == False:
                            food = k
                            break
                        if node == pathss[-1]:
                            return pathss                      
                        

                return None
            
            
            path = nodes_list[0]
            nodes_list = nodes_list[1:]
            
            # print(path)
            # if s.edges[1] in path:
            #     print("g")
            #     print(path)
                # forest.append(path)
            if path[0] == s and path[-1] == t:
                
                # print(path)
                forest.append(path)
            
            for neg in path[-1].edges:
                if neg not in visited:
                    # print(path)
                    temp = []
                    if(neg != t) and neg != s:
                        visited.append(neg)
                    #     the_t += 1
                    #     if(the_t == 2):
                    #         visited.append(neg)
                    # else:
                    
                    for temp1 in path:
                        # print(temp1)
                        temp.append(temp1)
                    temp.append(neg)
                    # print(temp)

                    nodes_list.append(temp)
                # print(node.)


                    
        
        return None

        
        
        
            

        # TODO implement me please
        
    def exists_path_with_extra_food(
        self,
        s: Vertex,
        t: Vertex,
        k: int,
        x: int
    ) -> bool:
        """
        Determines whether it is possible for the quokkas to make it from s to
        t along a SIMPLE path where from any location with food we reach the
        next location with food in at most k steps, by placing food at at most
        x new locations.

        :param s - The start vertex for the quokka colony
        :param t - The destination for the quokka colony
        :param k - The maximum number of hops between locations with food, so
        that the colony can survive!
        :param x - The number of extra foods to add.
        :returns
            * True if with x added food we can complete the simple path
            * False otherwise.

        Example:
        (* means the vertex has food)
                            *
            A---B---C---D---E

            1/ exists_with_extra_food(A, E, 2, 0) -> returns: False
                (because we can't get from A to E with k=2 and 0 extra food)

            2/ exists_with_extra_food(A, E, 2, 1) -> returns: True
                (Yes, if we put food on `C` then we can get to E with k=2)

            3/ exists_with_extra_food(A, E, 1, 6) -> returns: True
                (Yes, if we put food on `B`, `C`, `D` then we reach E!)

        """

        if k < 0  or self.vertices == None:
            return False
        if s == None or t == None or k == None:
            return False
            
        forest  = []
        visited = [s]
        nodes_list = [[s]]
        food = k
        the_t = 0
        add_food = False
        while True:
            if len(nodes_list) == 0:
                for pathss in forest:
                    # print(pathss)
                    for node in pathss:
                        if(node == pathss[0]):
                            continue
                        food -= 1
                        if node.has_food == True:
                            food = k
                        if food == 0 and node.has_food == False and x > 0:
                            food += k
                            x -= 1
                            add_food = True
                        if food == 0 and node.has_food == False:
                            food = k
                            # print(node)
                            break
                        if node == pathss[-1]:
                            return True
                            
                        
                    # print(pathss)
                return False
            
            
            path = nodes_list[0]
            nodes_list = nodes_list[1:]
            
            # print(path)
            if path[0] == s and path[-1] == t:
                forest.append(path)
            
            for neg in path[-1].edges:
                if neg not in visited:
                    # print(path)
                    temp = []
                    if neg != t  and neg != s:
                        visited.append(neg)
                    #     the_t += 1
                    #     if(the_t == 2):
                    #         visited.append(neg)
                    # else:
                    
                    for temp1 in path:
                        # print(temp1)
                        temp.append(temp1)
                    temp.append(neg)
                    # print(temp)

                    nodes_list.append(temp)
                # print(node.)


                    
        


        


        # TODO implement me please
        pass


# A = Vertex(False)
# B = Vertex(True)
# C = Vertex(True)
# D = Vertex(True)
# E = Vertex(True)
# G = Vertex(True)

# m = QuokkaMaze()

# m.add_vertex(A)
# m.add_vertex(B)
# m.add_vertex(C)
# m.add_vertex(D)
# m.add_vertex(E)
# # m.add_vertex(G)


# m.fix_edge(A, B)
# m.fix_edge(B,C)
# m.fix_edge(B, D)
# m.fix_edge(D, E)

# # Example 1: find_path A, E, 2 => returns [A, B, C, D, E]

# print(m.find_path(A,E, 2,))
# print(m.find_path(A,E, 2))
# m = QuokkaMaze()
# A = Vertex(False)
# B = Vertex(False)
# C = Vertex(True)
# D = Vertex(False)
# E = Vertex(True)
# F = Vertex(True)
# G = Vertex(False)
# H = Vertex(False)
# I = Vertex(False)

# m.add_vertex(A)
# m.add_vertex(B)
# m.add_vertex(D)
# m.add_vertex(E)
# m.add_vertex(F)
# m.add_vertex(C)
# m.add_vertex(G)
# m.add_vertex(H)
# m.add_vertex(I)

# """
# A -- B -- F(*) -- D -- E(*)
#     |
#     C(*) -- G -- H
#             | 
#             I
# """

# m.fix_edge(A, B)
# m.fix_edge(B, F)
# m.fix_edge(F, D)
# m.fix_edge(D, E)
# m.fix_edge(B, C)
# m.fix_edge(C, G)
# m.fix_edge(G, H)
# m.fix_edge(G, I)

# print(m.find_path(A, E, 2))
