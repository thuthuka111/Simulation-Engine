use crate::*;
use std::net::{TcpListener, TcpStream};
use std::sync::{mpsc, Arc, Mutex};
use std::thread;
use std::time::Duration;

#[no_mangle]
pub extern "C" fn server_start() {
    let listener = TcpListener::bind("127.0.0.1:7878").unwrap();

    let (tx, rx) = mpsc::channel();
    let tx = Arc::new(Mutex::new(tx));

    let mut thread_handles = vec![];

    thread_handles.push(thread::spawn(move || {
        let tx = Arc::clone(&tx);

        loop {
            let p1_tcp = listener.accept().unwrap();
            let p2_tcp = listener.accept().unwrap();

            if let Ok(tx) = tx.lock() {
                (*tx)
                    .send((p1_tcp.0, p2_tcp.0))
                    .expect("Receiver not there for some reason");
            }
        }
    }));

    for pair in rx.iter() {
        run_game_instance(pair.0, pair.1);
    }

    for thread_handle in thread_handles {
        thread_handle.join().unwrap();
    }
}

fn run_game_instance(p1_stream: TcpStream, p2_stream: TcpStream) {
    thread::scope(|scope| {
        let p1_on_packet = |packet: &Packet| {
            println!("FROM P1: {:?}", packet);
        };
        let p2_on_packet = |packet: &Packet| {
            println!("FROM P2: {:?}", packet);
        };

        let p1_conn = Connection::new(p1_stream, scope, p1_on_packet);
        let p2_conn = Connection::new(p2_stream, scope, p2_on_packet);

        loop {
            let message = Message {
                message: "hello cunt 1".into(),
            };
            let message_for_clients = Packet {
                header: "MESSAGE".into(),
                data: serde_json::to_string(&message).expect("Malformed Message Data"),
            };

            p1_conn.send_message(message_for_clients);

            let message = Message {
                message: "hello cunt 2".into(),
            };
            let message_for_clients = Packet {
                header: "MESSAGE".into(),
                data: serde_json::to_string(&message).expect("Malformed Message Data"),
            };

            println!("Sent both clients a message");
            p2_conn.send_message(message_for_clients);

            thread::sleep(Duration::from_secs(2));
        }
    });
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
