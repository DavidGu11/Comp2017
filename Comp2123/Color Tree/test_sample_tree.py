"""
Test Tree
=========

Checks that your tree performs basic functionality.
"""

import unittest

from colours import Colours
from node import Node
from tree import Tree


class TestTree(unittest.TestCase):
    """
    Checks super basic tree functionality
    """

    def test_put(self):
        """
        Can we insert into tree?
        """

        root = Node(Colours.CYAN)

        t = Tree(root)

        a = Node(Colours.CYAN)

        t.put(root, a)

        assert len(root.children) == 1, \
            "[tree.put] should add child to node."

        assert root.children[0] == a, \
            "[tree.put] should add the correct node, yours did not."

        t.put(a, Node(Colours.YELLOW))

        assert len(root.children) == 1, \
            "[tree.put] should add child to node."

        assert root.children[0] == a, \
            "[tree.put] should add the correct node, yours did not."

        assert len(a.children) == 1, \
            "[tree.put] should add child to node."

    def test_put_propagate(self):
        """
        Does the colour propagate?
        """

        A = Node(Colours.YELLOW)

        B = Node(Colours.BLUE)
        C = Node(Colours.GREEN)

        # D = Node(Colours.CYAN)
        # J = Node(Colours.CYAN)
        # K = Node(Colours.YELLOW)

        t = Tree(A)

        t.put(A, B)
        t.put(A, C)
        # t.put(B, D)
        # t.put(C, J)
        # t.put(J, K)

        # Nothing should propagate yet
        assert Colours.GREEN.cmp(A.propagated_colour) == 0, \
            "[propagate] Your colour didn't propagate correctly."

        # t.put(root, a)
        # # It should now be blue!
        # assert Colours.BLUE.cmp(root.propagated_colour) == 0, \
        #     "[propagate] Your colour didn't propagate correctly."

        # t.put(a, Node(Colours.RED))

        # assert a.propagated_colour == Colours.RED, \
        #     "yes"
        # # It should now be red!
        # assert Colours.RED.cmp(root.propagated_colour) == 0, \
        #     "[propagate] Your colour didn't propagate correctly."

        # assert Colours.RED.cmp(a.propagated_colour) == 0, \
        #     "[propagate] Your colour didn't propagate correctly."

    def test_update_colour_propagates(self):
        """
        Does the colour propagate when changed?
        """

        root = Node(Colours.CYAN)

        t = Tree(root)
        a = Node(Colours.BLUE)

        t.put(root, a)
        t.put(a, Node(Colours.RED))

        # It should now be red!
        assert Colours.RED.cmp(root.propagated_colour) == 0, \
            "[propagate] Your colour didn't propagate correctly."

        assert Colours.RED.cmp(a.propagated_colour) == 0, \
            "[propagate] Your colour didn't propagate correctly."

        t.update_node_colour(a.children[0], Colours.NYAN)
        assert a.propagated_colour == Colours.NYAN, \
            "yes"
        assert Colours.NYAN.cmp(root.propagated_colour) == 0, \
            "[propagate] Your colour didn't propagate correctly."

        assert Colours.NYAN.cmp(a.propagated_colour) == 0, \
            "[propagate] Your colour didn't propagate correctly."

    def test_can_rm(self):
        """
        Can we remove a child?
        """

        root = Node(Colours.CYAN)

        t = Tree(root)
        a = Node(Colours.BLUE)
        b = Node(Colours.YELLOW)
        c = Node(Colours.BLUE)
        d = Node(Colours.GREEN)
        e = Node(Colours.CYAN)
        f = Node(Colours.YELLOW)
        l = Node(Colours.NYAN)
        g = Node(Colours.YELLOW)
        i = Node(Colours.GREEN)
        h = Node(Colours.RED)
        j = Node(Colours.CYAN)
        k = Node(Colours.GREEN)

        t.put(root, a)
        t.put(root, b)
        t.put(root, h)
        t.put(h, i)
        t.put(a, j)
        t.put(j, k)
        t.put(b, d)
        t.put(d, e)
        t.put(e, f)
        t.put(b, c)
        t.put(c, l)
        t.put(c, g)
        # t.put(e, f)
        t.update_node_colour(l, Colours.GREEN)
        t.update_node_colour(l, Colours.GREEN)
        t.update_node_colour(l, Colours.BLUE)
        t.update_node_colour(l, Colours.CYAN)
        
        assert len(root.children) == 3, \
            "[tree.rm] did not remove the node."
        assert d.propagated_colour == Colours.GREEN, \
            "yes"
        t.rm(g)
        
        assert root.propagated_colour == Colours.NYAN, \
            "[tree.rm] did not remove the node."

        # assert b not in root.children, \
        #     "[tree.rm] did not remove the correct child."

    def test_rm_propagate(self):
        """
        Can we remove a child and the colour propagates?
        """

        A = Node(Colours.BLUE)

        B = Node(Colours.RED)
        C = Node(Colours.RED)

        D = Node(Colours.CYAN)
        J = Node(Colours.CYAN)
        K = Node(Colours.YELLOW)
        G = Node(Colours.YELLOW)

        t = Tree(A)

        t.put(A, B)
        t.put(A, C)
        t.put(A, G)
        t.put(B, D)
        t.put(C, J)
        t.put(J, K)

        t.rm(B)

        assert A.propagated_colour == Colours.RED, \
            "yes"

        # assert a.parent == root, \
        #     "yes"
        # assert Colours.GREEN.cmp(root.propagated_colour) == 0, \
        #     "Colour did not propagate when removing a child!"

    def test_can_swap_example(self):
        """
        Can you perform the swap in the comments?
        """

        root = Node(Colours.YELLOW)

        t = Tree(root)
        a = Node(Colours.BLUE)
        b = Node(Colours.YELLOW)
        c = Node(Colours.BLUE)
        d = Node(Colours.GREEN)
        e = Node(Colours.CYAN)
        f = Node(Colours.YELLOW)
        l = Node(Colours.NYAN)
        g = Node(Colours.YELLOW)
        i = Node(Colours.GREEN)
        h = Node(Colours.RED)
        j = Node(Colours.CYAN)
        k = Node(Colours.GREEN)

        t.put(root, a)
        t.put(root, b)
        t.put(root, h)
        t.put(h, i)
        t.put(a, j)
        t.put(j, k)
        t.put(b, d)
        assert Colours.GREEN.cmp(b.propagated_colour) == 0, \
            "Colour of sibling changed?"
        t.put(d, e)
        t.put(e, f)
        t.put(b, c)
        t.put(c, l)
        t.put(c, g)
        # t.put(e, f)
        # t.update_node_colour(l, Colours.GREEN)
        # t.update_node_colour(l, Colours.GREEN)
        # t.update_node_colour(l, Colours.BLUE)
        # t.update_node_colour(l, Colours.CYAN)
        assert Colours.NYAN.cmp(b.propagated_colour) == 0, \
            "Colour of sibling changed?"
        t.swap(j,b)
    


        assert Colours.NYAN.cmp(root.propagated_colour) == 0, \
            "Colour of sibling changed?"
        # Let's check if it worked!
        # assert f.parent == j, \
        #     "[tree.swap] Did not change parent."
        
        # assert k.parent == e, \
        #     "[tree.swap] Did not change parent."
        # assert B.parent == A, \
        # #     "[tree.swap] Did not change parent."
        # assert Colours.RED.cmp(J.colour) == 0, \
        #     "Colour of sibling changedsd?"
        # assert Colours.RED.cmp(A.propagated_colour) == 0, \
        #     "Colour of sibling changed?"
        # assert Colours.NYAN.cmp(root.propagated_colour) == 0, \
        #     "Colour of sibling changed?"
        # assert k in e.children, \
        #     "[tree.swap] Did not remove child from old parent."

        # assert f in j.children, \
        #     "[tree.swap] Did not remove child from old parent."

        # assert C in B.children, \
        #     "[tree.swap] child incorrectly swapped to children list."

        # assert D in A.children, \
        #     "[tree.swap] child incorrectly swapped to children list."

    def test_depth_example(self):
        """
        Can you perform the is_coloured function?


        (start)---> G
                   / \
              (A) N   B (B)
                 /|    \
           (A1) G R(A2) G (B1)
                  |
                  G (A21)
        """

        root = Node(Colours.GREEN)

        A = Node(Colours.NYAN)
        B = Node(Colours.BLUE)

        A1 = Node(Colours.GREEN)
        A2 = Node(Colours.RED)
        A21 = Node(Colours.GREEN)

        B1 = Node(Colours.GREEN)

        t = Tree(root)

        # t.put(root, A)
        # t.put(root, B)

        # t.put(A, A1)
        # t.put(A, A2)
        # t.put(A2, A21)
        # t.put(B, B1)
        t.update_node_colour(root,Colours.CYAN)

        assert t.is_coloured_to_depth_k(root, Colours.CYAN, 0), \
            "[is_coloured] Returned false, should be true!"

        # assert root.propagated_colour == Colours.RED, \
        #     "[is_coloured] Returned true, should be false!"

        # assert t.is_coloured_to_depth_k(root, Colours.GREEN, 2), \
        #     "[is_coloured] Returned true, should be false!"

        # assert t.is_coloured_to_depth_k(root, Colours.GREEN, 1), \
        #     "[is_coloured] Returned false, should be true!"
    def test(self):
        """
        Can you perform the swap in the comments?
        """

        A = Node(Colours.GREEN)

        B = Node(Colours.RED)
        C = Node(Colours.BLUE)

        D = Node(Colours.CYAN)
        J = Node(Colours.CYAN)
        K = Node(Colours.YELLOW)

        t = Tree(A)

        t.put(A, B)
        t.put(A, C)
        t.put(B, D)
        t.put(C, J)
        t.put(C, K)
        assert D.parent == B, \
            "yes"
        # Let's swap
        t.swap(D, K)

        # Let's check if it worked!
        assert D.parent == J, \
            "[tree.swap] Did not change parent."
        
        assert J.parent == B, \
            "[tree.swap] Did not change parent."
b = TestTree()


# b.test_can_swap_example()
b.test_depth_example()