use std::{error::Error, io};

use tinkerforge::{ip_connection::IpConnection, temperature_ir_v2_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Temperature IR Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let tir = TemperatureIrV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get current ambient temperature.
    let ambient_temperature = tir.get_ambient_temperature().recv()?;
    println!(
        "Ambient Temperature: {} °C",
        ambient_temperature as f32 / 10.0
    );

    // Get current object temperature.
    let object_temperature = tir.get_object_temperature().recv()?;
    println!(
        "Object Temperature: {} °C",
        object_temperature as f32 / 10.0
    );

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
