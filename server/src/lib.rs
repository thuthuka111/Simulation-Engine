pub mod client;
pub mod server;

use std::ffi::c_float;

#[repr(C)]
#[derive(Debug)]
pub struct Position {
    x: c_float,
    y: c_float,
}

// remove this and deduce the velocity on the client side
#[repr(C)]
#[derive(Debug)]
pub struct Velocity {
    x: c_float,
    y: c_float,
}

#[repr(C)]
#[derive(Debug)]
pub struct Player {
    rigid_body_id: u32,
    position: Position,
    velocity: Velocity,
}
