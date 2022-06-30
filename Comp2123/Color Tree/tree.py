"""
Tree
----

This file contains the tree data structure that will be used for interacting
with our coloured nodes.
The tree contains a "root" node, which is the topmost node of the tree.
It is interconnected through children and finally ends at external nodes ending
at the leaves.

*** Assignment Notes ***

This is the main file that will be tested, you must implement the related
functions with a TODO annotated.

Your task is to implement these methods.
"""

from node import Node
from colours import Colour
from colours import Colours




class Tree:
    """
    Tree Class
    ----------

    Contains the data structure of a tree, where each node of the tree has a
    parent and children.
    If a node has no parent, it is considered the "root" of the tree.
    If a node has zero (0) children, it is a leaf (or is "external").

    Each node in the tree has the type `Node`, which is defined in `node.py`.

    ====== Functions ======

    - __init__ : Sets up the tree with a specified root.
    - put(node, child) : Adds the `child` to the `node`.
    - swap(subtree_a, subtree_b) : Swaps the position of the subtrees.
    - is_coloured_to_depth_k(node, colour, k) : Checks that the subtree rooted
        at `node` has the same colour until `k` levels deep.

    == Things to note ==

    1. Every node given as an argument WILL be in the tree, you do not have to
        check whether it exists in the tree.
    2. Every node will be initialised with a parent (unless it is the root node
        of the tree).
    3. The ordering of the children does not matter.
    """
    def __init__(self, root: Node) -> None:
        """
        Initialises the tree with a root of type `Node` from `node.py`

        :param root: The root node of our tree.
        """

        self.root = root
        self.root.isRoot = True

    def propagation(self,child: Node) -> None:
        tmp_N = self.root
        if child.propagated_colour is None:
            child.propagated_colour = child.colour
        if child.children:
            for x in child.children:
                if x.propagated_colour.cmp(child.propagated_colour) == 1:
                    child.propagated_colour = x.propagated_colour
        while True:    
            if child.propagated_colour.cmp(self.root.propagated_colour) == 1:
                self.root.propagated_colour = child.propagated_colour
            if self.root.parent == None:
                break
            self.root = self.root.parent

    def rmpropagation(self,child: Node,removed_colour:Colour) -> None:
        tmp_N = self.root
        count = 0
        child.propagated_colour = child.colour
        # 
        while True:      
            # print(tmp_N.colour.cmp(Colours.YELLOW))
            if removed_colour.cmp(tmp_N.propagated_colour) == 0:
                tmp_N.propagated_colour = tmp_N.colour
            if tmp_N.children:
                # print(tmp_N.colour.cmp(Colours.CYAN))
                for x in tmp_N.children:
                    if x.propagated_colour.cmp(tmp_N.propagated_colour) == 1:
                        tmp_N.propagated_colour = x.propagated_colour
                        # 
            
            
            elif child.propagated_colour.cmp(tmp_N.colour) == 1:
                tmp_N.propagated_colour = child.propagated_colour
            if tmp_N.parent == None:
                break
            tmp_N = tmp_N.parent
            
    

    def update_node_colour(self, n: Node, new_colour: Colour) -> None:
        """
        Update the colour of a node.

        :param n: The node to change the colour of.
        :param new_colour: The new colour to change to.
        """
        if self.root is n:
            self.root.colour = new_colour
            self.root.propogated_colour = new_colour
        x = None
        removed_colour = n.colour
        if removed_colour == new_colour:
            return
        if n in self.root.children:
            n.colour = new_colour
            n.propagated_colour = new_colour
            if n.children:
                for b in n.children:
                    if b.propagated_colour.cmp(n.propagated_colour) == 1:
                            n.propagated_colour = b.propagated_colour



            while True:                 
                if removed_colour.cmp(self.root.propagated_colour) == 0:
                    self.root.propagated_colour = self.root.colour
                if self.root.children:
                    for c in self.root.children:
                        if c.propagated_colour.cmp(self.root.propagated_colour) == 1:
                            self.root.propagated_colour = c.propagated_colour
                            # print(tmp_N.propagated_colour.cmp(Colours.GREEN))
            
                
                elif n.propagated_colour.cmp(self.root.propagated_colour) == 1:
                    self.root.propagated_colour = n.propagated_colour
                if self.root.parent == None:
                    break

                self.root = self.root.parent

                

            # else:
            #     for x in self.root.children:

                

        else: 
            for x in self.root.children:
                tmp_Node = x
                x = Tree(tmp_Node)
                x.update_node_colour(n,new_colour)


        # else:
        #     for x in self.root.children:
        #         x.update_node_colour(n, new_colour)


        # Call update_colour() on the node
        # TODO implement me please.

    def put(self, parent: Node, child: Node) -> None:
        """
        Inserts a node into the tree.
        Adds `child` to `parent`.

        :param parent: The parent node currently in the tree.
        :param child: The child to add to the tree.
        """
        new_Node = child
        if self.root is None:
            self.root = new_Node

        if self.root is parent:
            self.root.add_child(child)
            for x in self.root.children:
                self.propagation(child)


        else: 
            for x in self.root.children:
                tmp_Node = x
                x = Tree(tmp_Node)
                x.put(parent,child)


 
        
        # TODO implement me please.

    def rm(self, child: Node) -> None:
        # new_Node = child
        if self.root is None:
            return
        if self.root == child:
            self.root = None
        # 
        if child in self.root.children:
            removed = child.colour
            self.root.remove_child(child)
            self.root.propagated_colour = self.root.colour
            tmp_N = self.root
            if self.root.children:
                for x in self.root.children:
                    self.rmpropagation(x,removed)
            else:
                self.rmpropagation(self.root,removed)
            #     # print(1)
            #     while tmp_N.parent != None:

            #         if removed.cmp(tmp_N.propagated_colour) == 0:
            #             print("MY colour" + str(tmp_N.colour.cmp(Colours.BLUE)))
            #             tmp_N.propagated_colour = tmp_N.colour
            #         if tmp_N.propagated_colour.cmp(tmp_N.parent.colour) == 1:

            #             tmp_N.parent.propagated_colour = tmp_N.propagated_colour
                    
            #         tmp_N = tmp_N.parent
                
                # else:
                #     if tmp_N.children:
                #         for x in tmp_N.children:
                #             if x.propagated_colour.cmp(tmp_N.colour) == 1:
                #                 tmp_N.propagated_colour = x.propagated_colour    

        else: 
            for x in self.root.children:
                tmp_Node = x
                x = Tree(tmp_Node)
                x.rm(child)
            

        """
        Removes child from parent.

        :param child: The child node to remove.
        """
        # TODO implement me please.

    def swap(self, subtree_a: Node, subtree_b: Node) -> None:
        a = subtree_a.parent
        b = subtree_b.parent
        self.rm(subtree_a)
        self.rm(subtree_b)
        # print(subtree_b.propagated_colour.cmp(Colours.NYAN))
        self.put(a,subtree_b)
        self.put(b,subtree_a)
        """
        Swaps subtree A with subtree B

        :param subtree_a : Root of the subtree A.
        :param subtree_b : Root of the subtree B.

        Example:

            A
           / \
           B  C
         /   / \
        D   J   K

        SWAP(B, C)
            A
           / \
          C  B
         / |  \
        J  K   D

        SWAP(D, C)

            A
           / \
          D  B
              \
               C
              / \
             J   K
        """
        # TODO implement me please.

    def is_coloured_to_depth_k(self, start_node: Node, colour: Colour, k: int) -> bool:
        # if self.root is None or start_node is None or colour is None:
        #     return

        if start_node.colour.cmp(colour) != 0:
            return False

        if k - 1== 0:
            for x in start_node.children:     
                tmp_Node = x
                x = Tree(tmp_Node)         
                if x.root.colour.cmp(colour) != 0:
                    return False
            
        elif k - 1 != 0 and k != 0:
            for x in start_node.children:
                tmp_Node = x
                x = Tree(tmp_Node)
                x = x.is_coloured_to_depth_k(tmp_Node, colour,k - 1)
                if x == False:
                    return False

        return True
        
        """
        Checks whether all nodes in the subtree (up and including level `k`
            starting from the start node) have the same colour!

        (This checks node.colour)

        :param start_node : The node to start checking.
        :param colour: The colour to compare a node's colour to.
        :param k: The depth we should check until.

        === Examples ===

        (start)---> G
                   / \
                  G   G
                 /|   |
                G R   G
                  |
                  R

        is_coloured_to_depth_k(start, Colour.GREEN, 0) => True
        is_coloured_to_depth_k(start, Colour.RED, 0) => False
        is_coloured_to_depth_k(start, Colour.GREEN, 1) => True
        is_coloured_to_depth_k(start, Colour.GREEN, 2) => False
        """


    