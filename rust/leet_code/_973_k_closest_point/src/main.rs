use std::cmp::Ordering;

struct Solution{

}
impl Solution {
    pub fn k_closest(mut points: Vec<Vec<i32>>, k: i32) -> Vec<Vec<i32>> {
        return Self::_k_closest( &mut points, k)
    }
    fn _k_closest(mut points: &mut Vec<Vec<i32>>, k: i32) -> Vec<Vec<i32>> {

        let points_len = points.len();

        if k < 0 || points.len() == 0{
            return Vec::<Vec<i32>>::new()
        }

        let ip = Solution::partition(&mut points, 0, points_len as i32 - 1);
        println!("k {}, ip {}, list {:?}",k, ip, points);

        match ip.cmp(&((k) as usize)){
            Ordering::Less => {
                println!("adding {} first elements to result and going right", ip);
                let mut result = points[0..=ip].to_vec();
                    result.append(
                        &mut Self::_k_closest(
                            &mut points[ip+1..=points_len-1].to_vec(), k - ip as i32 - 1)
                        );
                    return result.to_vec();
            } //add left to result and partition right
            Ordering::Greater => {
                println!("ip {}, k {} going left", ip, k);
                return Self::_k_closest(&mut points[0..ip].to_vec(), k).to_vec()
            } //partition left
            Ordering::Equal => {
                println!("returning {} first elements", ip);
                return points[0..ip].to_vec()
        }
    }
    }
    fn distance_to_center(x : i32, y : i32)->f64{
        ((x * x + y * y) as f64).sqrt()

    }
    fn partition(arr: &mut Vec<Vec<i32>>, left: i32, right: i32) -> usize {
        let pivot = right;
        let mut i: i32 = left as i32 - 1;

        for j in left..=right - 1 {
            if Solution::distance_to_center(arr[j as usize][0], arr[j as usize][1]) 
                <= Solution::distance_to_center(arr[pivot as usize][0], arr[pivot as usize][1]){
                i += 1;

                arr.swap(i as usize, j as usize);
            }
        }

        arr.swap((i + 1) as usize, pivot as usize);

        (i + 1) as usize
    }

}

fn main() { 
    println!("{:?}", Solution::k_closest(
            vec![vec![68,97],vec![34,-84],vec![60,100],vec![2,31],vec![-27,-38],vec![-73,-74],vec![-55,-39],vec![62,91],vec![62,92],vec![-57,-67]]
            ,5
            ))
}
