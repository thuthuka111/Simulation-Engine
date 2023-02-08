use crate::serde_json;

use std::{
    error::Error,
    fmt,
    io::{BufRead, BufReader, BufWriter, Read, Write},
    marker,
    net::TcpStream,
    sync::{mpsc, Arc, Mutex},
    thread::{self, JoinHandle},
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

// have c++ share 2 functions, 1 for sending data, 1 for receiving it (by atomically changing a single ref
#[no_mangle]
pub extern "C" fn start_client(
    server_string: String, // Change this to be FFI safe
    _send_data_func: unsafe extern "C" fn(Player),
    get_data_func: unsafe extern "C" fn() -> Player, // Could be a reference
) -> bool {
    let get_data_func_ptr = get_data_func as *const ();

    match TcpStream::connect(server_string) {
        Ok(tcp_stream) => {
            thread::scope(|scope| {
                let on_message_callback = |data: &str| {
                    println!("Received: {}", data);

                    // call the handle_communication function
                    None
                };

                let server_connection = Connection::new(tcp_stream, scope, on_message_callback);

                server_connection.send_message("EST CONNECTION\n".into());

                // server connection threads will be joined here
            });

            return true;
        }
        Err(e) => {
            eprint!("Failed to connect: {}", e);
            return false;
        }
    }
}

// Return an Optional String to be sent to the other server
fn handle_communication(
    write_stream: &mut TcpStream,
    line: &str,
    data_from_frontend_ptr: *const (),
) -> Result<(), Box<dyn Error>> {
    if line == "END CONNECTION" {
        return Err(Box::new(MyError("Server ended Connection".into())));
    }

    println!("Here");
    let new_player: Option<Player>;
    unsafe {
        // calling a C function that should give us new player data
        // this function should be turned into a function that returns any data really, stating what it is
        new_player = Some(std::mem::transmute::<*const (), fn() -> Player>(
            data_from_frontend_ptr,
        )());
    }
    let new_player = new_player.expect("No New Player Data");
    let send_data = {
        let data_type = "PLAYER UPDATE";
        let new_player = serde_json::to_string(&new_player).expect("Bad Data");
        format!("{}\n{}\n", data_type, new_player)
    };

    println!("Wrintign the followingn:\n{}", send_data);
    write_stream.write(send_data.as_bytes())?;
    Ok(())
}

fn _validate_connection<'a, 'b>(
    buf_reader: &'a mut BufReader<&'a TcpStream>,
    buf_writer: &'b mut BufWriter<&'b TcpStream>,
) -> Result<
    (
        &'a mut BufReader<&'a TcpStream>,
        &'b mut BufWriter<&'b TcpStream>,
    ),
    Box<dyn Error>,
> {
    let response = b"EST CONNECTION\r\n";

    buf_writer.write(response)?;

    let response_line = buf_reader.lines().next().unwrap()?;

    if response_line == "CON EST, LOOKING SEARCHING FOR PLAYER" {
        Ok((buf_reader, buf_writer))
    } else {
        Err(Box::new(MyError("Oops".into())))
    }
}

struct Connection<'a, F>
where
    F: (Fn(&str) -> Option<String>) + Send + 'a,
{
    tx: Arc<Mutex<mpsc::Sender<String>>>,
    thread_handles: Vec<thread::ScopedJoinHandle<'a, ()>>,
    phantom: marker::PhantomData<&'a F>,
}

impl<'a, F> Connection<'a, F>
where
    F: (Fn(&str) -> Option<String>) + Send + 'a,
{
    fn new(tcp_stream: TcpStream, scope: &'a thread::Scope<'a, '_>, func: F) -> Self {
        let (tx, rx) = mpsc::channel::<String>();

        let tx = Arc::new(Mutex::new(tx));
        let tx_clone = Arc::clone(&tx);

        let mut writer_stream = tcp_stream.try_clone().unwrap();
        let reader_stream = writer_stream.try_clone().unwrap();

        let thread_handles = vec![
            scope.spawn(move || {
                let tx = tx_clone;
                let mut buf_reader = BufReader::new(reader_stream);

                for line in buf_reader.lines() {
                    match line {
                        Ok(line) => {
                            func(&line);

                            // have this return an optional Response
                            if let Ok(tx) = tx.lock() {
                                (*tx).send("string tbd".into()).unwrap();
                            }
                        }
                        Err(_) => {
                            println!("SERVER ERROR");
                            break;
                        }
                    }
                }
            }),
            scope.spawn(move || {
                for received in rx {
                    println!("Going to send the following: {}", received);

                    // exit loop on on error
                    if let Err(_) = writer_stream.write(received.as_bytes()) {
                        break;
                    }
                }
            }),
        ];

        Self {
            tx,
            thread_handles,
            phantom: marker::PhantomData,
        }
    }

    fn send_message(&self, message: String) {
        if let Ok(tx) = self.tx.lock() {
            (*tx).send(message).expect("Server connection closed");
        }
    }
}
