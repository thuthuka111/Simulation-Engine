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
    use std::sync::mpsc::Sender;
    use std::sync::{mpsc, Arc, Mutex};
    use std::thread;
    use std::{
        io::{prelude::*, BufReader},
        net::{TcpListener, TcpStream},
    };
    use Velocity;
    use {Player, Position};

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
            let mut other_player: Option<Player> = None;
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

    fn run_game_instance(p1: &Player, p2: &Player) {
        println!("Player 1: {:?}", p1);
        println!("Player 2: {:?}", p2);
    }

    fn listen_for_connections(
        listener: &TcpListener,
        tx: &Arc<Mutex<Sender<Player>>>,
        player_count: &Arc<Mutex<u32>>,
    ) {
        let mut thread_handles = vec![];

        for stream in listener.incoming() {
            match stream {
                Ok(stream) => {
                    println!("New connection: {}", stream.peer_addr().unwrap());
                    let player_count = Arc::clone(&player_count);
                    let tx = Arc::clone(&tx);
                    let thread_handle = thread::spawn(move || {
                        let mut player_id = player_count.lock().unwrap();
                        let tx = tx.lock().unwrap();

                        (*tx)
                            .send(Player::new(*player_id))
                            .expect("Receiver not there for some reason");

                        *player_id += 1;
                    });

                    thread_handles.push(thread_handle);
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
