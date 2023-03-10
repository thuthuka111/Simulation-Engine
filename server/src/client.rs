use crate::{Connection, Packet};

use std::{
    error::Error,
    ffi::CStr,
    fmt,
    net::{SocketAddr, TcpStream},
    os::raw::c_char,
    sync::Mutex,
    thread,
    time::Duration,
};

use crate::Player;

#[derive(Debug)]
struct MyError(String);

impl fmt::Display for MyError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "There is an error: {}", self.0)
    }
}

impl Error for MyError {}

struct ForeignFunc {
    func_ptr: *const (),
}

impl std::ops::Deref for ForeignFunc {
    type Target = *const ();

    fn deref(&self) -> &Self::Target {
        &self.func_ptr
    }
}

unsafe impl Send for ForeignFunc {}

// have c++ share 2 functions, 1 for sending data, 1 for receiving it (by atomically changing a single ref
#[no_mangle]
pub extern "C" fn start_client(
    server_string: *const c_char, // Change this to be FFI safe
    send_data_func: unsafe extern "C" fn(Player),
    get_data_func: unsafe extern "C" fn() -> Player, // Could be a reference
) -> bool {
    let get_data_func_ptr = get_data_func as *const ();
    let send_data_func = ForeignFunc {
        func_ptr: send_data_func as *const (),
    };
    let send_data_func_mut = Mutex::new(send_data_func);

    let server_string = {
        if server_string.is_null() {
            eprintln!("Server String is null");
            return false;
        }
        let s = unsafe { CStr::from_ptr(server_string) };
        match s.to_str() {
            Ok(s) => s,
            Err(_) => {
                eprintln!("Server String Not Valid UTF8 String");
                return false;
            }
        }
    };

    let server_addr: SocketAddr = {
        match server_string.parse() {
            Ok(addr) => addr,
            Err(_) => {
                eprintln!("Invalid Server Address");
                return false;
            }
        }
    };

    let tcp_stream = {
        match TcpStream::connect_timeout(&server_addr, Duration::from_secs(3)) {
            Ok(tcp_stream) => tcp_stream,
            Err(_) => {
                eprintln!("Failed to connect");
                return false;
            }
        }
    };

    thread::scope(|scope| {
        let mut game_has_ended = false;

        // Make this return a result that indicates whether the packet was malformed
        let on_message_callback = |packet: &Packet| {
            println!("Received: {:?}", packet);

            match packet {
                Packet::Message(message) => {
                    println!("Message from Server: {}", message);
                }
                Packet::Player(fresh_player_data) => {
                    println!("Received Player Update: {:?}", fresh_player_data);

                    if let Ok(send_data_func_ptr) = send_data_func_mut.lock() {
                        // unsafe {
                        //     std::mem::transmute::<*const (), fn(Player) -> Player>(
                        //         **send_data_func_ptr,
                        //     )(*fresh_player_data);
                        // }
                    }
                }
            }
            ()
        };

        let server_connection = Connection::new(tcp_stream, scope, on_message_callback);

        server_connection.send_message(Packet::Message("EST CONNECTION\n".into()));

        while !game_has_ended {
            // Polling Player Data from game and sending it

            let player_data: Player;
            unsafe {
                // calling a C function that should give us new player data
                // this function should be turned into a function that returns any data really, stating what it is
                player_data = std::mem::transmute::<*const (), fn() -> Player>(get_data_func_ptr)();
            }

            let send_data = Packet::Player(player_data);
            server_connection.send_message(send_data);

            // chang this to poll c++ every 0.1s
            thread::sleep(Duration::from_secs(5));
        }

        // server connection threads will be joined here
    });

    return true;
}
