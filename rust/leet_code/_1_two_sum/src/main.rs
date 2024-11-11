use std::cmp::Ordering;
struct Solution{

}
impl Solution {
    pub fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
        let mut nums_pairs : Vec<(i32,i32)> = Vec::new();

        for i in 0..nums.len(){
            nums_pairs.push((nums[i], i as i32));
        }

        nums_pairs.sort_by(|a, b| a.0.cmp(&b.0));

        for i in 0..nums.len(){
            let mut j = nums.len() - 1;
            while nums_pairs[j].0 + nums_pairs[i].0 > target {j-=1};
            if nums_pairs[j].0 + nums_pairs[i].0 == target {
                return vec![nums_pairs[i].1 , nums_pairs[j].1]
            }


        }

        return vec![-1,-1];
        
    }
}
fn main() {
    println!("{:?}", Solution::two_sum(vec![3,2,4], 6));
}
