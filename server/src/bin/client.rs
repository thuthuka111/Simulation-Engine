extern crate basketball_networking;
use basketball_networking::{client, Player};

fn main() {
    client::start_client("127.0.0.1:7878".into(), send_data, get_data);
}

extern "C" fn send_data(_player: Player) {
    print!("Data has being recieved from rust server");
}

extern "C" fn get_data() -> Player {
    println!("Sending rust some data");
    Player::new(45)
}
