extern crate basketball_networking;
use std::ffi::{CString};

use basketball_networking::{client, Player};

fn main() {
    let server_string = CString::new("127.0.0.1:7878").unwrap();
    client::start_client(server_string.as_ptr(), send_data, get_data);
}

extern "C" fn send_data(_player: Player) {
    print!("Data has being recieved from rust server");
}

extern "C" fn get_data() -> Player {
    println!("Sending rust some data");
    Player::new(45)
}
