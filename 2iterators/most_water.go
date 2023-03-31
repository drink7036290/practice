package main

import (
	"fmt"
	"math"
)

func getMostWater(heights []int) int {
	left, right := 0, len(heights)-1
	maxVolume := math.MinInt32

	for left < right {
		minHeight := int(math.Min(float64(heights[left]), float64(heights[right])))
		maxVolume = int(math.Max(float64(maxVolume), float64(minHeight*(right-left))))

		if heights[left] == minHeight {
			left++
		} else {
			right--
		}
	}

	return maxVolume
}

func main() {
	heights := []int{1, 8, 6, 2, 5, 4, 8, 3, 7}
	fmt.Println(getMostWater(heights))

	// heights := []int{1, 1}
	// fmt.Println(getMostWater(heights))
}
