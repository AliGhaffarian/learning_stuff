struct Solution{

}

impl Solution {
    pub fn is_circular_sentence(sentence: String) -> bool {
        let do_log = true;
        let sentence_to_bytes = sentence.clone().into_bytes();

        let space = ' ' as u8;

        if sentence_to_bytes.iter().nth(0).unwrap() != sentence_to_bytes.iter().nth_back(0).unwrap()
            || sentence_to_bytes.iter().nth(0).unwrap() == &space
        {
            if do_log {println!("false cuz first != last");}
            return false
        }

        if sentence_to_bytes.len() < 3 {return true}
        for i in 0..=sentence_to_bytes.len() - 2{
            if sentence_to_bytes.iter().nth(i + 1).expect("wtf??") == &space {
                if sentence_to_bytes.iter().nth(i).expect("failed to i") !=  sentence_to_bytes.iter().nth(i+2).expect("failed to i+2"){
                    if do_log{
                    println!("false cuz {} != {}", i, i+2);
                    }
                    return false
                }
            }

        }
        true
    }
}


fn main() {
    println!("{}", Solution::is_circular_sentence(String::from("eetcode")));

}
