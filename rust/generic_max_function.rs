use std::cmp::Ordering;
fn max<T:std::cmp::Ord + Clone>(list : Vec<T>)->T{
    let mut largest = &list[0];
    for element in &list{
        if matches!(element.cmp(&largest), Ordering::Greater) {
            largest = element;
        }
    }
    return largest.to_owned()
}

fn main() {
    let number_list = vec![34, 50, 25, 100, 65];

    let largest ;

    largest = max(number_list);

    println!("The largest number is {largest}");
}
