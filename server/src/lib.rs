pub mod client;
pub mod server;

extern crate serde;
extern crate serde_json;

use serde::{Deserialize, Serialize};
use std::ffi::c_float;

#[repr(C)]
#[derive(Debug, Serialize, Deserialize)]
pub struct Position {
    x: c_float,
    y: c_float,
}

// remove this and deduce the velocity on the client side
#[repr(C)]
#[derive(Debug, Serialize, Deserialize)]
pub struct Velocity {
    x: c_float,
    y: c_float,
}

#[repr(C)]
#[derive(Debug, Serialize, Deserialize)]
pub struct Player {
    rigid_body_id: u32,
    position: Position,
    velocity: Velocity,
}

use std::{
    io::{BufRead, BufReader, Write},
    marker,
    net::TcpStream,
    sync::{mpsc, Arc, Mutex},
    thread,
};

#[derive(Debug, Serialize, Deserialize)]
pub enum Packet {
    Message(String),
    Player(Player),
}

#[derive(Debug, Serialize, Deserialize)]
pub struct Message {
    message: String,
}

pub struct Connection<'a, F>
where
    F: (Fn(&Packet) -> ()) + Send + 'a,
{
    tx: Arc<Mutex<mpsc::Sender<Packet>>>,
    _thread_handles: Vec<thread::ScopedJoinHandle<'a, ()>>,
    phantom: marker::PhantomData<&'a F>,
}

impl<'a, F> Connection<'a, F>
where
    F: (Fn(&Packet) -> ()) + Send + 'a,
{
    fn new(tcp_stream: TcpStream, scope: &'a thread::Scope<'a, '_>, func: F) -> Self {
        let (tx, rx) = mpsc::channel::<Packet>();

        let tx = Arc::new(Mutex::new(tx));
        let tx_clone = Arc::clone(&tx);

        let mut writer_stream = tcp_stream.try_clone().unwrap();
        let reader_stream = writer_stream.try_clone().unwrap();

        // give ability to panic
        let thread_handles = vec![
            scope.spawn(move || {
                let _tx = tx_clone;
                let buf_reader = BufReader::new(reader_stream);

                for packet in buf_reader.lines() {
                    match packet {
                        Ok(packet) => {
                            let packet: Packet =
                                serde_json::from_str(&packet).expect("Malformed Data");

                            func(&packet);
                            // implement this later, if it makes sense
                            // if let Some(return_packet) = func(&packet) {
                            //     if let Ok(tx) = tx.lock() {
                            //         (*tx).send("string tbd".into()).unwrap();
                            //     }
                            // }
                        }
                        Err(_) => {
                            panic!("Connection closed - EXITING");
                            // println!("SERVER ERROR");
                            // break;
                        }
                    }
                }
            }),
            scope.spawn(move || {
                for received in rx {
                    // exit loop on on error

                    if let Err(_) = writer_stream.write(
                        format!(
                            "{}\n",
                            serde_json::to_string(&received).expect("Malformed Data")
                        )
                        .as_bytes(),
                    ) {
                        break;
                    }
                }
            }),
        ];

        Self {
            tx,
            _thread_handles: thread_handles,
            phantom: marker::PhantomData,
        }
    }

    fn send_message(&self, message: Packet) {
        if let Ok(tx) = self.tx.lock() {
            (*tx).send(message).expect("Server connection closed");
        }
    }
}

#[cfg(test)]
mod tests {}
