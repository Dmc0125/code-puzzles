package main

import (
	"fmt"
)

type TreeNode struct {
	Val   int
	Left  *TreeNode
	Right *TreeNode
}

func invertTreeDFS(root *TreeNode) *TreeNode {
	if root == nil {
		return nil
	}

	invertTreeDFS(root.Left)
	invertTreeDFS(root.Right)

	tmp := root.Right
	root.Right = root.Left
	root.Left = tmp

	return root
}

func queueBFS(root *TreeNode) []*TreeNode {
	queue := []*TreeNode{root}
	i := 0

	for i < len(queue) {
		n := queue[i]
		i += 1

		if n.Left != nil {
			queue = append(queue, n.Left)
		}
		if n.Right != nil {
			queue = append(queue, n.Right)
		}
	}

	return queue
}

func (root *TreeNode) collect() []int {
	var result []int
	queue := []*TreeNode{root}
	i := 0

	for i < len(queue) {
		n := queue[i]
		i += 1
		result = append(result, n.Val)
		if n.Left != nil {
			queue = append(queue, n.Left)
		}
		if n.Right != nil {
			queue = append(queue, n.Right)
		}
	}

	return result
}

func main() {
	t1 := &TreeNode{
		Val: 4,
		Left: &TreeNode{
			Val: 2,
			Left: &TreeNode{
				Val: 1,
			},
			Right: &TreeNode{
				Val: 3,
			},
		},
		Right: &TreeNode{
			Val: 7,
			Left: &TreeNode{
				Val: 6,
			},
			Right: &TreeNode{
				Val: 9,
			},
		},
	}

	fmt.Println(t1.collect())
	invertTreeDFS(t1)
	fmt.Println(t1.collect())

	q := queueBFS(t1)
	for _, t := range q {
		fmt.Println(t.Val)
	}
}
