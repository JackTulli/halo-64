"""Synthetic collision graph termination controls, without retail data."""
import unittest
from tools.n64.collision import CollisionBSP,NONE,SIGN

class CollisionBoundsTests(unittest.TestCase):
    def bsp(self):
        b=CollisionBSP();b.nodes=[(0,NONE,SIGN)];b.planes=[(0.,0.,1.,0.)]
        b.leaves=[(0,0,0)];b.nodes2d=[(1.,0.,0.,NONE,SIGN)]
        return b

    def test_point_front_back_and_terminal_path(self):
        b=self.bsp()
        self.assertEqual(b.test_point((0,0,1)),0)
        self.assertEqual(b.test_point((0,0,-1)),NONE)
        b.nodes=[(0,NONE,1),(0,NONE,SIGN)]
        self.assertEqual(b.test_point((0,0,1)),0)

    def test_point_cycles_empty_and_invalid_node(self):
        for nodes in ([],[(0,0,0)],[(0,1,1),(0,0,0)],[(0,10,10)]):
            b=self.bsp();b.nodes=nodes
            with self.subTest(nodes=nodes),self.assertRaises(ValueError):b.test_point((0,0,1))

    def test_2d_front_back_and_root_leaf(self):
        b=self.bsp()
        self.assertEqual(b.bsp2d_test_point(0,(1,0)),0)
        self.assertEqual(b.bsp2d_test_point(0,(-1,0)),NONE)
        self.assertEqual(b.bsp2d_test_point(SIGN|3,(0,0)),3)
        self.assertEqual(b.bsp2d_test_point(NONE,(0,0)),NONE)
        b.nodes2d=[(1.,0.,0.,NONE,1),(1.,0.,0.,NONE,SIGN)]
        self.assertEqual(b.bsp2d_test_point(0,(1,0)),0)

    def test_2d_cycles_and_invalid_node(self):
        for nodes in ([],[(1.,0.,0.,0,0)],[(1.,0.,0.,1,1),(1.,0.,0.,0,0)],[(1.,0.,0.,4,4)]):
            b=self.bsp();b.nodes2d=nodes
            with self.subTest(nodes=nodes),self.assertRaises(ValueError):b.bsp2d_test_point(0,(0,0))

    def test_vector_normal_termination_and_repeated_calls(self):
        b=self.bsp()
        for _ in range(3):self.assertIsNone(b.test_vector((0,0,1),(0,0,-2)))
        b.nodes=[(0,1,1),(0,SIGN,SIGN)]
        # Shared child is a valid DAG, not an active-path cycle.
        self.assertIsNone(b.test_vector((0,0,1),(0,0,-2)))

    def test_vector_node_cycles_and_invalid_node(self):
        for nodes in ([],[(0,0,0)],[(0,1,1),(0,0,0)],[(0,10,10)]):
            b=self.bsp();b.nodes=nodes
            with self.subTest(nodes=nodes),self.assertRaises(ValueError):b.test_vector((0,0,1),(0,0,-2))

    def test_vector_rejects_nested_2d_cycle(self):
        b=self.bsp();b.leaves=[(0,1,0)];b.references=[(0,0)];b.nodes2d=[(1.,0.,0.,0,0)]
        with self.assertRaisesRegex(ValueError,'BSP2D'):
            b.test_vector((0,0,1),(0,0,-2))

if __name__=='__main__':unittest.main()
