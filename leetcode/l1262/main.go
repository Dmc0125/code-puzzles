package main

import (
	"fmt"
)

//	func maxSumDivThree(nums []int) int {
//		sum := 0
//		var rem1, rem2 []int
//
//		cmp := func(a, b int) int {
//			return a - b
//		}
//
//		for _, n := range nums {
//			sum += n
//
//			switch n % 3 {
//			case 1:
//				rem1 = append(rem1, n)
//			case 2:
//				rem2 = append(rem2, n)
//			}
//		}
//
//		slices.SortFunc(rem1, cmp)
//		slices.SortFunc(rem2, cmp)
//
//		rem := sum % 3
//
//		subtract := func(r1, r2 []int) int {
//			if len(r2) < 2 {
//				return r1[0]
//			}
//
//			r2Sum := r2[0] + r2[1]
//
//			if r1[0] <= r2Sum {
//				return r1[0]
//			} else {
//				return r2Sum
//			}
//		}
//
//		switch rem {
//		// if rem == 1 we can subtract
//		// - 1 from rem1
//		// - 2 from rem2
//		case 1:
//			sum -= subtract(rem1, rem2)
//		// if rem == 2 we can subtract
//		// - 1 from rem2
//		// - 2 from 1
//		case 2:
//			if len(rem1) >= 2 && len(rem2) == 0 {
//				sum -= (rem1[0] + rem1[1])
//			} else {
//				sum -= subtract(rem2, rem1)
//			}
//		}
//
//		return sum
//	}

func maxSumDivThree(nums []int) int {
	sum := 0
	const p = 10001
	rem1, rem2 := [2]int{p, p}, [2]int{p, p}

	for _, n := range nums {
		sum += n

		switch n % 3 {
		case 1:
			if n < rem1[0] {
				rem1[1] = rem1[0]
				rem1[0] = n
			} else if n < rem1[1] {
				rem1[1] = n
			}
		case 2:
			if n < rem2[0] {
				rem2[1] = rem2[0]
				rem2[0] = n
			} else if n < rem2[1] {
				rem2[1] = n
			}
		}
	}

	rem := sum % 3
	if rem == 0 {
		return sum
	}

	var toSub int

	switch rem {
	// if rem == 1 we can subtract
	// - 1 from rem1
	// - 2 from rem2
	case 1:
		toSub = rem1[0]
		if rem2[1] < p && rem2[0]+rem2[1] < toSub {
			toSub = rem2[0] + rem2[1]
		}
	// if rem == 2 we can subtract
	// - 1 from rem2
	// - 2 from 1
	case 2:
		if rem2[0] == p {
			toSub = rem1[0] + rem1[1]
		} else if rem1[1] < p && rem1[0]+rem1[1] < rem2[0] {
			toSub = rem1[0] + rem1[1]
		} else {
			toSub = rem2[0]
		}
	}

	return sum - toSub
}

func main() {
	inputs := [][]int{
		{3, 6, 5, 1, 8},
		{4},
		{1, 2, 3, 4, 4},
		{2, 6, 2, 2, 7},
		{4, 1, 5, 3, 1},
	}

	for _, i := range inputs {
		n := maxSumDivThree(i)
		fmt.Println(n)
	}
}
