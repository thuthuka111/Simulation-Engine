use std::ffi::c_float;

#[derive(Debug)]
struct Position {
    x: c_float,
    y: c_float,
}

// remove this and deduce the velocity on the client side
#[derive(Debug)]
struct Velocity {
    x: c_float,
    y: c_float,
}

#[derive(Debug)]
struct Player {
    rigid_body_id: u32,
    position: Position,
    velocity: Velocity,
}

pub mod server {
    use super::*;
    use std::sync::mpsc::Sender;
    use std::sync::{mpsc, Arc, Mutex};
    use std::thread;
    use std::{
        io::{prelude::*, BufReader},
        net::{TcpListener, TcpStream},
    };

    #[no_mangle]
    pub extern "C" fn server_start() {
        let listener = TcpListener::bind("127.0.0.1:7878").unwrap();
        let (tx, rx) = mpsc::channel();

        let mut thread_handles = vec![];

        thread_handles.push(thread::spawn(move || {
            let player_count = Arc::new(Mutex::new(0));
            let tx = Arc::new(Mutex::new(tx));

            listen_for_connections(&listener, &tx, &player_count);
        }));

        thread_handles.push(thread::spawn(move || {
            let mut other_player: Option<(TcpStream, Player)> = None;
            let mut can_start_game = false;

            // Match Clients with a Connection
            for received in rx {
                if can_start_game {
                    let this_player = other_player.as_mut().unwrap();
                    run_game_instance(&this_player, &received);

                    can_start_game = false;
                } else {
                    other_player = Some(received);

                    can_start_game = true;
                }
            }
        }));

        for thread_handle in thread_handles {
            thread_handle.join().unwrap();
        }
    }

    fn listen_for_connections(
        listener: &TcpListener,
        tx: &Arc<Mutex<Sender<(TcpStream, Player)>>>,
        player_count: &Arc<Mutex<u32>>,
    ) {
        let mut thread_handles = vec![];

        for stream in listener.incoming() {
            match stream {
                Ok(mut stream) => {
                    println!("New connection: {}", stream.peer_addr().unwrap());

                    match valid_game_connection(&stream) {
                        Ok(_) => {
                            // Send to Wait for another person
                            let response = String::from("CON EST, LOOKING SEARCHING FOR PLAYER\n");
                            stream.write_all(response.as_bytes()).unwrap();

                            let player_count = Arc::clone(&player_count);
                            let tx = Arc::clone(&tx);
                            let thread_handle = thread::spawn(move || {
                                let mut player_id = player_count.lock().unwrap();
                                let tx = tx.lock().unwrap();

                                (*tx)
                                    .send((stream, Player::new(*player_id)))
                                    .expect("Receiver not there for some reason");

                                *player_id += 1;
                            });

                            thread_handles.push(thread_handle);
                        }
                        Err(()) => {
                            eprintln!("FUGAZI");
                            // Not a Valid game connection, end connection
                        }
                    }
                }
                Err(e) => {
                    eprintln!("Error: {}", e);
                }
            }
        }

        for thread_handle in thread_handles {
            thread_handle.join().unwrap();
        }
    }

    fn valid_game_connection(mut tcp_stream: &TcpStream) -> Result<(), ()> {
        let buf_reader = BufReader::new(&mut tcp_stream);
        let request_line = buf_reader.lines().next().unwrap().unwrap(); // This breaks if there is no \r\n

        println!("{}", request_line);
        if request_line == "EST CONNECTION" {
            Ok(())
        } else {
            Err(())
        }
    }

    fn run_game_instance(p1: &(TcpStream, Player), p2: &(TcpStream, Player)) {
        println!("Player 1: {:?}", p1);
        println!("Player 2: {:?}", p2);
    }

    impl Player {
        fn new(id: u32) -> Player {
            Player {
                rigid_body_id: id,
                position: Position {
                    x: 0.0f32,
                    y: 0.0f32,
                },
                velocity: Velocity {
                    x: 0.0f32,
                    y: 0.0f32,
                },
            }
        }
    }
}

pub mod client {
    use std::{
        io::{BufRead, BufReader, Read, Write},
        net::TcpStream,
        str::from_utf8,
    };

    use super::*;

    #[no_mangle]
    pub extern "C" fn start_client() {
        match TcpStream::connect("localhost:7878") {
            Ok(mut stream) => {
                println!("Successfully connected to local server in port 7878");

                let response = b"EST CONNECTION\r\n";

                stream.write(response).unwrap();

                let mut buf_reader = BufReader::new(&mut stream);

                let mut con_est = false;

                for line in buf_reader.lines() {
                    let returned_line = line.expect("Cannot read new line");

                    if returned_line == "CON EST, LOOKING SEARCHING FOR PLAYER" {
                        println!("Listening for more data");
                        con_est = true;
                    } else if con_est {
                        println!("The recieved line: {}", returned_line);

                        println!("Listening for more data");
                    } else {
                        break;
                    }
                }
            }
            Err(e) => {
                eprint!("Failed to connect: {}", e);
            }
        }
    }
}
