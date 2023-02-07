use std::{
    error::Error,
    fmt,
    io::{self, BufRead, BufReader, BufWriter, Read, Write},
    net::TcpStream,
    str::from_utf8,
};

use crate::Player;

use std::sync::mpsc;

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
    _get_data_func: unsafe extern "C" fn() -> Player,
) -> bool {
    match try_connect(server_string) {
        Ok(mut stream) => {
            println!("Successfully connected to local server in port 7878");

            let mut reader_stream = stream;
            let mut writer_stream = reader_stream.try_clone().expect("Could'nt clone stream");

            let initial_response = b"EST CONNECTION\n";
            writer_stream.write(initial_response).unwrap();

            let mut buf_reader = BufReader::new(&reader_stream);

            let mut still_listening = true;
            let mut con_est = false;

            while still_listening {
                for line in buf_reader.by_ref().lines() {
                    let returned_line = line.expect("Cannot read line");

                    if returned_line == "CON EST, LOOKING SEARCHING FOR PLAYER" {
                        println!("Listening: Looking for player");
                        con_est = true;
                    } else if con_est {
                        if handle_communication(&mut writer_stream, &returned_line).is_err() {
                            still_listening = false;
                        }
                    } else {
                        break;
                    }
                }
            }
            return true;
        }
        Err(e) => {
            eprint!("Failed to connect: {}", e);
            return false;
        }
    }
}

fn try_connect(addr: String) -> Result<TcpStream, Box<dyn Error>> {
    let stream = TcpStream::connect(addr)?;
    Ok(stream)
}

fn handle_communication(write_stream: &mut TcpStream, line: &str) -> Result<(), Box<dyn Error>> {
    println!("The recieved line: {}", line);
    write_stream.write(b"SOME SHIT")?;
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
