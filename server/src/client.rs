
use crate::serde_json;

use std::{
    error::Error,
    fmt,
    io::{BufRead, BufReader, BufWriter, Read, Write},
    net::TcpStream,
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

// have c++ share 2 functions, 1 for sending data, 1 for receiving it (by atomically changing a single ref
#[no_mangle]
pub extern "C" fn start_client(
    server_string: String, // Change this to be FFI safe
    _send_data_func: unsafe extern "C" fn(Player),
    get_data_func: unsafe extern "C" fn() -> Player, // Could be a reference
) -> bool {
    let get_data_func_ptr = get_data_func as *const ();

    match try_connect(server_string) {
        Ok(stream) => {
            println!("Successfully connected to local server in port 7878");

            let reader_stream = stream;
            let mut writer_stream = reader_stream.try_clone().expect("Could'nt clone stream");

            let initial_response = b"EST CONNECTION\n";
            writer_stream.write(initial_response).unwrap();

            let mut buf_reader = BufReader::new(&reader_stream);

            let mut valid_con = true;
            let mut con_est = false;

            while valid_con {
                for line in buf_reader.by_ref().lines() {
                    let returned_line = match line {
                        Ok(line) => line,
                        Err(_) => "END CONNECTION".into(),
                    };

                    if returned_line == "CON EST, LOOKING SEARCHING FOR PLAYER" {
                        println!("Listening: Looking for player");
                        con_est = true;
                    } else if con_est {
                        if handle_communication(
                            &mut writer_stream,
                            &returned_line,
                            get_data_func_ptr,
                        )
                        .is_err()
                        {
                            valid_con = false;
                            break;
                        }
                    } else {
                        break;
                    }
                }
                thread::sleep(Duration::from_secs(2));
                // only ask for data ever 0.1 millisecond
                // thread::sleep(Duration::from_millis(100));
            }
            if valid_con {
                writer_stream.write(b"END CONNECTION\n").unwrap();
            }
            println!("SERVER: ENDING");
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

fn handle_communication(
    write_stream: &mut TcpStream,
    line: &str,
    data_from_frontend_ptr: *const (),
) -> Result<(), Box<dyn Error>> {
    if line == "END CONNECTION" {
        return Err(Box::new(MyError("Server ended Connection".into())));
    }

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
