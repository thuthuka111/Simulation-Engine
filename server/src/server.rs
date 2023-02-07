
use crate::*;
use std::sync::mpsc::Sender;
use std::sync::{mpsc, Arc, Mutex};
use std::thread;
use std::time::Duration;
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
        for mut received in rx {
            if can_start_game {
                let mut this_player = other_player.as_mut().unwrap();
                run_game_instance(&mut this_player, &mut received);

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

fn run_game_instance(p1: &mut (TcpStream, Player), p2: &mut (TcpStream, Player)) {
    println!("Player 1: {:?}", p1);
    println!("Player 2: {:?}", p2);

    loop {
        p1.0.write(b"This is some Data for p1\n").unwrap();
        p2.0.write(b"This is some Data for p2\n").unwrap();

        thread::sleep(Duration::from_secs(2));
    }
}

impl Player {
    pub fn new(id: u32) -> Player {
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
